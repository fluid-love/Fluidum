#include "topbar.h"

#include "../../Calc/Lua/lua.h"
#include "../../Calc/Run/run.h"

#include "../../Utils/Popup/message.h"
#include "../../Utils/Popup/internal_error.h"
#include "../../Utils/Scene/deleteAll.h"

#include "../../Utils/Scene/include.h"

#include <imgui_internal.h>

using namespace FU::ImGui::Operators;

FS::TopBar::TopBar(
	const FD::ProjectRead* const projectRead,
	const FD::FluidumFilesRead* const fluidumFilesRead,
	const FD::GuiRead* const guiRead,
	FD::GuiWrite* const guiWrite,
	const FD::SceneRead* const sceneRead,
	const FD::ToolBarRead* const toolBarRead,
	FD::ToolBarWrite* const toolBarWrite,
	const FD::ImGuiWindowRead* const imguiWindowRead,
	const FD::LayoutRead* const layoutRead
) :
	projectRead(projectRead),
	fluidumFilesRead(fluidumFilesRead),
	guiRead(guiRead),
	guiWrite(guiWrite),
	sceneRead(sceneRead),
	toolBarRead(toolBarRead),
	toolBarWrite(toolBarWrite),
	imguiWindowRead(imguiWindowRead),
	layoutRead(layoutRead)
{
	FluidumScene_Log_Constructor(::FS::TopBar);

	style.windowPos = ImVec2(0.0f, guiRead->menuBarHeight());
	const float windowHeight = guiRead->menuBarHeight() + (ImGui::GetFontSize() * 0.35f);
	style.windowSize = ImVec2(guiRead->windowSize().x, windowHeight);

	if (windowHeight <= ImGui::GetStyle().WindowMinSize.y)
		guiWrite->topBarHeight(ImGui::GetStyle().WindowMinSize.y);
	else
		guiWrite->topBarHeight(windowHeight);

}

FS::TopBar::~TopBar() noexcept {
	FluidumScene_Log_Destructor(::FS::TopBar);
}

namespace FS::Internal::Bar {
	constexpr auto CommonWindowFlag =
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse;
}

void FS::TopBar::call() {

	ImGui::SetNextWindowPos(style.windowPos);
	ImGui::SetNextWindowSize(style.windowSize);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGui::Begin("TopBar", nullptr, Internal::Bar::CommonWindowFlag | ImGuiWindowFlags_NoBringToFrontOnFocus);
	this->windowBorder();
	ImGui::End();


	this->rightGui();
	this->calc();
	this->scene();


	ImGui::PopStyleVar(2);

}

void FS::TopBar::rightGui() {
	ImGui::SetNextWindowPos(ImVec2(style.windowSize.x * 0.7f, style.windowPos.y));
	ImGui::SetNextWindowSize(ImVec2(style.windowSize.x * 0.4f, style.windowSize.y));

	ImGui::Begin("Right", nullptr, Internal::Bar::CommonWindowFlag | ImGuiWindowFlags_NoBackground);

	ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();
	this->areaGui();

	ImGui::End();
}

void FS::TopBar::areaGui() {
	//static bool temp = false;
	//ImGui::Button(ICON_MD_BRANDING_WATERMARK);
	//if (ImGui::Button("aaa")) {
	//	constexpr FU::Class::ClassCode::CodeType code = FU::Class::ClassCode::GetClassCode<Analysis::Overview>();
	//	ImGuiWindow* window = imguiWindowRead->get<ImGuiWindow*>(code);
	//	ImGui::SetWindowDock(window, layoutRead->leftLayoutID(), ImGuiCond_Always);
	//}
}

void FS::TopBar::calc() {
	ImGui::SetNextWindowPos(ImVec2(style.windowSize.x * 0.4f, style.windowPos.y));
	ImGui::SetNextWindowSize(ImVec2(style.windowSize.x * 0.3f, style.windowSize.y));

	ImGui::Begin("Run", nullptr, Internal::Bar::CommonWindowFlag | ImGuiWindowFlags_NoBackground);
	this->separator(ImGui::GetWindowPos().x + 2.0f, { 0.3f,0.2f,0.2f,1.0f });
	ImGui::Spacing(); ImGui::SameLine();

	this->mode();
	ImGui::SameLine();

	const bool running = sceneRead->exist<Calc::Lua::Run>();

	if (running) {
		ImGui::Button(ICON_MD_SKIP_PREVIOUS);
	}
	else {
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.1f);
		ImGui::Button(ICON_MD_SKIP_PREVIOUS);
		ImGui::PopStyleVar();
	}

	ImGui::SameLine();

	if (running) {

		//request pause
		bool pause = ImGui::Button(ICON_MD_STOP);

		ImGui::SameLine();
	}
	else {
		bool run = ImGui::Button(ICON_MD_PLAY_ARROW);
		//FU::ImGui::tooltip(text.);
		if (run) {
			pos.run = FU::ImGui::messagePos();
			this->run();
		}
	}

	ImGui::SameLine();

	//next step
	if (running) {
		ImGui::Button(ICON_MD_SKIP_NEXT);

	}
	else {
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.1f);

		//request stop
		bool stop = ImGui::Button(ICON_MD_PAUSE);
		ImGui::SameLine();

		//next step
		ImGui::Button(ICON_MD_SKIP_NEXT);

		ImGui::PopStyleVar();
	}


	ImGui::End();

	this->playCheck();
}

