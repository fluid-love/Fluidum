#include "menubar.h"
#include <nfd.h>

#include "../../Layout/layout.h"
#include "../Exit/exit.h"

#include "../../Project/File/new.h"
#include "../../Project/File/saveas.h"
#include "../../Project/Property/property.h"

#include "../../Utils/Popup/backwindow.h"
#include "../../Utils/Popup/message.h"
#include "../../Utils/Popup/internal_error.h"
#include "../../Utils/Scene/deleteAll.h"

#include "../../Calc/Run/run.h"

#include "../../Utils/Scene/include.h"

using namespace FU::ImGui::Operators;

FS::MenuBar::MenuBar(
	const FD::Style::ColorRead* const colorRead,
	FD::ProjectWrite* const projectWrite,
	const FD::ProjectRead* const projectRead,
	FD::WindowWrite* const windowWrite,
	const FD::GuiRead* const guiRead,
	FD::GuiWrite* const guiWrite,
	FD::Coding::TabWrite* const tabWrite,
	const FD::Coding::TabRead* const tabRead,
	const FD::CalcRead* const calcRead,
	FD::CalcWrite* const calcWrite,
	const FD::SceneRead* const sceneRead
) :
	colorRead(colorRead),
	projectWrite(projectWrite),
	projectRead(projectRead),
	windowWrite(windowWrite),
	guiRead(guiRead),
	guiWrite(guiWrite),
	tabWrite(tabWrite),
	tabRead(tabRead),
	calcRead(calcRead),
	calcWrite(calcWrite),
	sceneRead(sceneRead)
{
	FluidumScene_Log_Constructor(::FS::MenuBar);

	const auto size = ImGui::GetFontSize() * 0.45f;
	style.offset = { size ,size };
	style.itemSpacing = { 0.0f ,size };
	guiWrite->menuBarHeight(ImGui::GetFontSize() + size * 2.0f);

}

FS::MenuBar::~MenuBar() noexcept {
	FluidumScene_Log_Destructor(::FS::MenuBar);
}

void FS::MenuBar::call() {

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, style.offset);

	//MenuBar size == ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2

	ImGui::SetNextWindowPos(ImVec2(), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(guiRead->windowSize().x, ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2), ImGuiCond_Once);

	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImGui::GetStyleColorVec4(ImGuiCol_PopupBg));
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.087f, 0.087f, 0.087f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.087f, 0.087f, 0.087f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.16f, 0.16f, 0.24f, 0.9f));

	constexpr ImGuiWindowFlags flag =
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_MenuBar |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoSavedSettings;

	ImGui::Begin("MenuBar", nullptr, flag);
	if (ImGui::BeginMenuBar()) {
		//dummy: for Fluidum icon
		if (ImGui::BeginMenu("   "))
			ImGui::EndMenu();

		this->fileGui();
		this->editGui();
		this->calcGui();
		this->viewGui();
		this->projectGui();
		this->extensionGui();
		this->windowGui();
		this->helpGui();

		ImGui::EndMenuBar();
	}
	ImGui::End();

	ImGui::PopStyleColor(4);
	ImGui::PopStyleVar();

}

void FS::MenuBar::fileGui() {

	if (!ImGui::BeginMenu(text.file))
		return;

	this->pushItemSpacing();

	this->itemCreateNewProject();
	this->itemOpen();
	this->itemSave();
	this->itemSaveAs();

	ImGui::Separator();

	this->itemExit();

	ImGui::PopStyleVar();

	ImGui::EndMenu();
}

void FS::MenuBar::itemCreateNewProject() {
	if (!ImGui::MenuItem(text.create))
		return;

	FluidumScene_Log_RequestAddScene(::FS::Project::File::New);
	Scene::addScene<Project::File::New>();
}

