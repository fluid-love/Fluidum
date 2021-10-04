#include "backwindow.h"

FS::PopupBackWindow::PopupBackWindow(const FD::GuiRead* const guiRead)
	: guiRead(guiRead)
{
	GLog.add<FD::Log::Type::None>("Construct PopupBackWindowScene.");

	style.windowPos = { 0.0f,0.0f };
	style.windowSize = guiRead->windowSize();
}

FS::PopupBackWindow::~PopupBackWindow() noexcept {
	try {
		GLog.add<FD::Log::Type::None>("Destruct PopupBackWindowScene.");
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

void FS::PopupBackWindow::call() {

	ImGui::SetNextWindowPos(style.windowPos);
	ImGui::SetNextWindowSize(style.windowSize);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05f, 0.05f, 0.05f, 0.07f));

	constexpr auto flag =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar|
		ImGuiWindowFlags_NoSavedSettings
		;

	ImGui::Begin("BackWindow", nullptr, flag);


	ImGui::End();


	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
}
