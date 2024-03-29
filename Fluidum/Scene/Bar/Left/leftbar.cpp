﻿#include "leftbar.h"

#include "../../Coding/Select/select.h"
#include "../../Coding/TextEditor/texteditor.h"
#include "../../Coding/Tab/tab.h"
#include "../../Coding/Debug/debug.h"
#include "../../Project/project.h"
#include "../../Analysis/Overview/overview.h"
#include "../../Analysis/Func/function.h"
#include "../../Analysis/Plot/plot.h"
#include "../../Flu/Node/node.h"
#include "../../Console/Console/console.h"
#include "../../Animation/Animation/animation.h"
#include "../../Genome/Overview/overview.h"


using namespace FU::ImGui::Operators;

FS::LeftBar::LeftBar(
	const FD::ProjectRead* const projectRead,
	const FD::FluidumFilesRead* const fluidumFilesRead,
	const FD::GuiRead* const  guiRead,
	FD::GuiWrite* const  guiWrite,
	const FD::SceneRead* const  sceneRead,
	const FD::Coding::TabRead* const tabRead,
	FD::Coding::TabWrite* const tabWrite,
	std::vector<FDR::ImGuiImage>&& images
) :
	projectRead(projectRead),
	fluidumFilesRead(fluidumFilesRead),
	guiRead(guiRead),
	sceneRead(sceneRead),
	tabWrite(tabWrite),
	tabRead(tabRead),
	images(images)
{
	GLog.add<FD::Log::Type::None>("Construct MenuBarScene.");


	const auto windowSizeX = guiRead->windowSize().x * 0.03f;
	style.windowPos = { 0.0f,guiRead->menuBarHeight() + guiRead->topBarHeight() };
	style.windowSize = { windowSizeX,guiRead->windowSize().y + 1.0f };

	//画像の幅 windowpaddingと内部のスペースを考慮する必要がある
	//windowpaddingを1/2にして調整する
	auto imageWidth = windowSizeX - (ImGui::GetStyle().WindowPadding.x) - (ImGui::GetStyle().FramePadding.x * 2.0f);

	style.imageSize = { imageWidth, imageWidth };

	guiWrite->leftBarWidth(style.windowSize.x);

	//set analysis icons
	for (uint16_t i = 0; i < 2; i++) {
		sub.analysisImages.emplace_back(images.at(images.size() - 2 + i));
	}

}

FS::LeftBar::~LeftBar() noexcept {
	try {
		GLog.add<FD::Log::Type::None>("Destruct MenuBarScene.");
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

void FS::LeftBar::call() {

	ImGui::SetNextWindowPos(style.windowPos);
	ImGui::SetNextWindowSize(style.windowSize);

	constexpr auto flag =
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollbar;

	//角とボーダーを消す
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImGui::GetStyle().WindowPadding / 2.0f);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.015f, 0.015f, 0.015f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.13f, 0.13f, 0.19f, 1.0f));

	ImGui::Begin("LeftBar", nullptr, flag);

	this->imageGui();

	ImGui::PopStyleColor(2);
	ImGui::PopStyleVar(3);

	ImGui::End();


	//subwindow

	if (sub.isIconHovered)
		this->subWindow();
}

namespace FS::Internal {
	//main scenes
	constexpr inline std::array<FU::Class::ClassCode::CodeType, 7> MainScenes = {
		FU::Class::ClassCode::GetClassCode<::FS::TextEditor>(),
		FU::Class::ClassCode::GetClassCode<::FS::Flu::Node>(),
		FU::Class::ClassCode::GetClassCode<::FS::Analysis::Overview>(),
		FU::Class::ClassCode::GetClassCode<::FS::Genome::Overview>(),
		FU::Class::ClassCode::GetClassCode<::FS::Animation>(),
		FU::Class::ClassCode::GetClassCode<::FS::Project>(),
		FU::Class::ClassCode::GetClassCode<::FS::Console>()
	};
}

