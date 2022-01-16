#include "statusbar.h"

#include "../../Calc/Lua/lua.h"

FS::StatusBar::StatusBar(
	FD::GuiWrite* const guiWrite,
	const FD::GuiRead* const guiRead,
	const FD::Style::ColorRead* const colorRead,
	const FD::TaskRead* const taskRead,
	const FD::SceneRead* const sceneRead
) :
	guiRead(guiRead),
	colorRead(colorRead),
	taskRead(taskRead),
	sceneRead(sceneRead)
{
	FluidumScene_Log_Constructor(::FS::StatusBar);

	//style
	style.taskIcon = ImGui::CalcTextSize(text.task).x;
	style.version = ImGui::CalcTextSize(text.version).x;
	style.barHeight = ImGui::GetFontSize() * 1.7f;

	//back
	style.windowPos = { 0.0f, guiRead->windowSize().y - style.barHeight };

	//center
	style.infoWindowPos = { guiRead->windowSize().x / 2.0f, style.windowPos.y };
	style.infoWindowSize = { guiRead->windowSize().x / 3.0f,style.barHeight };

	//lower right
	style.versionWindowPos = { guiRead->windowSize().x - (1.5f * style.version), style.windowPos.y };
	style.versionWindowSize = { 1.5f * style.version,style.barHeight };

	guiWrite->statusBarHeight(style.barHeight);
}

FS::StatusBar::~StatusBar() noexcept {
	FluidumScene_Log_Destructor(::FS::StatusBar);
}

namespace FS::Internal::Bar {
	constexpr ImGuiWindowFlags CommonWindowFlag =
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_HorizontalScrollbar |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoScrollbar;
}

void FS::StatusBar::call() {
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.007f, 0.007f, 0.007f, 1.0f));

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { ImGui::GetStyle().WindowPadding.x,ImGui::GetStyle().WindowPadding.y / 2.0f });

	ImGui::SetNextWindowPos(style.windowPos);
	ImGui::SetNextWindowSize({ guiRead->windowSize().x, style.barHeight });


	ImGui::Begin("StatusBar", nullptr, Internal::Bar::CommonWindowFlag | ImGuiWindowFlags_NoBringToFrontOnFocus);//アンダーバーの背景

	this->taskGui();

	ImGui::End();

	ImGui::PopStyleVar();//padding

	this->infoGui();
	this->versionGui();

	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);//rounding bordersize
}

void FS::StatusBar::taskGui() {
	ImGui::BeginChild("##StatusBarTask", { ImGui::GetWindowWidth() * 0.2f,0.0f });

	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
	bool clicked = ImGui::Button(text.task); ImGui::SameLine();
	ImGui::PopStyleVar();

	if (clicked)
		ImGui::OpenPopup("TaskPopup");

	const bool called = taskRead->notice([&](const FD::Task::Info* info)
		{
			ImGui::Text(info->message.c_str());
		}
	);

	if (!called)
		ImGui::TextColored(ImVec4(0.85f, 0.85f, 0.85f, 0.9f), text.taskInfo);

	ImGui::EndChild();
	ImGui::SameLine();

	this->taskIcons();

	//if clicked
	this->taskPopup();
}

void FS::StatusBar::taskPopup() {
	if (!ImGui::BeginPopup("TaskPopup"))
		return;

	taskRead->for_each([&](FD::Task::Info* info)
		{
			ImGui::Text(info->message.c_str());
			ImGui::Separator();
		}
	);

	ImGui::EndPopup();
}

void FS::StatusBar::taskIcons() {
	const bool running = sceneRead->exist<Calc::Lua::Run>();
	if (running) {
		ImAnime::PushStyleVar(anime.running, 1.6f, 0.8f, 0.2f, ImAnimeType::SQUARE, ImGuiStyleVar_Alpha);
		ImGui::AlignTextToFramePadding();
		ImGui::TextColored(colorRead->info(), ICON_FA_RUNNING);
		ImAnime::PopStyleVar();
	}
	else {
		ImGui::AlignTextToFramePadding();
		ImGui::TextDisabled(ICON_FA_RUNNING);
	}

}

void FS::StatusBar::infoGui() {
	ImGui::SetNextWindowPos(style.infoWindowPos);
	ImGui::SetNextWindowSize(style.infoWindowSize);

	ImGui::Begin("Info", nullptr, Internal::Bar::CommonWindowFlag);

	//fps
	ImGui::Text(text.fps); ImGui::SameLine();
	ImGui::Text("%.1f", ImGui::GetIO().Framerate);

	ImGui::End();
}

void FS::StatusBar::versionGui() {
	ImGui::SetNextWindowPos(style.versionWindowPos);
	ImGui::SetNextWindowSize(style.versionWindowSize);

	ImGui::Begin("Version", nullptr, Internal::Bar::CommonWindowFlag);

	ImGui::Text(text.version);

	ImGui::End();
}


