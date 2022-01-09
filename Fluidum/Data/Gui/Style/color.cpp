#include "color.h"

void FD::Style::ColorRead::pushButtonDisabled() const noexcept {
	ImVec4 colDisabled = ImGui::GetStyle().Colors[ImGuiCol_Button];
	ImGui::PushStyleColor(ImGuiCol_Button, colDisabled);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colDisabled);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, colDisabled);
	ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f,1.0f,1.0f,0.3f });
}

void FD::Style::ColorRead::popButtonDisabled() const noexcept {
	ImGui::PopStyleColor(4);
}

void FD::Style::ColorRead::pushSelectableDisabled() const noexcept {
	ImVec4 colDisabled = ImGui::GetStyle().Colors[ImGuiCol_Button];
	ImGui::PushStyleColor(ImGuiCol_Header, colDisabled);
	//ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colDisabled);
	//ImGui::PushStyleColor(ImGuiCol_ButtonActive, colDisabled);
	//ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f,1.0f,1.0f,0.3f });
}

void FD::Style::ColorRead::popSelectableDisabled() const noexcept {
	ImGui::PopStyleColor();
}

ImU32 FD::Style::ColorRead::toolBarVerticalSeparator() const noexcept {
	constexpr ImU32 color = FU::ImGui::ConvertImVec4ToImU32(0.01f, 0.01f, 0.01f, 1.0f);
	return color;
}















