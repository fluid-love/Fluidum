#include "var.h"

namespace FD::Style {
	struct {
		float iconFontWidth{};

	}GData;
}

void FD::Style::VarWrite::initialize() const {
	GData.iconFontWidth = ImGui::CalcTextSize("\xee\xa1\x8d").x;
}

float FD::Style::VarRead::iconDummyWidth() const {
	return GData.iconFontWidth;
}

void FD::Style::VarRead::iconDummy() const {
	ImGui::Dummy({ iconDummyWidth(),0.0f });
	ImGui::SameLine();
}