void FS::LeftBar::imageGui() {
	ImGui::Spacing();

	for (std::size_t i = 0; i < std::tuple_size_v<decltype(Internal::MainScenes)>; i++) {
		//シーンが存在するなら降ろす
		if (sceneRead->isExist(Internal::MainScenes[i])) {
			//シーンの削除
			ImGui::ImageButton(this->images[i], style.imageSize, ImVec2(), ImVec2(1.0f, 1.0f), 2, color.main);

			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
				this->deleteScene(Internal::MainScenes[i]);

			this->hoveredIcon(i);
		}
		else {
			//ボタンが押されたら == シーンの追加を要請
			ImGui::ImageButton(this->images[i], style.imageSize, ImVec2(), ImVec2(1.0f, 1.0f), 2, color.dummy);

			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
				this->addScene(Internal::MainScenes[i]);
		}

		//つめつめなので少し空ける
		ImGui::Spacing(); ImGui::Spacing();
	}

}

void FS::LeftBar::hoveredIcon(const std::size_t index) {
	if (ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), { ImGui::GetItemRectMax().x + 20.0f,ImGui::GetItemRectMax().y })) {
		sub.current = static_cast<SceneIndex>(index + 1);
		sub.selectWindowPos = ImVec2(ImGui::GetItemRectMax().x + 1.0f, ImGui::GetItemRectMin().y);
		sub.isIconHovered = true;
	}
	else {
		if (!sub.isSubWindowHovered)
			sub.isIconHovered = false;
	}
}

void FS::LeftBar::addScene(const ClassCode::CodeType code) {
	if (code == Internal::MainScenes[0]) {
		this->addCodingScene();
	}
	else if (code == Internal::MainScenes[1]) {
		this->addFluScene();
	}
	else if (code == Internal::MainScenes[2]) {
		this->addAnalysisScene();
	}
	else if (code == Internal::MainScenes[3]) {
		this->addProjectScene();
	}
	else if (code == Internal::MainScenes[4]) {
		this->addAnimationScene();
	}
	else if (code == Internal::MainScenes[5]) {
		this->addGenomeScene();
	}
	else if (code == Internal::MainScenes[6]) {
		this->addConsoleScene();
	}
	else {
		GLog.add<FD::Log::Type::Error>("abort() has been called. File {}.", __FILE__);
		abort();
	}
}

void FS::LeftBar::addCodingScene() {
	//set image
	std::string path = Resource::LeftBarIconsFilePath;
	sub.codingImages.emplace_back(FDR::createImGuiImage((path + "tab.png").c_str()));	
	sub.codingImages.emplace_back(FDR::createImGuiImage((path + "debug.png").c_str()));

	if (!fluidumFilesRead->isMainCodeFileExist()) {
		GLog.add<FD::Log::Type::None>("Main file does not exist.");
		GLog.add<FD::Log::Type::None>("Request add CodingSelectScene.");
		Scene::addScene<CodingSelect>();
	}
	else {
		GLog.add<FD::Log::Type::None>("Request add TextEditorScene.");
		Scene::addScene<TextEditor>();
	}
}

void FS::LeftBar::addFluScene() {
	GLog.add<FD::Log::Type::None>("Request add Flu::NodeScene.");
	Scene::addScene<::FS::Flu::Node>();
}

void FS::LeftBar::addAnalysisScene() {
	GLog.add<FD::Log::Type::None>("Request add AnalysisScene.");
	Scene::addScene<::FS::Analysis::Overview>();
}

void FS::LeftBar::addProjectScene() {
	GLog.add<FD::Log::Type::None>("Request add ProjectScene.");
	Scene::addScene<::FS::Project>();
}

void FS::LeftBar::addAnimationScene() {
	GLog.add<FD::Log::Type::None>("Request add AnimationScene.");
	Scene::addScene<::FS::Animation>();
}

void FS::LeftBar::addGenomeScene() {
	GLog.add<FD::Log::Type::None>("Request add Genome::OverviewScene.");
	Scene::addScene<::FS::Genome::Overview>();
}

void FS::LeftBar::addConsoleScene() {
	GLog.add<FD::Log::Type::None>("Request add ConsoleScene.");
	Scene::addScene<::FS::Console>();
}