void FS::MenuBar::itemOpen() {
	if (!ImGui::MenuItem(text.open))
		return;
	pos.open = ImGui::GetItemRectMax();

	std::unique_ptr<nfdchar_t*> outPath = std::make_unique<nfdchar_t*>();
	GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Open file dialog.");
	const nfdresult_t result = NFD_OpenDialog(".fproj", nullptr, outPath.get());
	if (result == NFD_OKAY) {
		GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Load project file({}).", *outPath.get());
	}
	else if (result == NFD_CANCEL) {
		GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Dialog has been canceled.");
		return;
	}
	else {//NFD_ERROR
		{
			auto lock = GLog.getLock();
			GLog.add<FU::Log::Type::Warning>(__FILE__, __LINE__, "Error file dialog.");
			FluidumScene_Log_InternalWarning();
		}
		return;
	}

	FU::Cursor::setCursorType(FU::Cursor::Type::Wait);

	try {
		projectWrite->loadProject(*outPath.get());
	}
	catch (const FD::ProjectWrite::Exception type) {
		using enum FD::ProjectWrite::Exception;

		//std::ifstream::operator bool() == false
		if (type == FailedToOpenProjectFile) {
			FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
			Scene::addScene<Utils::Message>(text.error_openProjectFile, pos.open);
		}
		//wrong identifier 
		else if (type == IllegalFile) {
			FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
			Scene::addScene<Utils::Message>(text.error_illegalFile, pos.open);
		}
		//broken file
		else if (type == BrokenFile) {
			FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
			Scene::addScene<Utils::Message>(text.error_brokenFile, pos.open);
		}
		else {
			FluidumScene_Log_InternalWarning();
		}
		GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Failed to open .fproj file.");
		return;
	}
	catch (const std::exception& e) {
		FluidumScene_Log_StdExceptionError(e);
		FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
		Scene::addScene<Utils::Message>(text.error_internal, pos.open);
		return;
	}
	catch (...) {
		FluidumScene_Log_InternalWarning();
		return;
	}

	//reset Layout
	FluidumScene_Log_RecreateScene(::FS::Layout);
	Scene::recreateScene<Layout>();

}

void FS::MenuBar::itemSave() {
	if (!ImGui::MenuItem(text.save))
		return;

	if (tabRead->isAllTextSaved())
		return;

	tabWrite->saveAllTexts();
}

void FS::MenuBar::itemSaveAs() {
	const bool running = sceneRead->running();
	if (!ImGui::MenuItem(text.saveFileAs, nullptr, false, !running))
		return;

	FluidumScene_Log_RequestAddScene(::FS::Project::File::SaveAs);
	Scene::addScene<Project::File::SaveAs>();
}

void FS::MenuBar::itemExit() {
	//red
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, colorRead->error());

	if (!ImGui::MenuItem(text.terminate)) {
		ImGui::PopStyleColor();
		return;
	}

	GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "[Request] Exit.");

	FluidumScene_Log_RequestAddScene(::FS::Bar::Exit);
	Scene::addScene<Bar::Exit>();

	ImGui::PopStyleColor();
}

void FS::MenuBar::editGui() {
	if (!ImGui::BeginMenu(text.edit))
		return;

	this->pushItemSpacing();


	ImGui::PopStyleVar();
	ImGui::EndMenu();
}

void FS::MenuBar::calcGui() {
	if (!ImGui::BeginMenu(text.calc))
		return;

	bool exist = false;

	this->pushItemSpacing();

	if (ImGui::MenuItem(text.run_debug, nullptr, false, exist)) {
		//this->itemRunDebugMode();
	}

	if (ImGui::MenuItem(text.run_nomal, nullptr, false, exist)) {
		//this->itemRunNormalMode();
	}

	ImGui::Separator();
	ImGui::Spacing();

	if (ImGui::MenuItem(text.calcInfo)) {

	}

	ImGui::PopStyleVar();
	ImGui::EndMenu();
}

void FS::MenuBar::itemRunDebugMode() {
	if (calcRead->mode() != FD::Calc::Mode::Debug) {
		calcWrite->mode(FD::Calc::Mode::Debug);
		calcWrite->save();
	}

	//FluidumScene_Log_RequestAddScene(::FS::Calc::Run);
	//Scene::addScene<Calc::Run>();
}

void FS::MenuBar::itemRunNormalMode() {
	if (calcRead->mode() != FD::Calc::Mode::Normal) {
		calcWrite->mode(FD::Calc::Mode::Normal);
		calcWrite->save();
	}

	//FluidumScene_Log_RequestAddScene(::FS::Calc::Run);
	//Scene::addScene<Calc::Run>();
}

