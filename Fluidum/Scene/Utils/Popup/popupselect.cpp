#include "popupselect.h"

using namespace FU::ImGui::Operators;

FS::PopupSelect::~PopupSelect() noexcept {
	try {
		Internal::GLog.add<FD::Log::Type::None>("Destruct PopupSelectScene.");
	}
	catch (const std::exception& e) {
		try {
			std::cerr << e.what() << std::endl;
			abort();
		}
		catch (...) {
			abort();
		}
	}
	catch (...) {
		abort();
	}
}

void FS::PopupSelect::call() {

	ImGui::SetNextWindowPos(style.windowPos);
	ImGui::SetNextWindowSize(style.windowSize);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImGui::GetStyle().WindowPadding * 2.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);

	constexpr auto flag =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse;


	ImGui::OpenPopup("PopupSelect");
	if (ImGui::BeginPopup("PopupSelect", flag)) {
		ImGui::BeginChild("PopupSelect1", { style.windowSize.x,style.windowSize.y * 0.7f });

		this->messageGui();

		ImGui::EndChild();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(ImGui::GetStyle().Colors[ImGuiCol_ChildBg] * 0.5f));
		ImGui::BeginChild("PopupSelect2", { style.windowSize.x,style.windowSize.y * 0.2f });
		ImGui::PopStyleColor();

		this->button();

		ImGui::EndChild();
		ImGui::EndPopup();
	}

	ImGui::PopStyleVar(3);
}

void FS::PopupSelect::messageGui() {
	ImGui::SetWindowFontScale(1.15f);
	const float size = style.windowSize.x * 0.08f;
	if (image) {
		ImGui::Image(image.value(), { size,size });
	}
	ImGui::Dummy({ size,0.0f });
	ImGui::SameLine();
	ImGui::Text(message.c_str());

}

void FS::PopupSelect::button() {

	for (auto& x : buttonLabels) {
		if (ImGui::Button(x, style.buttonSize)) {
			ImGui::CloseCurrentPopup();
			Internal::GLog.add<FD::Log::Type::None>("Pressed button. Label is {}.", x);

			Internal::GLog.add<FD::Log::Type::None>("Request delete PopupSelectScene.");
			Scene::deleteScene<PopupSelect>();
		}
		ImGui::SameLine();
	}

}

ImVec2 FS::PopupSelect::calcButtonSize(const uint16_t size) {
	float width;
	float fSize = static_cast<float>(size);

	width = style.windowSize.x / fSize;

	width -= (ImGui::GetStyle().WindowPadding.x * 2.0f);

	///width -= (ImGui::GetStyle().FramePadding.x / fSize);

	return { width,0.0f };
}

std::optional<FDR::ImGuiImage> FS::PopupSelect::createImage(const Utils::PopupSelectIconType type) {
	constexpr const char* fileNames[] = {
		"info.png",
		"warning.png"
	};

	std::string path = Internal::Resource::UtilsIconFolderPath;

	using enum Utils::PopupSelectIconType;

	if (type == Warning) {
		path += fileNames[0];
		return FDR::createImGuiImage(path.c_str());
	}
	else if (type == Warning) {
		path += fileNames[1];
		return FDR::createImGuiImage(path.c_str());
	}

	return std::nullopt;
}

void FS::PopupSelect::init() {
	style.windowPos = guiRead->centerPos() - (guiRead->windowSize() / 8.0f);
	style.windowSize = guiRead->windowSize() - (style.windowPos * 2.0f);
}
