#include "var.h"

namespace FD::Style {
	struct {
		float iconFontWidth{};
		float popupWindowBorderSize = 0.05f;
	}GData;
}

void FD::Style::VarWrite::initialize() {
	//All icons are the same width.
	GData.iconFontWidth = ImGui::CalcTextSize("\xee\xa1\x8d").x;
}

float FD::Style::VarRead::iconDummyWidth() const noexcept {
	return GData.iconFontWidth;
}

void FD::Style::VarRead::iconDummy() const {
	ImGui::Dummy({ iconDummyWidth(), 0.0f });
	ImGui::SameLine();
}

float FD::Style::VarRead::popupWindowBorderSize() const noexcept {
	return GData.popupWindowBorderSize;
}

float FD::Style::VarRead::inputTextHeight() const noexcept {
	return ImGui::GetFontSize() * 5.0f;
}

ImVec2 FD::Style::VarRead::viewWindowSizeConstraints() const noexcept {
	return { 150.0f, 150.0f };
}





