void FS::MenuBar::viewGui() {
	if (!ImGui::BeginMenu(text.view))
		return;

	//once
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsWindowHovered()) {
		this->updateView();
	}

	this->pushItemSpacing();

	if (ImGui::BeginMenu(text.coding)) {
		this->itemCoding();
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu(text.flu)) {
		this->itemFlu();
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu(text.analysis)) {
		this->itemAnalysis();
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu(text.genome)) {
		this->itemGenome();
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu(text.animation)) {
		this->itemAnimation();
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu(text.project)) {
		this->itemProject();
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu(text.console)) {
		this->itemConsole();
		ImGui::EndMenu();
	}

	ImGui::PopStyleVar();

	ImGui::EndMenu();
}

void FS::MenuBar::updateView() {
	view.coding = sceneRead->exist<TextEditor>();
	view.tab = sceneRead->exist<Coding::Tab>();
	view.debug = sceneRead->exist<Coding::Debug>();

	view.flu = sceneRead->exist<Flu::Node>();

	view.analysis = sceneRead->exist<Analysis::Overview>();
	view.plot = sceneRead->exist<Analysis::Plot>();
	view.function = sceneRead->exist<Analysis::Function>();

	view.genome = sceneRead->exist<Genome::Overview>();

	view.animation = sceneRead->exist<Animation>();

	view.project = sceneRead->exist<Project::Explorer>();

	view.console = sceneRead->exist<Console>();
}

void FS::MenuBar::windowGui() {
	if (!ImGui::BeginMenu(text.window))
		return;

	if (ImGui::BeginMenu(text.layoutTemplates)) {
		this->itemLayoutTemplates();
	}

	ImGui::EndMenu();
}

void FS::MenuBar::itemLayoutTemplates() {
	if (!ImGui::MenuItem(text.clear, nullptr)) {
		if (this->layoutConfirm())
			this->setLayoutEmpty();
	}

	if (!ImGui::MenuItem(text.coding, nullptr)) {
		if (this->layoutConfirm())
			this->setLayoutCoding();
	}
}

bool FS::MenuBar::layoutConfirm() {
	GLog.add_str<FU::Log::Type::None>("Set layout template.");

	const auto clicked = FU::MB::ok_cancel(text.confirm_changeLayout);
	if (clicked == 0) {//ok
		return true;
	}
	else if (clicked == 1) {//cancel
		return false;;
	}

	//else
	FluidumScene_Log_InternalError();
	FluidumScene_Log_CallSceneConstructor(::FS::Utils::InternalError);
	Scene::callConstructor<Utils::InternalError>();
	return false;
}

void FS::MenuBar::setLayoutEmpty() {
	//delete all view-scenes.
	FluidumScene_Log_CallSceneConstructor(::FS::Utils::DeleteAllScenes);
	Scene::callConstructor<Utils::DeleteAllScenes>();
}

void FS::MenuBar::setLayoutCoding() {

}

void FS::MenuBar::extensionGui() {
	if (!ImGui::BeginMenu(text.extension))
		return;

	if (ImGui::MenuItem(text.manage, nullptr, false, false)) {

	}


	ImGui::EndMenu();
}

void FS::MenuBar::itemCoding() {

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsWindowHovered() && ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
		FluidumScene_Log_RequestTryAddScene(::FS::TextEditor);
		Scene::tryAddScene<TextEditor>();
		ImGui::CloseCurrentPopup();
	}

	if (ImGui::MenuItem(text.tab, nullptr, view.tab, view.coding)) {
		FluidumScene_Log_RequestAddScene(::FS::Coding::Tab);
		Scene::addScene<Coding::Tab>();
	}

	if (ImGui::MenuItem(text.debugInfo, nullptr, view.debug, view.coding)) {
		FluidumScene_Log_RequestAddScene(::FS::Coding::Debug);
		Scene::addScene<Coding::Debug>();
	}
}

void FS::MenuBar::itemFlu() {
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsWindowHovered() && ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
		if (view.flu) {
			FluidumScene_Log_RequestDeleteScene(::FS::Flu::Node);
			Scene::deleteScene<Flu::Node>();
		}
		else {
			FluidumScene_Log_RequestAddScene(::FS::Flu::Node);
			Scene::addScene<Flu::Node>();
		}
		ImGui::CloseCurrentPopup();
	}
}

