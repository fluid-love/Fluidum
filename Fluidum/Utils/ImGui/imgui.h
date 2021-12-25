#pragma once

#include <imgui.h>
#include <string>

namespace FU::ImGui {

	template<typename ImCounter>//ImGuiAnimation ImCounter<ImAnime>
	void tooltip(ImCounter& counter, const char* desc);

	ImVec2 messagePos();
}

namespace FU::ImGui {
	consteval ImU32 convertImVec4ToImU32(const float x, const float y, const float z, const float w) {
		auto saturate = [](const float f) { return (f < 0.0f) ? 0.0f : (f > 1.0f) ? 1.0f : f; };

		ImU32 out;
		out = static_cast<ImU32>(saturate(x) * 255.0f + 0.5f) << IM_COL32_R_SHIFT;
		out |= static_cast<ImU32>(saturate(y) * 255.0f + 0.5f) << IM_COL32_G_SHIFT;
		out |= static_cast<ImU32>(saturate(z) * 255.0f + 0.5f) << IM_COL32_B_SHIFT;
		out |= static_cast<ImU32>(saturate(w) * 255.0f + 0.5f) << IM_COL32_A_SHIFT;

		return out;
	}

	void cursor_busy();

	//https://github.com/ocornut/imgui/issues/1901
	bool spinner(const char* label, float radius, float thickness, const ImU32& color);

	//compiler bug
	//issue https://github.com/fluid-love/Fluidum/issues/1
	/*template<ImGuiCol... Colors>
	requires((Colors < ImGuiCol_COUNT) && ...)
		int32_t pushStyleColor(const ImVec4& col) {
		for (ImGuiCol x : { Colors... })
			::ImGui::PushStyleColor(x, col);
		return sizeof...(Colors);
	}*/
}