void FS::TopBar::mode() {
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.275f);
	if (!ImGui::BeginCombo("##Mode", "Debug"))
		return;

	if (ImGui::Selectable("Standard"))
		std::cout << "a";

	ImGui::EndCombo();
}

void FS::TopBar::run() {
	if (!fluidumFilesRead->isMainCodeFileExist()) {
		FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
		Scene::addScene<Utils::Message>(text.error_mainfile, pos.run);
	}

	FluidumScene_Log_CallSceneConstructor(::FS::Calc::Run);
	Scene::callConstructor<Calc::Run>();
}

void FS::TopBar::playCheck() {
	//if (!wait.isPlayButtonClicked || !loadingParam.finishFlag.load())
	//	return;



	////計算を開始する命令が飛んできた

	////保存が速く終わっていても待機
	//const auto currentTime = std::chrono::system_clock::now();
	//if (currentTime > wait.time) {
	//	std::cout << "a\n\n\n\n";
	//	wait.reset();
	//	//Scene::addAsyncScene<FS::Lua>();
	//}



}

void FS::TopBar::scene() {
	ImGui::SetNextWindowPos(ImVec2(7.0f, style.windowPos.y - 2.0f));
	ImGui::SetNextWindowSize(ImVec2(style.windowSize.x * 0.4f, style.windowSize.y));

	ImGui::Begin("SceneFunc", nullptr, Internal::Bar::CommonWindowFlag | ImGuiWindowFlags_NoBackground);

	//combo button color
	if (!toolBarRead->getIndices()->empty())
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.01f, 0.01f, 1.0f));
	else
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.02f, 0.02f, 0.02f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.01f, 0.01f, 0.01f, 1.0f));

	this->combo();	ImGui::SameLine();
	ImGui::PopStyleColor(2);

	this->func();

	ImGui::End();
}

void FS::TopBar::combo() {
	const auto* info = toolBarRead->getInfo();

	if (!ImGui::BeginCombo("##SceneFuncCombo", "##None", ImGuiComboFlags_NoPreview))
		return;

	for (auto& x : *info) {
		if (ImGui::Selectable(x.sceneName.c_str(), x.select)) {
			if (!x.select)
				toolBarWrite->lock(x.code);
			else
				toolBarWrite->unlock(x.code);
		}
	}


	ImGui::EndCombo();

}

void FS::TopBar::func() {

	const auto* indices = toolBarRead->getIndices();

	if (indices->empty())
		return;

	for (auto itr = indices->begin(), end = (indices->end() - 1); itr != end; itr++) {
		ImGui::Spacing(); ImGui::SameLine();
		toolBarRead->call(*itr);
		ImGui::SameLine(); ImGui::Spacing();
		this->separator(ImGui::GetItemRectMax().x + 10.0f, { 0.4f, 0.2f, 0.2f, 1.0f });
		ImGui::SameLine();
	}

	ImGui::Spacing(); ImGui::SameLine();

	toolBarRead->call(indices->back());

}

void FS::TopBar::separator(const float posX, const ImVec4& col4) {

	ImDrawList* list = ImGui::GetWindowDrawList();
	const ImU32 col = ImGui::ColorConvertFloat4ToU32(col4);
	list->AddLine({ posX,style.windowPos.y + 2.0f }, { posX,style.windowPos.y + style.windowSize.y - 2.0f }, col);

}

#include <imgui_internal.h>
void FS::TopBar::windowBorder() {
	constexpr ImU32 col = FU::ImGui::ConvertImVec4ToImU32(0.4f, 0.4f, 0.4f, 1.0f);
	ImGui::GetWindowDrawList()->AddLine(
		{ style.windowPos.x ,style.windowPos.y + style.windowSize.y },
		{ style.windowPos.x + style.windowSize.x ,style.windowPos.y + style.windowSize.y },
		col,
		5.0f
	);
}