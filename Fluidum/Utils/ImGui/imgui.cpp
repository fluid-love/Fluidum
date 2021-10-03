#include "imgui.h"

#include <imgui_internal.h>
#include <cmath>

void FU::ImGui::helpTooltip(const char* desc) {
    ::ImGui::TextDisabled("(?)");
    if (::ImGui::IsItemHovered()) {
        ::ImGui::BeginTooltip();
        ::ImGui::PushTextWrapPos(::ImGui::GetFontSize() * 35.0f);
        ::ImGui::TextUnformatted(desc);
        ::ImGui::PopTextWrapPos();
        ::ImGui::EndTooltip();
    }
}

void FU::ImGui::exclamationTooltip(const char* desc) {
    ::ImGui::TextDisabled("(!)");
    if (::ImGui::IsItemHovered()) {
        ::ImGui::BeginTooltip();
        ::ImGui::PushTextWrapPos(::ImGui::GetFontSize() * 35.0f);
        ::ImGui::TextUnformatted(desc);
        ::ImGui::PopTextWrapPos();
        ::ImGui::EndTooltip();
    }
}

void FU::ImGui::exclamationFadeTooltip(const ImVec2& pos, const char* desc) {

}

void FU::ImGui::hoveredMarker(const char* desc) {
    if (::ImGui::IsItemHovered()) {
        ::ImGui::BeginTooltip();
        ::ImGui::PushTextWrapPos(::ImGui::GetFontSize() * 35.0f);
        ::ImGui::TextUnformatted(desc);
        ::ImGui::PopTextWrapPos();
        ::ImGui::EndTooltip();
    }
}



/***************************************************************************************************************************/
/*
* https://github.com/ocornut/imgui/issues/1901

The MIT License (MIT)

Copyright (c) 2018 zfedoran

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
bool FU::ImGui::spinner(const char* label, float radius, float thickness, const ImU32& color) {
    using namespace ::ImGui;

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size((radius) * 2, (radius + style.FramePadding.y) * 2);

    const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    ItemSize(bb, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    // Render
    window->DrawList->PathClear();

    int num_segments = 30;
    float start = std::abs(ImSin(static_cast<float>(g.Time) * 1.8f) * (num_segments - 5));

    const float a_min = IM_PI * 2.0f * ((float)start) / (float)num_segments;
    const float a_max = IM_PI * 2.0f * ((float)num_segments - 3) / (float)num_segments;

    const ImVec2 centre = ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);

    for (int i = 0; i < num_segments; i++) {
        const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
        window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a + static_cast<float>(g.Time) * 8) * radius,
            centre.y + ImSin(a + static_cast<float>(g.Time) * 8) * radius));
    }

    window->DrawList->PathStroke(color, false, thickness);

    return true;
}
/***************************************************************************************************************************/