void FS::LeftBar::deleteScene(const ClassCode::CodeType code) {
	if (code == Internal::MainScenes[0]) {
		this->deleteCodingScene();
	}
	else if (code == Internal::MainScenes[1]) {
		GLog.add<FD::Log::Type::None>("Request delete Flu::NodeScene.");
		Scene::deleteScene<Flu::Node>();
	}
	else if (code == Internal::MainScenes[2]) {
		GLog.add<FD::Log::Type::None>("Request delete AnalysisScene.");
		Scene::deleteScene<Analysis::Overview>();
	}
	else if (code == Internal::MainScenes[3]) {
		GLog.add<FD::Log::Type::None>("Request delete ProjectScene.");
		Scene::deleteScene<Project>();
	}
	else if (code == Internal::MainScenes[4]) {
		GLog.add<FD::Log::Type::None>("Request delete AnimationScene.");
		Scene::deleteScene<Animation>();
	}
	else if (code == Internal::MainScenes[5]) {
		GLog.add<FD::Log::Type::None>("Request delete Genome::OverviewScene.");
		Scene::deleteScene<Genome::Overview>();
	}
	else if (code == Internal::MainScenes[6]) {
		GLog.add<FD::Log::Type::None>("Request delete ConsoleScene.");
		Scene::deleteScene<Console>();
	}
	else {
		GLog.add<FD::Log::Type::Error>("abort() has been called. File {}.", __FILE__);
		abort();
	}
	sub.codingImages.clear();
}

void FS::LeftBar::deleteCodingScene() {
	if (!tabRead->isAllTextSaved()) {
		GLog.add<FD::Log::Type::None>("Popup MessageBox.");
		int32_t button = FU::MB::button_button_cancel(FU::MB::Icon::Warning, text.popup_save, text.popup_saveAndClose, text.popup_withoutSaving, text.popup_cancel);
		if (button == 0) {//save close
			GLog.add<FD::Log::Type::None>("Save all tab texts.");
			tabWrite->saveAllTexts();
			tabWrite->clear();
			tabWrite->save();
		}
		else if (button == 1) {//without saving
			GLog.add<FD::Log::Type::None>("Clear tab texts.");
			tabWrite->clear();
			tabWrite->save();
		}
		else if (button == 2)//cancel
			return;
		else {//unexpected
			GLog.add<FD::Log::Type::Error>("abort() has been called. File {}.", __FILE__);
			abort();
		}
	}

	GLog.add<FD::Log::Type::None>("Request tryDelete Coding::TabScene.");
	Scene::tryDeleteScene<Coding::Tab>();
	GLog.add<FD::Log::Type::None>("Request delete TextEditorScene.");
	Scene::deleteScene<TextEditor>();
}

void FS::LeftBar::subWindow() {

	ImGui::SetNextWindowPos(sub.selectWindowPos);
	//padding xを半分
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(ImGui::GetStyle().WindowPadding.x / 1.8f, ImGui::GetStyle().WindowPadding.y));
	//frameも
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { ImGui::GetStyle().FramePadding.x / 2.0f,ImGui::GetStyle().FramePadding.y });
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

	ImGui::Begin("SubWinow", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

	sub.isSubWindowHovered = ImGui::IsWindowHovered();

	if (sub.current == SceneIndex::Coding)
		this->subWindowCoding();
	else if (sub.current == SceneIndex::AnalysisOverview)
		this->subWindowAnalysis();

	this->subWindowHelpSetting();


	ImGui::End();

	ImGui::PopStyleVar(3);
}

void FS::LeftBar::subWindowCoding() {

	constexpr std::array<ClassCode::CodeType, 1> scenes = {
		ClassCode::GetClassCode<Coding::Tab>()
	};

	for (std::size_t i = 0; i < std::tuple_size_v<decltype(scenes)>; i++) {

		//選択されているなら
		if (sceneRead->isExist(scenes[i])) {
			//降ろす　シーンの削除
			ImGui::ImageButton(sub.codingImages[i], style.imageSize, ImVec2(), ImVec2(1.0f, 1.0f), 2, color.sub);

			const ImVec2 pos1 = ImGui::GetItemRectMin();
			const ImVec2 pos2 = ImGui::GetItemRectMax();
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect(pos1, pos2, false)) {
				this->deleteCodingSubScene(scenes[i]);
			}
		}
		else {
			//ボタンが押されたら == シーンの追加を要請
			//ImageButtonは機能しないので，Rect + MouseEventでする
			ImGui::ImageButton(sub.codingImages[i], style.imageSize, ImVec2(), ImVec2(1.0f, 1.0f), 2, color.dummy);

			const ImVec2 pos1 = ImGui::GetItemRectMin();
			const ImVec2 pos2 = ImGui::GetItemRectMax();
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect(pos1, pos2, false)) {
				this->addCodingSubScene(scenes[i]);
			}

		}
	}
}

