#include "api_helper.hpp"

DEFINE_API(bool, BeginTabBar, ((ImGui_Context*,ctx))
((const char*,str_id))((int*,flagsInOptional)),
R"(Create and append into a TabBar.

Default values: flags = ImGui_TabBarFlags_None)",
{
  ENTER_CONTEXT(ctx, false);
  return ImGui::BeginTabBar(str_id, valueOr(flagsInOptional, ImGuiTabBarFlags_None));
});

DEFINE_API(void, EndTabBar, ((ImGui_Context*,ctx)),
"Only call EndTabBar() if BeginTabBar() returns true!",
{
  ENTER_CONTEXT(ctx);
  ImGui::EndTabBar();
});

DEFINE_API(bool, BeginTabItem, ((ImGui_Context*,ctx))
((const char*,label))((bool*,openInOptional))((int*,flagsInOptional)),
R"(Create a Tab. Returns true if the Tab is selected.

Default values: flags = ImGui_TabItemFlags_None
'open' is read/write.)",
{
  ENTER_CONTEXT(ctx, false);
  return ImGui::BeginTabItem(label, openInOptional,
    valueOr(flagsInOptional, ImGuiTabItemFlags_None));
});

DEFINE_API(void, EndTabItem, ((ImGui_Context*,ctx)),
"Only call EndTabItem() if BeginTabItem() returns true!",
{
  ENTER_CONTEXT(ctx);
  ImGui::EndTabItem();
});

DEFINE_API(bool, TabItemButton, ((ImGui_Context*,ctx))
((const char*,label))((int*,flagsInOptional)),
R"(Create a Tab behaving like a button. return true when clicked. cannot be selected in the tab bar.

Default values: flags = ImGui_TabItemFlags_None)",
{
  ENTER_CONTEXT(ctx, false);
  return ImGui::TabItemButton(label,
    valueOr(flagsInOptional, ImGuiTabItemFlags_None));
});

// IMGUI_API void          SetTabItemClosed(const char* tab_or_docked_window_label);           // notify TabBar or Docking system of a closed tab/window ahead (useful to reduce visual flicker on reorderable tab bars). For tab-bar: call after BeginTabBar() and before Tab submissions. Otherwise call with a window name.
