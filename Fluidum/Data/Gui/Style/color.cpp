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
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, colDisabled);
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, colDisabled);
	ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f,1.0f,1.0f,0.3f });
}

void FD::Style::ColorRead::popSelectableDisabled() const noexcept {
	ImGui::PopStyleColor(4);
}

ImVec4 FD::Style::ColorRead::createButton() const noexcept {
	return { 0.02f, 0.35f, 0.02f, 1.0f };
}

ImVec4 FD::Style::ColorRead::cancelButton() const noexcept {
	return { 0.7f, 0.05f, 0.05f, 0.5f };
}

ImVec4 FD::Style::ColorRead::backButton() const noexcept {
	return ImGui::GetStyleColorVec4(ImGuiCol_Button);
}

ImVec4 FD::Style::ColorRead::saveButton() const noexcept {
	return { 0.05f, 0.05f, 0.65f, 0.5f };
}

ImU32 FD::Style::ColorRead::toolBarVerticalSeparator() const noexcept {
	constexpr ImU32 color = FU::ImGui::ConvertImVec4ToImU32(0.01f, 0.01f, 0.01f, 1.0f);
	return color;
}

ImVec4 FD::Style::ColorRead::noerror() const noexcept {
	return { 0.05f, 0.65f, 0.65f, 1.0f };
}

ImVec4 FD::Style::ColorRead::error() const noexcept {
	return { 1.00f, 0.30f, 0.30f, 1.0f };
}

ImVec4 FD::Style::ColorRead::warning() const noexcept{
	return { 0.95f, 0.95f, 0.25f, 1.0f };
}

ImVec4 FD::Style::ColorRead::info() const noexcept{
	return { 0.20f, 0.70f, 1.00f, 1.0f };
}












