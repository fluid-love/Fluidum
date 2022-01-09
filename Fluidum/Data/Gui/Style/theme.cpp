#include "theme.h"

namespace FD::Style {

	enum class Theme : UIF8 {
		Error,
		Default,
		Dark,
		Light
	};

	[[nodiscard]] Theme loadType() noexcept {
		try {
			std::ifstream ifs(Internal::Resource::GuiStyleFilePath, std::ios::in);

			std::string data{};
			std::getline(ifs, data);

			if (data == "Default")
				return Theme::Default;
			else if (data == "Dark")
				return Theme::Dark;
			else if (data == "Light")
				return Theme::Light;
		}
		catch (...) {
			Internal::GMessenger.add<FU::Log::Type::Warning>(__FILE__, __LINE__, "Failed to load style file.");
		}
		return Theme::Error;
	}

}

void FD::Style::ThemeWrite::setTheme() noexcept {

	const Theme theme = loadType();

	if (theme == Theme::Error) {
		Internal::GMessenger.add<FU::Log::Type::Warning>(__FILE__, __LINE__, "Failed to load style file.");
	}
	else if (theme == Theme::Default) {
		this->setDefault();
	}
	else {
		Internal::GMessenger.add<FU::Log::Type::Warning>(__FILE__, __LINE__, "Internal Error.");
	}

}

void FD::Style::ThemeWrite::setDefault() noexcept {

	auto& style = ImGui::GetStyle();
	auto& colors = style.Colors;

	colors[ImGuiCol_Text] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.200f, 0.200f, 0.200f, 0.800f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.016f, 0.016f, 0.016f, 1.000f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.180f, 0.180f, 0.180f, 0.000f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.02f, 0.02f, 0.02f, 1.000f);
	colors[ImGuiCol_Border] = ImVec4(0.26f, 0.200f, 0.200f, 1.000f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.060f, 0.060f, 0.060f, 1.000f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.100f, 0.100f, 0.100f, 1.000f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.000f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.07f, 0.07f, 0.07f, 1.000f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.277f, 0.277f, 0.277f, 1.000f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.300f, 0.300f, 0.300f, 1.000f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_Button] = ImVec4(1.000f, 1.000f, 1.000f, 0.000f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
	colors[ImGuiCol_ButtonActive] = ImVec4(1.000f, 1.000f, 1.000f, 0.391f);
	colors[ImGuiCol_Header] = ImVec4(0.113f, 0.113f, 0.113f, 1.000f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.1f, 0.1f, 0.1f, 1.000f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(1.000f, 1.000f, 1.000f, 0.250f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.670f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_Tab] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.352f, 0.352f, 0.352f, 1.000f);
	colors[ImGuiCol_TabActive] = ImVec4(0.165f, 0.165f, 0.165f, 1.000f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
	colors[ImGuiCol_DockingPreview] = ImVec4(1.000f, 0.391f, 0.000f, 0.781f);
	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.586f, 0.586f, 0.586f, 1.000f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);

	style.ChildRounding = 4.0f;
	style.FrameBorderSize = 1.0f;
	style.FrameRounding = 2.0f;
	style.GrabMinSize = 7.0f;
	style.PopupRounding = 2.0f;
	style.ScrollbarRounding = 12.0f;
	style.ScrollbarSize = 13.0f;
	style.TabBorderSize = 1.0f;
	style.TabRounding = 0.0f;
	style.WindowRounding = 0.0f;

}