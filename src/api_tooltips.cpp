#include "api_helper.hpp"

// Tooltips
// - Tooltip are windows following the mouse. They do not take focus away.

DEFINE_API(void, BeginTooltip, ((ImGui_Context*,ctx)),
"begin/append a tooltip window. to create full-featured tooltip (with any kind of items).",
{
  ENTER_CONTEXT(ctx);
  ImGui::BeginTooltip();
});

DEFINE_API(void, EndTooltip, ((ImGui_Context*,ctx)),
"",
{
  ENTER_CONTEXT(ctx);
  ImGui::EndTooltip();
});

DEFINE_API(void, SetTooltip, ((ImGui_Context*,ctx))((const char*,text)),
"Set a text-only tooltip, typically use with ImGui_IsItemHovered(). override any previous call to SetTooltip().",
{
  ENTER_CONTEXT(ctx);
  ImGui::SetTooltip("%s", text);
});