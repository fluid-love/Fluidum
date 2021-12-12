#include "menubar.h"
#include <nfd.h>

#include "../Title/Exit/exit.h"

#include "Project/newproject.h"
#include "Project/saveas.h"

#include "../../Utils/Popup/backwindow.h"
#include "../../Utils/Popup/popupselect.h"
#include "../../Utils/Popup/message.h"
#include "../../Utils/Popup/internal_error.h"
#include "../../Utils/Scene/deleteAll.h"

#include "../../Calc/Run/run.h"

#include "../../Utils/Scene/include.h"

using namespace FU::ImGui::Operators;

FS::MenuBar::MenuBar(
	FD::ProjectWrite* const projectWrite,
	const FD::ProjectRead* const projectRead,
	FD::WindowWrite* const windowWrite,
	const FD::GuiRead* const guiRead,
	FD::GuiWrite* const guiWrite,
	FD::Coding::TabWrite* const tabWrite,
	const FD::Coding::TabRead* const tabRead,
	const FD::FluidumFilesRead* const fluidumFilesRead,
	const FD::CalcRead* const calcRead,
	FD::CalcWrite* const calcWrite,
	const FD::SceneRead* const sceneRead
) :
	projectWrite(projectWrite),
	projectRead(projectRead),
	windowWrite(windowWrite),
	guiRead(guiRead),
	guiWrite(guiWrite),
	tabWrite(tabWrite),
	tabRead(tabRead),
	fluidumFilesRead(fluidumFilesRead),
	calcRead(calcRead),
	calcWrite(calcWrite),
	sceneRead(sceneRead)
{
	GLog.add<FD::Log::Type::None>("Construct MenuBarScene.");

	const auto size = ImGui::GetFontSize() * 0.45f;
	style.offset = { size ,size };
	style.itemSpacing = { 0.0f ,size };
	guiWrite->menuBarHeight(ImGui::GetFontSize() + size * 2.0f);

}

FS::MenuBar::~MenuBar() {
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

void FS::MenuBar::call() {

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, style.offset);//Œú‚Ý‚ð‚à‚½‚¹‚é

	//MenuBar‚ÌƒTƒCƒY‚ÍImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2

	ImGui::SetNextWindowPos(ImVec2(), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(guiRead->windowSize().x, ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2), ImGuiCond_Once);

	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.017f, 0.017f, 0.017f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.087f, 0.087f, 0.087f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.087f, 0.087f, 0.087f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.16f, 0.16f, 0.24f, 0.9f));
	ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.017f, 0.017f, 0.017f, 1.0f));

	constexpr ImGuiWindowFlags flag =
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_MenuBar |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove;

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

	ImGui::PopStyleColor(5);
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
	ImGui::Spacing();

	this->itemExit();

	ImGui::PopStyleVar();

	ImGui::EndMenu();
}

void FS::MenuBar::itemCreateNewProject() {
	if (!ImGui::MenuItem(text.create))
		return;

	GLog.add<FD::Log::Type::None>("Request add NewProjectScene.");
	Scene::addScene<Bar::NewProject>();
}

void FS::MenuBar::itemOpen() {
	if (!ImGui::MenuItem(text.open))
		return;
	pos.open = ImGui::GetItemRectMax();

	std::unique_ptr<nfdchar_t*> outPath = std::make_unique<nfdchar_t*>();
	GLog.add<FD::Log::Type::None>("Open file dialog.");
	const nfdresult_t result = NFD_OpenDialog(".fproj", nullptr, outPath.get());
	if (result == NFD_OKAY) {
		GLog.add<FD::Log::Type::None>("Load project file({}).", *outPath.get());
	}
	else if (result == NFD_CANCEL) {
		GLog.add<FD::Log::Type::None>("Dialog has been canceled.");
		return;
	}
	else {//NFD_ERROR
		GLog.add<FD::Log::Type::Error>("Error file dialog.");
		throw std::runtime_error("NFD_OpenDialog() return NFD_ERROR.");
	}

	FU::Cursor::setCursorType(FU::Cursor::Type::Wait);

	try {
		projectWrite->loadProject(*outPath.get());
	}
	catch (const FD::Project::ExceptionType type) {
		GLog.add<FD::Log::Type::None>("Request add Utils::MessageScene.");

		//std::ifstream::operator bool() == false
		if (type == FD::Project::ExceptionType::FailedToOpenProjectFile) {
			Scene::addScene<Utils::Message>(text.error_openProjectFile, pos.open);
		}
		//wrong identifier 
		else if (type == FD::Project::ExceptionType::IllegalFile) {
			Scene::addScene<Utils::Message>(text.error_illegalFile, pos.open);
		}
		//broken file
		else if (type == FD::Project::ExceptionType::BrokenFile) {
			Scene::addScene<Utils::Message>(text.error_brokenFile, pos.open);
		}
		else {
			GLog.add<FD::Log::Type::Error>("abort() has been called. File {}.", __FILE__);
			abort();
		}
		GLog.add<FD::Log::Type::None>("Failed to open .fproj file.");
		return;
	}
	catch (const std::exception&) {
		GLog.add<FD::Log::Type::Error>("Failed to open .fproj file.");
		GLog.add<FD::Log::Type::None>("Request add Utils::MessageScene.");
		Scene::addScene<Utils::Message>(text.error_internal, pos.open);
		return;
	}
}

