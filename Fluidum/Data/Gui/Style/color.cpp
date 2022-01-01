#include "color.h"

//future
//namespace FD {
//	Style::StyleType getType() {
//		std::ifstream ifs(Internal::Resource::GuiStyleFilePath, std::ios::in);
//
//		std::string data{};
//		std::getline(ifs, data);
//
//		if (data == "Default")
//			return Style::StyleType::Default;
//		else if (data == "Dark")
//			return Style::StyleType::Dark;
//		else if (data == "Light")
//			return Style::StyleType::Light;
//		else
//			throw std::runtime_error("Failed to read GuiStyleType.");
//
//		return{};
//	}
//}

void FD::Style::ColorRead::pushButtonDisabled() const {
	ImVec4 colDisabled = ImGui::GetStyle().Colors[ImGuiCol_Button];
	ImGui::PushStyleColor(ImGuiCol_Button, colDisabled);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colDisabled);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, colDisabled);
	ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f,1.0f,1.0f,0.3f });
}

void FD::Style::ColorRead::popButtonDisabled() const {
	ImGui::PopStyleColor(4);
}