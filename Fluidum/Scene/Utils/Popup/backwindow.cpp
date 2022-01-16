#include "backwindow.h"

FS::Utils::PopupBackWindow::PopupBackWindow(const FD::GuiRead* const guiRead) 
	: guiRead(guiRead)
{
	FluidumScene_Log_Constructor(::FS::Utils::PopupBackWindow);

	style.windowPos = { 0.0f,0.0f };
	style.windowSize = guiRead->windowSize();
}

FS::Utils::PopupBackWindow::~PopupBackWindow() noexcept {
	FluidumScene_Log_Destructor(::FS::Utils::PopupBackWindow);
}

void FS::Utils::PopupBackWindow::call() {

	ImGui::SetNextWindowPos(style.windowPos);
	ImGui::SetNextWindowSize(style.windowSize);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.07f, 0.07f, 0.07f, 0.08f));

	constexpr ImGuiWindowFlags windowFlags = FD::Style::VarRead::PopupWindowFlags();

	ImGui::Begin("BackWindow", nullptr, windowFlags);

	ImGui::End();


	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
}
