/* ReaImGui: ReaScript binding for Dear ImGui
 * Copyright (C) 2021-2023  Christian Fillion
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

#include "helper.hpp"

API_SECTION("Button",
R"(Most widgets return true when the value has been changed or when pressed/selected.

You may also use one of the many IsItem* functions (e.g. IsItemActive,
IsItemHovered, etc.) to query widget state.)");

DEFINE_API(bool, Button, (ImGui_Context*,ctx)
(const char*,label)(double*,API_RO(size_w),0.0)(double*,API_RO(size_h),0.0),
"")
{
  FRAME_GUARD;
  return ImGui::Button(label, ImVec2(API_RO_GET(size_w), API_RO_GET(size_h)));
}

DEFINE_API(bool, SmallButton, (ImGui_Context*,ctx)
(const char*,label),
"Button with StyleVar_FramePadding=(0,0) to easily embed within text.")
{
  FRAME_GUARD;
  return ImGui::SmallButton(label);
}

DEFINE_API(bool, InvisibleButton, (ImGui_Context*,ctx)
(const char*,str_id)(double,size_w)(double,size_h)
(int*,API_RO(flags),ImGuiButtonFlags_None),
R"(Flexible button behavior without the visuals, frequently useful to build
custom behaviors using the public api (along with IsItemActive, IsItemHovered, etc.).)")
{
  FRAME_GUARD;
  return ImGui::InvisibleButton(str_id, ImVec2(size_w, size_h), API_RO_GET(flags));
}

DEFINE_API(bool, ArrowButton, (ImGui_Context*,ctx)
(const char*,str_id)(int,dir),
"Square button with an arrow shape. 'dir' is one of the Dir_* values")
{
  FRAME_GUARD;
  return ImGui::ArrowButton(str_id, dir);
}

DEFINE_API(bool, Checkbox, (ImGui_Context*,ctx)
(const char*,label)(bool*,API_RW(v)),
"")
{
  FRAME_GUARD;
  if(!API_RW(v))
    return false;
  return ImGui::Checkbox(label, API_RW(v));
}

DEFINE_API(bool, CheckboxFlags, (ImGui_Context*,ctx)
(const char*,label)(int*,API_RW(flags))(int,flags_value),
"")
{
  FRAME_GUARD;
  return ImGui::CheckboxFlags(label, API_RW(flags), flags_value);
}

DEFINE_API(bool, RadioButton, (ImGui_Context*,ctx)
(const char*,label)(bool,active),
R"(Use with e.g. if (RadioButton("one", my_value==1)) { my_value = 1; })")
{
  FRAME_GUARD;
  return ImGui::RadioButton(label, active);
}

DEFINE_API(bool, RadioButtonEx, (ImGui_Context*,ctx)
(const char*,label)(int*,API_RW(v))(int,v_button),
"Shortcut to handle RadioButton's example pattern when value is an integer")
{
  FRAME_GUARD;
  return ImGui::RadioButton(label, API_RW(v), v_button);
}

DEFINE_API(void, PushButtonRepeat, (ImGui_Context*,ctx)
(bool,repeat),
R"(In 'repeat' mode, Button*() functions return repeated true in a typematic
manner (using ConfigVar_KeyRepeatDelay/ConfigVar_KeyRepeatRate settings).

Note that you can call IsItemActive after any Button to tell if the button is
held in the current frame.)")
{
  FRAME_GUARD;
  ImGui::PushButtonRepeat(repeat);
}

DEFINE_API(void, PopButtonRepeat, (ImGui_Context*,ctx),
"See PushButtonRepeat")
{
  FRAME_GUARD;
  ImGui::PopButtonRepeat();
}

API_SUBSECTION("Flags", "For InvisibleButton.");
DEFINE_ENUM(ImGui, ButtonFlags_None, "");
DEFINE_ENUM(ImGui, ButtonFlags_MouseButtonLeft,
  "React on left mouse button (default).");
DEFINE_ENUM(ImGui, ButtonFlags_MouseButtonRight,
  "React on right mouse button.");
DEFINE_ENUM(ImGui, ButtonFlags_MouseButtonMiddle,
  "React on center mouse button.");

API_SUBSECTION("Cardinal directions", "For ArrowButton.");
DEFINE_ENUM(ImGui, Dir_None,  "");
DEFINE_ENUM(ImGui, Dir_Left,  "");
DEFINE_ENUM(ImGui, Dir_Right, "");
DEFINE_ENUM(ImGui, Dir_Up,    "");
DEFINE_ENUM(ImGui, Dir_Down,  "");