void FS::MenuBar::itemSave() {
	if (!ImGui::MenuItem(text.save))
		return;

	if (tabRead->isAllTextSaved())
		return;

	tabWrite->saveAllTexts();
}

void FS::MenuBar::itemSaveAs() {
	if (!ImGui::MenuItem(text.saveFileAs))
		return;

	GLog.add<FD::Log::Type::None>("Request add Bar::SaveAsScene.");
	Scene::addScene<Bar::SaveAs>();
}

void FS::MenuBar::itemExit() {
	//red
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.7f, 0.05f, 0.05f, 0.5f));

	if (!ImGui::MenuItem(text.terminate)) {
		ImGui::PopStyleColor();
		return;
	}

	GLog.add<FD::Log::Type::None>("Request Exit.");

	GLog.add<FD::Log::Type::None>("Request Add Bar::ExitScene.");
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

	bool exist = fluidumFilesRead->isMainCodeFileExist();

	this->pushItemSpacing();

	if (ImGui::MenuItem(text.run_debug, nullptr, false, exist)) {
		this->itemRunDebugMode();
	}

	if (ImGui::MenuItem(text.run_nomal, nullptr, false, exist)) {
		this->itemRunNormalMode();
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

	GLog.add<FD::Log::Type::None>("Request add Calc::RunScene.");
	Scene::addScene<Calc::Run>();
}

void FS::MenuBar::itemRunNormalMode() {
	if (calcRead->mode() != FD::Calc::Mode::Normal) {
		calcWrite->mode(FD::Calc::Mode::Normal);
		calcWrite->save();
	}

	GLog.add<FD::Log::Type::None>("Request add Calc::RunScene.");
	Scene::addScene<Calc::Run>();
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
	view.coding = sceneRead->isExist<TextEditor>();
	view.tab = sceneRead->isExist<Coding::Tab>();
	view.debug = sceneRead->isExist<Coding::Debug>();

	view.flu = sceneRead->isExist<Flu::Node>();

	view.analysis = sceneRead->isExist<Analysis::Overview>();
	view.plot = sceneRead->isExist<Analysis::Plot>();
	view.function = sceneRead->isExist<Analysis::Function>();

	view.genome = sceneRead->isExist<Genome::Overview>();

	view.animation = sceneRead->isExist<Animation>();

	view.project = sceneRead->isExist<Project>();

	view.console = sceneRead->isExist<Console>();
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
	GLog.add<FD::Log::Type::None>("Set layout template.");

	const auto clicked = FU::MB::ok_cancel(text.confirm_changeLayout);
	if (clicked == 0) {//ok
		return true;
	}
	else if (clicked == 1) {//cancel
		return false;;
	}

	//else
	FluidumScene_Log_InternalError();
	FluidumScene_Log_CallSceneConstructor("Utils::InternalError");
	Scene::callConstructor<Utils::InternalError>();
	return false;
}

