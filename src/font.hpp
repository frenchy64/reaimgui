/* ReaImGui: ReaScript binding for Dear ImGui
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

#ifndef REAIMGUI_FONT_HPP
#define REAIMGUI_FONT_HPP

#include <string>
#include <vector>

#include "resource.hpp"
#include "variant.hpp"

enum FontFlags {
  ReaImGuiFontFlags_None      = 0,
  ReaImGuiFontFlags_IndexMask = 0xFF, // font index when loading from a collection file
  ReaImGuiFontFlags_Bold      = 1<<8,
  ReaImGuiFontFlags_Italic    = 1<<9,
  ReaImGuiFontFlags_StyleMask = ~0xFF,
};

struct ImFont;

class Font : public Resource {
public:
  static constexpr const char *api_type_name { "ImGui_Font" };

  // generic fonts
  static constexpr const char
    *CURSIVE    { "cursive" },
    *FANTASY    { "fantasy" },
    *MONOSPACE  { "monospace" },
    *SANS_SERIF { "sans-serif" },
    *SERIF      { "serif" };

  Font(const char *family, int size, int style);
  ImFont *load();

private:
  bool resolve(const char *family, int style);

  std::variant<std::string, std::vector<unsigned char>> m_data;
  int m_index, m_size, m_missingStyles;
};

class FontList {
public:
  FontList();
  void add(Font *);
  bool isLoaded() const { return m_loaded; }
  void invalidate() { m_loaded = false; }
  void keepAliveAll();
  void loadAll();

  Font *get(ImFont *) const;
  ImFont *instanceOf(Font *) const;

private:
  bool m_loaded;

  struct FontAttachment { Font *descriptor; ImFont *instance; };
  std::vector<FontAttachment> m_fonts;
};

#endif