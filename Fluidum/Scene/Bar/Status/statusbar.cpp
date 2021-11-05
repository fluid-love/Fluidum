#include "statusbar.h"

FS::StatusBar::StatusBar(
	FD::GuiWrite* const guiWrite,
	const FD::GuiRead* const guiRead,
	const FD::TaskRead* const taskRead
) : guiRead(guiRead), taskRead(taskRead)
{
	GLog.add<FD::Log::Type::None>("Construct StatusBarScene.");

	//style ‰Šú‰»
	style.taskIcon = ImGui::CalcTextSize(text.task).x;
	style.version = ImGui::CalcTextSize(text.version).x;

	//”wŒi
	style.windowPos = { 0.0f, guiRead->windowSize().y - style.barHeight };

	//’†‰›
	style.infoWindowPos = { guiRead->windowSize().x / 2.0f, style.windowPos.y };
	style.infoWindowSize = { guiRead->windowSize().x / 3.0f,style.barHeight };

	//‰E‰º
	style.versionWindowPos = { guiRead->windowSize().x - (1.5f * style.version), style.windowPos.y };
	style.versionWindowSize = { 1.5f * style.version,style.barHeight };

	guiWrite->statusBarHeight(style.barHeight);
}

FS::StatusBar::~StatusBar() noexcept {
	try {
		GLog.add<FD::Log::Type::None>("Destruct StatusBarScene.");
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

namespace FS::Internal::Bar {
	constexpr ImGuiWindowFlags commonWindowFlag =
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
	//”wŒi‚ÌF‚ðˆÃ‚­‚·‚é
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.01f, 0.01f, 0.01f, 1.0f));

	//•¶Žš‚Ìã‚Ì—]”’‚ª‘å‚«‚·‚¬‚é‚Ì‚Å¬‚³‚­
	//auto currentPadding = ImGui::GetStyle().WindowPadding;
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(currentPadding.x, currentPadding.y));

	//Šp‚ðŽæ‚é
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

	//ƒ{[ƒ_[‚ðÁ‚·
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);


	ImGui::SetNextWindowPos(style.windowPos);
	ImGui::SetNextWindowSize(ImVec2(guiRead->windowSize().x, style.barHeight));


	ImGui::Begin("StatusBar", nullptr, Internal::Bar::commonWindowFlag | ImGuiWindowFlags_NoBringToFrontOnFocus);//ƒAƒ“ƒ_[ƒo[‚Ì”wŒi



	//¶‰º‚É”z’u‚·‚é‚Ì‚Å‚±‚±‚É‘‚­
	this->taskGui();

	ImGui::End();


	//’†g
	this->infoGui();
	this->versionGui();


	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);//rounding padding
}

void FS::StatusBar::taskGui() {
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

void FS::StatusBar::infoGui() {
	ImGui::SetNextWindowPos(style.infoWindowPos);
	ImGui::SetNextWindowSize(style.infoWindowSize);

	ImGui::Begin("Info", nullptr, Internal::Bar::commonWindowFlag);

	//fps
	ImGui::Text(text.fps); ImGui::SameLine();
	ImGui::Text("%.1f", ImGui::GetIO().Framerate);

	ImGui::End();
}

void FS::StatusBar::versionGui() {
	ImGui::SetNextWindowPos(style.versionWindowPos);
	ImGui::SetNextWindowSize(style.versionWindowSize);

	ImGui::Begin("Version", nullptr, Internal::Bar::commonWindowFlag);

	ImGui::Text(text.version);

	ImGui::End();
}

