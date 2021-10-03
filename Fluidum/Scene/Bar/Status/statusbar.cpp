#include "statusbar.h"

FS::StatusBar::StatusBar(const FD::GuiRead* const guiRead)
	: guiRead(guiRead)
{
	Internal::GLog.add<FD::Log::Type::None>("Construct StatusBarScene.");

	//style 初期化
	style.taskIcon = ImGui::CalcTextSize(text.task).x;
	style.version = ImGui::CalcTextSize(text.version).x;

	//背景
	style.windowPos = { 0.0f, guiRead->windowSize().y - style.barHeight };

	//中央
	style.infoWindowPos = { guiRead->windowSize().x / 2.0f, style.windowPos.y };
	style.infoWindowSize = { guiRead->windowSize().x / 3.0f,style.barHeight };

	//右下
	style.versionWindowPos = { guiRead->windowSize().x - (1.5f * style.version), style.windowPos.y };
	style.versionWindowSize = { 1.5f * style.version,style.barHeight };
}

FS::StatusBar::~StatusBar() noexcept {
	try {
		Internal::GLog.add<FD::Log::Type::None>("Destruct StatusBarScene.");
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
	//背景の色を暗くする
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.01f, 0.01f, 0.01f, 1.0f));

	//文字の上の余白が大きすぎるので小さく
	//auto currentPadding = ImGui::GetStyle().WindowPadding;
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(currentPadding.x, currentPadding.y));

	//角を取る
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

	//ボーダーを消す
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);


	ImGui::SetNextWindowPos(style.windowPos);
	ImGui::SetNextWindowSize(ImVec2(guiRead->windowSize().x, style.barHeight));


	ImGui::Begin("StatusBar", nullptr, Internal::Bar::commonWindowFlag | ImGuiWindowFlags_NoBringToFrontOnFocus);//アンダーバーの背景



	//左下に配置するのでここに書く
	this->taskGui();

	ImGui::End();


	//中身
	this->infoGui();
	this->versionGui();


	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);//rounding padding
}

void FS::StatusBar::taskGui() {
	ImGui::Text(text.task); ImGui::SameLine();
	ImGui::TextColored(ImVec4(0.87f, 0.87f, 0.87f, 0.9f), text.taskInfo);//少し薄く
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

