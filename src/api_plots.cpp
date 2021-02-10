#include "api_helper.hpp"

static float getArrayValue(void *data, const int index)
{
  const double value { static_cast<double *>(data)[index] };
  return static_cast<float>(value);
}

// Widgets: Data Plotting
DEFINE_API(void, PlotLines, ((ImGui_Context*,ctx))
((const char*,label))((reaper_array*,values))((int*,valuesOffsetInOptional))
((const char*,overlayTextInOptional))
((double*,scaleMinInOptional))((double*,scaleMaxInOptional))
((double*,graphWidthInOptional))((double*,graphHeightInOptional)),
"",
{
  ENTER_CONTEXT(ctx);
  nullIfEmpty(overlayTextInOptional);

  ImGui::PlotLines(label, &getArrayValue, values->data, values->size,
    valueOr(valuesOffsetInOptional, 0), overlayTextInOptional,
    valueOr(scaleMinInOptional, FLT_MAX), valueOr(scaleMaxInOptional, FLT_MAX),
    ImVec2(valueOr(graphWidthInOptional, 0.0), valueOr(graphHeightInOptional, 0.0)));
});

DEFINE_API(void, PlotHistogram, ((ImGui_Context*,ctx))
((const char*,label))((reaper_array*,values))((int*,valuesOffsetInOptional))
((const char*,overlayTextInOptional))
((double*,scaleMinInOptional))((double*,scaleMaxInOptional))
((double*,graphWidthInOptional))((double*,graphHeightInOptional)),
"",
{
  ENTER_CONTEXT(ctx);

  ImGui::PlotHistogram(label, &getArrayValue, values->data, values->size,
    valueOr(valuesOffsetInOptional, 0), overlayTextInOptional,
    valueOr(scaleMinInOptional, FLT_MAX), valueOr(scaleMaxInOptional, FLT_MAX),
    ImVec2(valueOr(graphWidthInOptional, 0.0), valueOr(graphHeightInOptional, 0.0)));
});