void FS::MenuBar::itemAnalysis() {
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsWindowHovered() && ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
		if (view.analysis) {
			FluidumScene_Log_RequestDeleteScene(::FS::Analysis::Plot);
			Scene::deleteScene<Analysis::Plot>();
			FluidumScene_Log_RequestDeleteScene(::FS::Analysis::Function);
			Scene::deleteScene<Analysis::Function>();
			FluidumScene_Log_RequestDeleteScene(::FS::Analysis::Overview);
			Scene::deleteScene<Analysis::Overview>();
		}
		else {
			FluidumScene_Log_RequestAddScene(::FS::Analysis::Overview);
			Scene::addScene<Analysis::Overview>();
		}

		ImGui::CloseCurrentPopup();
	}

	if (ImGui::MenuItem(text.plot, nullptr, view.plot, view.analysis)) {
		if (view.plot) {
			FluidumScene_Log_RequestDeleteScene(::FS::Analysis::Plot);
			Scene::deleteScene<Analysis::Plot>();
		}
		else {
			FluidumScene_Log_RequestAddScene(::FS::Analysis::Plot);
			Scene::addScene<Analysis::Plot>();
		}
	}

	if (ImGui::MenuItem(text.function, nullptr, view.function, view.analysis)) {
		if (view.function) {
			FluidumScene_Log_RequestDeleteScene(::FS::Analysis::Function);
			Scene::deleteScene<Analysis::Function>();
		}
		else {
			FluidumScene_Log_RequestAddScene(::FS::Analysis::Function);
			Scene::addScene<Analysis::Function>();
		}
	}
}

void FS::MenuBar::itemGenome() {
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsWindowHovered() && ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
		if (view.genome) {
			FluidumScene_Log_RequestDeleteScene(::FS::Genome::Overview);
			Scene::deleteScene<Genome::Overview>();
		}
		else {
			FluidumScene_Log_RequestAddScene(::FS::Genome::Overview);
			Scene::addScene<Genome::Overview>();
		}
		ImGui::CloseCurrentPopup();
	}
}

void FS::MenuBar::itemAnimation() {
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsWindowHovered() && ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
		if (view.animation) {
			FluidumScene_Log_RequestDeleteScene(::FS::Animation);
			Scene::deleteScene<Animation>();
		}
		else {
			FluidumScene_Log_RequestAddScene(::FS::Animation);
			Scene::addScene<Animation>();
		}
		ImGui::CloseCurrentPopup();
	}
}

void FS::MenuBar::itemProject() {
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsWindowHovered() && ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
		if (view.project) {
			FluidumScene_Log_RequestDeleteScene(::FS::Project::Explorer);
			Scene::deleteScene<Project::Explorer>();
		}
		else {
			FluidumScene_Log_RequestAddScene(::FS::Project::Explorer);
			Scene::addScene<Project::Explorer>();
		}
		ImGui::CloseCurrentPopup();
	}
}

void FS::MenuBar::itemConsole() {
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsWindowHovered() && ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
		if (view.console) {
			FluidumScene_Log_RequestDeleteScene(::FS::Console);
			Scene::deleteScene<Console>();
		}
		else {
			FluidumScene_Log_RequestAddScene(::FS::Console);
			Scene::addScene<Console>();
		}
		ImGui::CloseCurrentPopup();
	}
}

void FS::MenuBar::projectGui() {
	if (!ImGui::BeginMenu(text.project_))
		return;

	if (ImGui::MenuItem(text.property_icon)) {
		this->project_property();
	}

	ImGui::EndMenu();
}

void FS::MenuBar::project_property() {
	FluidumScene_Log_RequestAddScene(::FS::Project::Property);
	Scene::addScene<Project::Property>();
}

void FS::MenuBar::helpGui() {
	if (!ImGui::BeginMenu(text.help))
		return;

	this->pushItemSpacing();

	if (ImGui::MenuItem("github")) {
#ifdef BOOST_OS_WINDOWS
		system("start https://github.com/fluid-love/Fluidum");
#elif BOOST_OS_MAC
		system("open https://github.com/fluid-love/Fluidum");
#else
#error Not Supported
#endif 
	}
	if (ImGui::MenuItem(text.document)) {
#ifdef BOOST_OS_WINDOWS
		system("start https://fluidum.fluid-love.com/manual_beta/");
#elif BOOST_OS_MAC
		system("open https://fluidum.fluid-love.com/manual_beta/");
#else
#error Not Supported
#endif 
	}


	ImGui::PopStyleVar();
	ImGui::EndMenu();
}

void FS::MenuBar::pushItemSpacing() {
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f ,ImGui::GetStyle().ItemSpacing.y * 2.2f });
}











