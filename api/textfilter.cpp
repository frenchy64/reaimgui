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

#include "textfilter.hpp"

TextFilter::TextFilter(const char *filter)
  : m_filter { filter }
{
}

void TextFilter::set(const char *filter)
{
  keepAlive();
  m_filter = filter;
  m_filter.Build();
}

ImGuiTextFilter *TextFilter::operator->()
{
  keepAlive();
  return &m_filter;
}

API_SECTION("Text Filter",
R"(Helper to perform simple filtering on text strings.
In format "aaaaa[,bbbb][,ccccc]".

Filter usage:
- ""         display all lines
- "xxx"      display lines containing "xxx"
- "xxx,yyy"  display lines containing "xxx" or "yyy"
- "-xxx"     hide lines containing "xxx")");

DEFINE_API(ImGui_TextFilter*, CreateTextFilter,
(const char*,API_RO(default_filter),""),
"Valid while used every frame unless attached to a context (see Attach).")
{
  return new TextFilter { API_RO_GET(default_filter) };
}

DEFINE_API(void, TextFilter_Set, (ImGui_TextFilter*,filter)
(const char*,filter_text),
"")
{
  assertValid(filter);
  filter->set(filter_text);
}

DEFINE_API(const char*, TextFilter_Get, (ImGui_TextFilter*,filter),
"")
{
  assertValid(filter);
  return (*filter)->InputBuf;
}

DEFINE_API(bool, TextFilter_Draw, (ImGui_TextFilter*,filter)
(ImGui_Context*,ctx)(const char*,API_RO(label),"Filter (inc,-exc)")
(double*,API_RO(width),0.0),
"Helper calling InputText+TextFilter_Set")
{
  assertValid(filter);
  FRAME_GUARD;

  return (*filter)->Draw(API_RO_GET(label), API_RO_GET(width));
}

DEFINE_API(bool, TextFilter_PassFilter, (ImGui_TextFilter*,filter)
(const char*,text),
"")
{
  assertValid(filter);
  return (*filter)->PassFilter(text);
}

DEFINE_API(void, TextFilter_Clear, (ImGui_TextFilter*,filter),
"")
{
  assertValid(filter);
  return (*filter)->Clear();
}

DEFINE_API(bool, TextFilter_IsActive, (ImGui_TextFilter*,filter),
"")
{
  assertValid(filter);
  return (*filter)->IsActive();
}
