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

ImVec2 FD::Style::VarRead::popupWindowPos(const ImVec2& centerPos, const ImVec2& screenSize) const noexcept {
	using namespace FU::ImGui::Operators;
	return (centerPos - (screenSize / 3.0f));
}

ImVec2 FD::Style::VarRead::popupWindowSize(const ImVec2& windowPos, const ImVec2& screenSize) const noexcept {
	using namespace FU::ImGui::Operators;
	return (screenSize - (windowPos * 2.0f));
}

float FD::Style::VarRead::popupWindowBorderSize() const noexcept {
	return GData.popupWindowBorderSize;
}

float FD::Style::VarRead::popupWindowBottomButtonWidth(const float windowWidth) const noexcept {
	return windowWidth * 0.15f;
}

float FD::Style::VarRead::inputTextHeight() const noexcept {
	return ImGui::GetFontSize() * 5.0f;
}

ImVec2 FD::Style::VarRead::viewWindowSizeConstraints() const noexcept {
	return { 150.0f, 150.0f };
}

void FD::Style::VarRead::bulletDummy() const noexcept {
	const float width = ImGui::GetStyle().FramePadding.x + ImGui::GetFontSize() * 0.5f;
	ImGui::Dummy({ width + ImGui::GetStyle().FramePadding.x, 0.0f });
	ImGui::SameLine();
}





















