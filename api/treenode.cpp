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

API_SECTION("Tree Node");

DEFINE_API(bool, TreeNode, (ImGui_Context*,ctx)
(const char*,label)(int*,API_RO(flags),ImGuiTreeNodeFlags_None),
R"(TreeNode functions return true when the node is open, in which case you need
to also call TreePop when you are finished displaying the tree node contents.)")
{
  FRAME_GUARD;
  return ImGui::TreeNodeEx(label, API_RO_GET(flags));
}

DEFINE_API(bool, TreeNodeEx, (ImGui_Context*,ctx)
(const char*,str_id)(const char*,label)
(int*,API_RO(flags),ImGuiTreeNodeFlags_None),
R"(Helper variation to easily decorelate the id from the displayed string.
Read the [FAQ](https://dearimgui.org/faq) about why and how to use ID.
To align arbitrary text at the same level as a TreeNode you can use Bullet.)")
{
  FRAME_GUARD;
  return ImGui::TreeNodeEx(str_id, API_RO_GET(flags), "%s", label);
}

DEFINE_API(void, TreePush, (ImGui_Context*,ctx)
(const char*,str_id),
R"(Indent()+PushID(). Already called by TreeNode when returning true,
but you can call TreePush/TreePop yourself if desired.)")
{
  FRAME_GUARD;
  ImGui::TreePush(str_id);
}

DEFINE_API(void, TreePop, (ImGui_Context*,ctx),
"Unindent()+PopID()")
{
  FRAME_GUARD;
  ImGui::TreePop();
}

DEFINE_API(double, GetTreeNodeToLabelSpacing, (ImGui_Context*,ctx),
R"(Horizontal distance preceding label when using TreeNode*() or Bullet()
== (GetFontSize + StyleVar_FramePadding.x*2) for a regular unframed TreeNode.)")
{
  FRAME_GUARD;
  return ImGui::GetTreeNodeToLabelSpacing();
}

DEFINE_API(bool, CollapsingHeader, (ImGui_Context*,ctx)
(const char*,label)(bool*,API_RW(p_visible))
(int*,API_RO(flags),ImGuiTreeNodeFlags_None),
R"(Returns true when opened but do not indent nor push into the ID stack
(because of the TreeNodeFlags_NoTreePushOnOpen flag).

This is basically the same as calling TreeNode(label, TreeNodeFlags_CollapsingHeader).
You can remove the _NoTreePushOnOpen flag if you want behavior closer to normal
TreeNode.

When 'visible' is provided: if 'true' display an additional small close button
on upper right of the header which will set the bool to false when clicked,
if 'false' don't display the header.)")
{
  FRAME_GUARD;
  // p_visible behavior differs from ImGui: false as input is treated the same
  // as NULL. This is because EEL doesn't have a NULL (0 = false), API_W never
  // receives a NULL, and API_RWO aren't listed in the output values list.
  return ImGui::CollapsingHeader(label,
    openPtrBehavior(API_RW(p_visible)), API_RO_GET(flags));
}

DEFINE_API(void, SetNextItemOpen, (ImGui_Context*,ctx)
(bool,is_open)(int*,API_RO(cond),ImGuiCond_Always),
R"(Set next TreeNode/CollapsingHeader open state.
Can also be done with the TreeNodeFlags_DefaultOpen flag.)")
{
  FRAME_GUARD;
  ImGui::SetNextItemOpen(is_open, API_RO_GET(cond));
}

DEFINE_API(bool, IsItemToggledOpen, (ImGui_Context*,ctx),
"Was the last item open state toggled? Set by TreeNode.")
{
  FRAME_GUARD;
  return ImGui::IsItemToggledOpen();
}

API_SUBSECTION("Flags", "For TreeNode, TreeNodeEx and CollapsingHeader.");

DEFINE_ENUM(ImGui, TreeNodeFlags_None,     "");
DEFINE_ENUM(ImGui, TreeNodeFlags_Selected, "Draw as selected.");
DEFINE_ENUM(ImGui, TreeNodeFlags_Framed,
  "Draw frame with background (e.g. for CollapsingHeader).");
DEFINE_ENUM(ImGui, TreeNodeFlags_AllowItemOverlap,
  "Hit testing to allow subsequent widgets to overlap this one.");
DEFINE_ENUM(ImGui, TreeNodeFlags_NoTreePushOnOpen,
R"(Don't do a TreePush when open (e.g. for CollapsingHeader)
   = no extra indent nor pushing on ID stack.)");
DEFINE_ENUM(ImGui, TreeNodeFlags_NoAutoOpenOnLog,
R"(Don't automatically and temporarily open node when Logging is active
   (by default logging will automatically open tree nodes).)");
DEFINE_ENUM(ImGui, TreeNodeFlags_DefaultOpen,       "Default node to be open.");
DEFINE_ENUM(ImGui, TreeNodeFlags_OpenOnDoubleClick, "Need double-click to open node.");
DEFINE_ENUM(ImGui, TreeNodeFlags_OpenOnArrow,
R"(Only open when clicking on the arrow part.
   If TreeNodeFlags_OpenOnDoubleClick is also set, single-click arrow or
   double-click all box to open.)");
DEFINE_ENUM(ImGui, TreeNodeFlags_Leaf,
  "No collapsing, no arrow (use as a convenience for leaf nodes).");
DEFINE_ENUM(ImGui, TreeNodeFlags_Bullet,
  "Display a bullet instead of arrow.");
DEFINE_ENUM(ImGui, TreeNodeFlags_FramePadding,
R"(Use FramePadding (even for an unframed text node) to vertically align text
   baseline to regular widget height.
   Equivalent to calling AlignTextToFramePadding.)");
DEFINE_ENUM(ImGui, TreeNodeFlags_SpanAvailWidth,
R"(Extend hit box to the right-most edge, even if not framed.
   This is not the default in order to allow adding other items on the same line.
   In the future we may refactor the hit system to be front-to-back,
   allowing natural overlaps and then this can become the default.)");
DEFINE_ENUM(ImGui, TreeNodeFlags_SpanFullWidth,
  "Extend hit box to the left-most and right-most edges (bypass the indented area).");
// DEFINE_ENUM(ImGui, TreeNodeFlags_NavLeftJumpsBackHere,
// R"((WIP) Nav: left direction may move to this TreeNode() from any of its child
//    (items submitted between TreeNode and TreePop).)");
DEFINE_ENUM(ImGui, TreeNodeFlags_CollapsingHeader,
  "TreeNodeFlags_Framed | TreeNodeFlags_NoTreePushOnOpen | TreeNodeFlags_NoAutoOpenOnLog");
