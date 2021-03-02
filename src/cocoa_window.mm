/* ReaImGui: ReaScript binding for dear imgui
 * Copyright (C) 2021  Christian Fillion
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "window.hpp"

#include "context.hpp"
#include "inputview.hpp"
#include "opengl_renderer.hpp"

#include <WDL/wdltypes.h>
#include <reaper_plugin_functions.h>

static_assert(__has_feature(objc_arc),
  "This file must be built with automatic reference counting enabled.");

struct Window::Impl {
  static int translateAccel(MSG *msg, accelerator_register_t *accel);

  Context *ctx;
  HwndPtr hwnd;
  NSView *view;
  InputView *inputView;
  accelerator_register_t accel { &translateAccel, true, this };
  ImDrawData *lastDrawData     {};
  NSOpenGLContext *gl;
  OpenGLRenderer *renderer;
};

Window::Window(const char *title, RECT rect, Context *ctx)
  : m_impl { std::make_unique<Impl>() }
{
  const NSPoint position {
    static_cast<float>(rect.left), static_cast<float>(rect.top)
  };
  const NSSize size {
    static_cast<float>(rect.right - rect.left),
    static_cast<float>(rect.bottom - rect.top)
  };

  HWND hwnd { createSwellDialog(title) };
  SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(ctx));
  ShowWindow(hwnd, SW_SHOW);

  m_impl->ctx = ctx;
  m_impl->hwnd = HwndPtr { hwnd };
  m_impl->view = (__bridge NSView *)hwnd; // SWELL_hwndChild inherits from NSView
  [[m_impl->view window] setColorSpace:[NSColorSpace sRGBColorSpace]];
  [[m_impl->view window] setFrameOrigin:position];
  [[m_impl->view window] setContentSize:size];

  constexpr NSOpenGLPixelFormatAttribute attrs[] {
    NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
    NSOpenGLPFAAccelerated,
    NSOpenGLPFADoubleBuffer,
    kCGLPFASupportsAutomaticGraphicsSwitching,
    0
  };

  NSOpenGLPixelFormat *fmt { [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs] };
  m_impl->gl = [[NSOpenGLContext alloc] initWithFormat:fmt shareContext:nil];
  if(!m_impl->gl)
    throw reascript_error { "failed to initialize OpenGL 3.2 core context" };

  [m_impl->view setWantsBestResolutionOpenGLSurface:YES]; // retina
  [m_impl->gl setView:m_impl->view];

  [m_impl->gl makeCurrentContext];
  m_impl->renderer = new OpenGLRenderer;
  [m_impl->gl flushBuffer]; // avoid a quick flash of undefined pixels
  [NSOpenGLContext clearCurrentContext];

  m_impl->inputView = [[InputView alloc] initWithContext:ctx parent:m_impl->view];

  // Temprarily enable repeat character input
  // WARNING: this is application-wide!
  NSUserDefaults *defaults { [NSUserDefaults standardUserDefaults] };
  [defaults registerDefaults:@{@"ApplePressAndHoldEnabled":@NO}];

  plugin_register("accelerator", &m_impl->accel);

  ImGuiIO &io { ImGui::GetIO() };
  io.ConfigMacOSXBehaviors = false; // don't swap Cmd/Ctrl, SWELl already does it
  io.BackendPlatformName = "reaper_imgui_cocoa";
  setupMacOSKeyMap(io);
}

Window::~Window()
{
  plugin_register("-accelerator", &m_impl->accel);

  [m_impl->gl makeCurrentContext];
  delete m_impl->renderer;
}

HWND Window::nativeHandle() const
{
  return m_impl->hwnd.get();
}

void Window::beginFrame()
{
  m_impl->lastDrawData = nullptr;
}

void Window::drawFrame(ImDrawData *drawData)
{
  m_impl->lastDrawData = drawData;
  [m_impl->gl makeCurrentContext];
  m_impl->renderer->draw(drawData, m_impl->ctx->clearColor());
  [m_impl->gl flushBuffer];
  [NSOpenGLContext clearCurrentContext];
}

void Window::endFrame()
{
}

float Window::scaleFactor() const
{
  return [[m_impl->view window] backingScaleFactor];
}

bool Window::handleMessage(const unsigned int msg, WPARAM, LPARAM)
{
  switch(msg) {
  case WM_SIZE:
    [m_impl->gl update];
    if(m_impl->lastDrawData)
      drawFrame(m_impl->lastDrawData);
    return true;
  }

  return false;
}

int Window::Impl::translateAccel(MSG *msg, accelerator_register_t *accel)
{
  enum { NotOurWindow = 0, EatKeystroke = 1 };

  auto *impl { static_cast<Window::Impl *>(accel->user) };
  if(impl->hwnd.get() != msg->hwnd && !IsChild(impl->hwnd.get(), msg->hwnd))
    return NotOurWindow;

  [[impl->view window] sendEvent:[NSApp currentEvent]];

  return EatKeystroke;
}
