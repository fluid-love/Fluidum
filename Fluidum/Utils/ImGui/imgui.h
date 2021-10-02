#pragma once

#include <imgui.h>

namespace Fluidum::Utils::ImGui {

	void helpTooltip(const char* desc);

	void exclamationTooltip(const char* desc);

	void exclamationFadeTooltip(const ImVec2& pos, const char* desc);

	void hoveredMarker(const char* desc);



	//https://github.com/ocornut/imgui/issues/1901
	bool spinner(const char* label, float radius, float thickness, const ImU32& color);

}
