#include "api_helper.hpp"

#include <algorithm>

enum class StyleVarType { Unknown, Float, ImVec2 };

static StyleVarType styleVarType(const ImGuiStyleVar var)
{
  constexpr ImGuiStyleVar floatVars[] {
    ImGuiStyleVar_Alpha,
    ImGuiStyleVar_ChildBorderSize,
    ImGuiStyleVar_ChildRounding,
    ImGuiStyleVar_FrameBorderSize,
    ImGuiStyleVar_FrameRounding,
    ImGuiStyleVar_GrabMinSize,
    ImGuiStyleVar_GrabRounding,
    ImGuiStyleVar_IndentSpacing,
    ImGuiStyleVar_PopupBorderSize,
    ImGuiStyleVar_PopupRounding,
    ImGuiStyleVar_ScrollbarRounding,
    ImGuiStyleVar_ScrollbarSize,
    ImGuiStyleVar_TabRounding,
    ImGuiStyleVar_WindowBorderSize,
    ImGuiStyleVar_WindowRounding,
  };

  constexpr ImGuiStyleVar vec2Vars[] {
    ImGuiStyleVar_ButtonTextAlign,
    ImGuiStyleVar_SelectableTextAlign,
    ImGuiStyleVar_CellPadding,
    ImGuiStyleVar_ItemSpacing,
    ImGuiStyleVar_ItemInnerSpacing,
    ImGuiStyleVar_FramePadding,
    ImGuiStyleVar_WindowPadding,
    ImGuiStyleVar_WindowMinSize,
    ImGuiStyleVar_WindowTitleAlign,
  };

  if(std::find(std::begin(floatVars), std::end(floatVars), var) != std::end(floatVars))
    return StyleVarType::Float;
  else if(std::find(std::begin(vec2Vars), std::end(vec2Vars), var) != std::end(vec2Vars))
    return StyleVarType::ImVec2;
  else
    return StyleVarType::Unknown;
}

// IMGUI_API ImFont*       GetFont();                                                      // get current font

DEFINE_API(double, GetFontSize, ((ImGui_Context*,ctx)),
"Get current font size (= height in pixels) of current font with current scale applied",
{
  ENTER_CONTEXT(ctx, 0.0);
  return ImGui::GetFontSize();
});

DEFINE_API(bool, PushStyleVar, ((ImGui_Context*,ctx))
((int,varIdx))((double,val1))((double*,val2InOptional)),
"See ImGui_StyleVar_* for possible values of 'varIdx'.",
{
  ENTER_CONTEXT(ctx, false);

  switch(styleVarType(varIdx)) {
  case StyleVarType::Unknown:
    return false;
  case StyleVarType::Float:
    ImGui::PushStyleVar(varIdx, val1);
    return true;
  case StyleVarType::ImVec2:
    if(!val2InOptional)
      return false;
    ImGui::PushStyleVar(varIdx, ImVec2(val1, *val2InOptional));
    return true;
  }

  return false;
});

DEFINE_API(void, PopStyleVar, ((ImGui_Context*,ctx))
((int*,countInOptional)),
R"(Reset a style variable.

Default values: count = 1)",
{
  ENTER_CONTEXT(ctx);

  ImGui::PopStyleVar(valueOr(countInOptional, 1));
});

DEFINE_API(bool, GetStyleVar, ((ImGui_Context*,ctx))
((int,varIdx))((double*,val1Out))((double*,val2Out)),
"",
{
  ENTER_CONTEXT(ctx, false); // TODO: don't start a frame

  // const ImGuiStyle &style { ImGui::GetStyle() };        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);

  switch(varIdx) {
  case ImGuiStyleVar_Alpha:
    return true;
  default:
    return false;
  }
});

// IMGUI_API ImVec2        GetFontTexUvWhitePixel();                                       // get UV coordinate for a while pixel, useful to draw custom shapes via the ImDrawList API
// IMGUI_API ImU32         GetColorU32(ImGuiCol idx, float alpha_mul = 1.0f);              // retrieve given style color with style alpha applied and optional extra alpha multiplier, packed as a 32-bit value suitable for ImDrawList
// IMGUI_API ImU32         GetColorU32(const ImVec4& col);                                 // retrieve given color with style alpha applied, packed as a 32-bit value suitable for ImDrawList
// IMGUI_API ImU32         GetColorU32(ImU32 col);                                         // retrieve given color with style alpha applied, packed as a 32-bit value suitable for ImDrawList
// IMGUI_API const ImVec4& GetStyleColorVec4(ImGuiCol idx);                                // retrieve style color as stored in ImGuiStyle structure. use to feed back into PushStyleColor(), otherwise use GetColorU32() to get style color with style alpha baked in.