void FS::MenuBar::setLayoutEmpty() {
	FluidumScene_Log_CallSceneConstructor("Utils::DeleteAllScenes");
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
		FluidumScene_Log_RequestTryAddScene("TextEditor");
		Scene::tryAddScene<TextEditor>();
		ImGui::CloseCurrentPopup();
	}

	if (ImGui::MenuItem(text.tab, nullptr, view.tab, view.coding)) {
		FluidumScene_Log_RequestAddScene("Coding::Tab");
		Scene::addScene<Coding::Tab>();
	}

	if (ImGui::MenuItem(text.debugInfo, nullptr, view.debug, view.coding)) {
		FluidumScene_Log_RequestAddScene("Coding::Debug");
		Scene::addScene<Coding::Debug>();
	}
}

void FS::MenuBar::itemFlu() {
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsWindowHovered() && ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
		if (view.flu) {
			FluidumScene_Log_RequestDeleteScene("Flu::Node");
			Scene::deleteScene<Flu::Node>();
		}
		else {
			FluidumScene_Log_RequestAddScene("Flu::Node");
			Scene::addScene<Flu::Node>();
		}
		ImGui::CloseCurrentPopup();
	}
}

void FS::MenuBar::itemAnalysis() {
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsWindowHovered() && ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
		if (view.analysis) {
			FluidumScene_Log_RequestDeleteScene("Analysis::Plot");
			Scene::deleteScene<Analysis::Plot>();
			FluidumScene_Log_RequestDeleteScene("Analysis::Function");
			Scene::deleteScene<Analysis::Function>();
			FluidumScene_Log_RequestDeleteScene("Analysis::Overview");
			Scene::deleteScene<Analysis::Overview>();
		}
		else {
			FluidumScene_Log_RequestAddScene("Analysis::Overview");
			Scene::addScene<Analysis::Overview>();
		}

		ImGui::CloseCurrentPopup();
	}

	if (ImGui::MenuItem(text.plot, nullptr, view.plot, view.analysis)) {
		if (view.plot) {
			FluidumScene_Log_RequestDeleteScene("Analysis::Plot");
			Scene::deleteScene<Analysis::Plot>();
		}
		else {
			FluidumScene_Log_RequestAddScene("Analysis::Plot");
			Scene::addScene<Analysis::Plot>();
		}
	}

	if (ImGui::MenuItem(text.function, nullptr, view.function, view.analysis)) {
		if (view.function) {
			FluidumScene_Log_RequestDeleteScene("Analysis::Function");
			Scene::deleteScene<Analysis::Function>();
		}
		else {
			FluidumScene_Log_RequestAddScene("Analysis::Function");
			Scene::addScene<Analysis::Function>();
		}
	}
}

void FS::MenuBar::itemGenome() {
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsWindowHovered() && ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
		if (view.genome) {
			FluidumScene_Log_RequestDeleteScene("Genome::Overview");
			Scene::deleteScene<Genome::Overview>();
		}
		else {
			FluidumScene_Log_RequestAddScene("Genome::Overview");
			Scene::addScene<Genome::Overview>();
		}
		ImGui::CloseCurrentPopup();
	}
}

void FS::MenuBar::itemAnimation() {
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsWindowHovered() && ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
		if (view.animation) {
			FluidumScene_Log_RequestDeleteScene("Animation");
			Scene::deleteScene<Animation>();
		}
		else {
			FluidumScene_Log_RequestAddScene("Animation");
			Scene::addScene<Animation>();
		}
		ImGui::CloseCurrentPopup();
	}
}

void FS::MenuBar::itemProject() {
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsWindowHovered() && ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
		if (view.project) {
			FluidumScene_Log_RequestDeleteScene("Project");
			Scene::deleteScene<Project>();
		}
		else {
			FluidumScene_Log_RequestAddScene("Project");
			Scene::addScene<Project>();
		}
		ImGui::CloseCurrentPopup();
	}
}

void FS::MenuBar::itemConsole() {
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsWindowHovered() && ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
		if (view.console) {
			FluidumScene_Log_RequestDeleteScene("Console");
			Scene::deleteScene<Console>();
		}
		else {
			FluidumScene_Log_RequestAddScene("Console");
			Scene::addScene<Console>();
		}
		ImGui::CloseCurrentPopup();
	}
}

void FS::MenuBar::projectGui() {
	if (!ImGui::BeginMenu(text.project_))
		return;

	ImGui::EndMenu();
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
		system("start https://github.com/fluid-love/Fluidum/tree/master/Document");
#elif BOOST_OS_MAC
		system("open https://github.com/fluid-love/Fluidum/tree/master/Document");
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











