#include "titlebar.h"
#include "../../Project/File/saveas.h"
#include "../Exit/exit.h"

using namespace FU::ImGui::Operators;

FS::TitleBar::TitleBar(
	const FD::GuiRead* const guiRead,
	FD::GuiWrite* const guiWrite,
	const FD::WindowRead* const windowRead,
	FD::WindowWrite* const windowWrite,
	const FD::SceneRead* const sceneRead,
	const FD::ProjectRead* const projectRead,
	FD::ExitWrite* const exitWrite,
	const FD::Coding::TabRead* const tabRead,
	FD::Coding::TabWrite* const tabWrite
) :
	guiRead(guiRead),
	guiWrite(guiWrite),
	windowWrite(windowWrite),
	windowRead(windowRead),
	sceneRead(sceneRead),
	projectRead(projectRead),
	exitWrite(exitWrite),
	tabRead(tabRead),
	tabWrite(tabWrite)
{
	FluidumScene_Log_Constructor(::FS::TitleBar);

	style.windowPos = ImVec2(guiRead->windowSize().x * 0.91f, 0.0f);
	const float windowHeight = guiRead->menuBarHeight();
	style.windowSize = ImVec2(guiRead->windowSize().x * 0.09f, windowHeight + 1.0f);

	//MenuBar size == ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2
	style.iconWindowPos = { 0.0f,windowHeight / 10.0f };
	style.iconWindowSize = { (ImGui::GetStyle().WindowPadding.x * 2.0f + (ImGui::GetStyle().FramePadding.x * 2.0f)) + ImGui::CalcTextSize("   ").x ,windowHeight };

	style.projectNameWindowPos = { guiRead->windowSize().x * 0.6f, 0.0f };
	style.projectNameWindowSize = { guiRead->windowSize().x * 0.3f ,windowHeight };

	style.iconSize = ImVec2{ windowHeight ,windowHeight } *0.86f;

	style.buttonSize = { style.windowSize.x / 3.0f, style.windowSize.y };
}

FS::TitleBar::~TitleBar() noexcept {
	FluidumScene_Log_Destructor(::FS::TitleBar);
}

void FS::TitleBar::call() {
	this->icon();
	this->bar();
	this->project();
}

void FS::TitleBar::icon() {
	constexpr auto flag =
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoBackground;

	ImGui::SetNextWindowPos(style.iconWindowPos);
	ImGui::SetNextWindowSize(style.iconWindowSize);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(ImGui::GetStyle().WindowPadding.x, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2());

	ImGui::Begin("BarIcon", nullptr, flag);
	ImGui::Image(this->iconImage, style.iconSize);
	ImGui::End();

	ImGui::PopStyleVar(2);
}

void FS::TitleBar::bar() {

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2());
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2());


	ImGui::SetNextWindowPos(style.windowPos);
	ImGui::SetNextWindowSize(style.windowSize);

	constexpr auto flag =
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoBackground;


	ImGui::Begin("TitleBar", nullptr, flag);

	ImGui::SetWindowFontScale(1.2f);

	const auto size = ImVec2(style.windowSize.x / 3.0f, style.windowSize.y);

	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4());

	bool minimize = ImGui::Button(ICON_MD_REMOVE, style.buttonSize); ImGui::SameLine();
	ImGui::Button(ICON_MD_LAYERS, style.buttonSize); ImGui::SameLine();

	//color: red
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.05f, 0.05f, 0.5f));
	bool close = ImGui::Button(ICON_MD_CLOSE, style.buttonSize);

	if (minimize) {
		GLog.add_str<FU::Log::Type::None>("[Request] MinimizeWindow.");
		FDR::minimizeWindow();
	}
	else if (close) {
		GLog.add_str<FU::Log::Type::None>("[Request] Terminate.");
		this->exit();
	}

	ImGui::PopStyleColor(2);

	ImGui::End();

	ImGui::PopStyleVar(3);
}

void FS::TitleBar::exit() {
	FluidumScene_Log_RequestAddScene(::FS::Bar::Exit);
	Scene::addScene<Bar::Exit>();
}

void FS::TitleBar::project() {
	ImGui::SetNextWindowPos(style.projectNameWindowPos);
	ImGui::SetNextWindowSize(style.projectNameWindowSize);

	constexpr ImGuiWindowFlags flag =
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoBackground;

	ImGui::Begin("##ProjectName", nullptr, flag);

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);

	if (projectRead->isDefaultProject())
		ImGui::Button(text.tempProject);
	else
		ImGui::Button(projectRead->projectName().c_str());

	ImGui::PopStyleColor(3);
	ImGui::PopStyleVar(2);

	ImGui::End();
}