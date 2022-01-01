#include "imgui.h"
#include "operator.h"

#include <imgui_internal.h>
#include <cmath>
#include <numbers>
#include <ImGuiAnimation/ImGuiAnimation.h>

template<typename T>
void FU::ImGui::tooltip(T& counter, const char* desc) {
	static void* address = nullptr;
	static std::chrono::system_clock::time_point timePoint = {};
	static ImVec2 pos = {};

	if (!::ImGui::IsMouseHoveringRect(::ImGui::GetItemRectMin(), ::ImGui::GetItemRectMax())) {
		if (address == &counter) {
			address = nullptr;
		}
		return;
	}

	if (address != &counter) {
		counter.reset();
		timePoint = std::chrono::system_clock::now();
		address = &counter;
		pos = { ::ImGui::GetMousePos().x , ::ImGui::GetItemRectMax().y };
	}


	if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - timePoint).count() < 730)
		return;

	ImAnime::PushStyleVar(counter, 0.07f, 0.0f, 1.0f, ImAnimeType::LINEAR, ImGuiStyleVar_Alpha);

	::ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
	::ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 4.0f);
	::ImGui::PushStyleColor(ImGuiCol_PopupBg, { 0.016f,0.016f,0.016f,1.0f });

	constexpr ImGuiWindowFlags flag =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_Tooltip |
		ImGuiWindowFlags_AlwaysAutoResize;

	::ImGui::SetNextWindowPos(pos);

	::ImGui::Begin("__Tooltip", nullptr, flag);
	::ImGui::Text(desc);
	::ImGui::End();

	ImAnime::PopStyleVar();
	::ImGui::PopStyleColor();
	::ImGui::PopStyleVar(2);

}
template void FU::ImGui::tooltip(ImCounter<ImAnimeTime>&, const char*);

ImVec2 FU::ImGui::messagePos() {
	return { ::ImGui::GetMousePos().x ,::ImGui::GetItemRectMax().y };
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

void FU::ImGui::cursor_busy() {

	static float val = 0.0f;
	val += 0.02f;
	if (val > std::numbers::pi)
		val = 0.0f;

	const float sin = std::abs(std::sinf(val));
	const float cos = std::abs(std::cosf(val));

	ImU32 col = ::ImGui::ColorConvertFloat4ToU32({ 0.1f, sin , cos, 1.0f });

	::ImGui::GetForegroundDrawList()->AddCircle(::ImGui::GetMousePos(), 8.0f, col, 0, 3.0f);
}