void FS::LeftBar::subWindowAnalysis() {
	constexpr std::array<ClassCode::CodeType, 2> scenes = {
		ClassCode::GetClassCode<Analysis::Function>(),
		ClassCode::GetClassCode<Analysis::Plot>()
	};

	for (std::size_t i = 0; i < std::tuple_size_v<decltype(scenes)>; i++) {

		if (sceneRead->isExist(scenes[i])) {
			ImGui::ImageButton(sub.analysisImages[i], style.imageSize, ImVec2(), ImVec2(1.0f, 1.0f), 2, color.sub);

			const ImVec2 pos1 = ImGui::GetItemRectMin();
			const ImVec2 pos2 = ImGui::GetItemRectMax();
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect(pos1, pos2, false)) {
				this->deleteAnalysisSubScene(scenes[i]);
			}
		}
		else {
			ImGui::ImageButton(sub.analysisImages[i], style.imageSize, ImVec2(), ImVec2(1.0f, 1.0f), 2, color.dummy);

			const ImVec2 pos1 = ImGui::GetItemRectMin();
			const ImVec2 pos2 = ImGui::GetItemRectMax();
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect(pos1, pos2, false)) {
				this->addAnalysisSubScene(scenes[i]);
			}

		}
	}
}

void FS::LeftBar::addCodingSubScene(const ClassCode::CodeType code) {
	if (code == ClassCode::GetClassCode<Coding::Tab>()) {
		GLog.add<FD::Log::Type::None>("Request add Coding::TabScene.");
		Scene::addScene<Coding::Tab>();
	}
	else if (code == ClassCode::GetClassCode<Coding::Debug>()){
		GLog.add<FD::Log::Type::None>("Request add Coding::DebugScene.");
		Scene::addScene<Coding::Debug>();
	}
	else {
		GLog.add<FD::Log::Type::Error>("abort() has been called. File {}.", __FILE__);
		abort();
	}
}

void FS::LeftBar::addAnalysisSubScene(const ClassCode::CodeType code) {
	if (code == ClassCode::GetClassCode<Analysis::Function>()) {
		GLog.add<FD::Log::Type::None>("Request add Analysis::FunctionScene.");
		Scene::addScene<Analysis::Function>();
	}
	else if (code == ClassCode::GetClassCode<Analysis::Plot>()) {
		GLog.add<FD::Log::Type::None>("Request add Analysis::PlotScene.");
		Scene::addScene<Analysis::Plot>();
	}
	else {
		GLog.add<FD::Log::Type::Error>("abort() has been called. File {}.", __FILE__);
		abort();
	}

}

void FS::LeftBar::deleteCodingSubScene(const ClassCode::CodeType code) {
	if (code == ClassCode::GetClassCode<Coding::Tab>()) {
		GLog.add<FD::Log::Type::None>("Request delete Coding::TabScene.");
		Scene::deleteScene<Coding::Tab>();
	}
	else {
		GLog.add<FD::Log::Type::Error>("abort() has been called. File {}.", __FILE__);
		abort();
	}
}

void FS::LeftBar::deleteAnalysisSubScene(const ClassCode::CodeType code) {
	if (code == ClassCode::GetClassCode<Analysis::Function>()) {
		GLog.add<FD::Log::Type::None>("Request delete Analysis::FunctionScene.");
		Scene::deleteScene<Analysis::Function>();
	}
	else if (code == ClassCode::GetClassCode<Analysis::Plot>()) {
		GLog.add<FD::Log::Type::None>("Request delete Analysis::PlotScene.");
		Scene::deleteScene<Analysis::Plot>();
	}
	else {
		GLog.add<FD::Log::Type::Error>("abort() has been called. File {}.", __FILE__);
		abort();
	}
}

void FS::LeftBar::subWindowHelpSetting() {
	//余白を取って，横線
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();


	//ボタンのボーダーを消す
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4());
	//ボタン間の距離を小さく
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { ImGui::GetStyle().ItemSpacing.x / 4.0f,ImGui::GetStyle().ItemSpacing.y });

	ImGui::SetWindowFontScale(0.8f);
	ImGui::Button(ICON_MD_HELP_OUTLINE);
	ImGui::SameLine();
	ImGui::Button(ICON_MD_SETTINGS);

	ImGui::PopStyleColor();
	ImGui::PopStyleVar();

}
