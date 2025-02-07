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

#ifndef REAIMGUI_VIEWPORT_HPP
#define REAIMGUI_VIEWPORT_HPP

#include "../src/resource_proxy.hpp"

#include "../src/context.hpp"

struct ViewportProxy
    : public ResourceProxy<ViewportProxy, Context, ImGuiViewport> {
  static constexpr const char *api_type_name { "ImGui_Viewport" };

  template<Key KeyValue, auto GetterFunc>
  struct Getter {
    static constexpr Key key { KeyValue };
    static auto get(Context *ctx)
    {
      ctx->setCurrent();
      return GetterFunc();
    }
  };

  using Main   = Getter<'MNVP', ImGui::GetMainViewport>;
  using Window = Getter<'WNVP', ImGui::GetWindowViewport>;

  using Decoder = MakeDecoder<Main, Window>;
};

using ImGui_Viewport = ViewportProxy;

#endif
