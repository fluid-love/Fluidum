#include "menubar.h"
#include <nfd.h>

#include "Project/newproject.h"
#include "Project/saveas.h"

#include "../../Utils/Popup/backwindow.h"
#include "../../Utils/Popup/popupselect.h"
#include "../../Utils/Popup/message.h"

using namespace FU::ImGui::Operators;

FS::MenuBar::MenuBar(
	FD::ProjectWrite* const projectWrite,
	const FD::ProjectRead* const projectRead,
	FD::WindowWrite* const windowWrite,
	const FD::GuiRead* const guiRead,
	FD::GuiWrite* const guiWrite,
	FD::Coding::TabWrite* const tabWrite,
	const FD::Coding::TabRead* const tabRead
) :
	projectWrite(projectWrite),
	projectRead(projectRead),
	windowWrite(windowWrite),
	guiRead(guiRead),
	guiWrite(guiWrite),
	tabWrite(tabWrite),
	tabRead(tabRead)
{
	GLog.add<FD::Log::Type::None>("Construct MenuBarScene.");

	const auto size = ImGui::GetFontSize() * 0.45f;
	style.offset = { size ,size };

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
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, style.offset);//å˙Ç›ÇÇ‡ÇΩÇπÇÈ

	//MenuBarÇÃÉTÉCÉYÇÕImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2

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
		//dummy for icon
		if (ImGui::BeginMenu("   "))
			ImGui::EndMenu();


		this->fileGui();
		this->editGui();
		this->calc();
		this->viewGui();
		this->helpGui();

		ImGui::EndMenuBar();
	}
	ImGui::End();

	ImGui::PopStyleColor(5);
	ImGui::PopStyleVar();

}

void FS::MenuBar::fileGui() {

	if (ImGui::BeginMenu(text.file)) {

		this->itemCreateNewProject();
		ImGui::Spacing();
		this->itemOpen();
		ImGui::Spacing();
		this->itemSave();
		ImGui::Spacing();
		this->itemSaveAs();

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		this->itemExit();

		ImGui::EndMenu();
	}
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
	//ï¬Ç∂ÇÈÇÕê‘Ç≠Ç∑ÇÈ
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.7f, 0.05f, 0.05f, 0.5f));
	if (!ImGui::MenuItem(text.terminate)) {
		ImGui::PopStyleColor();
		return;
	}
	GLog.add<FD::Log::Type::None>("Request Terminate.");

	if (projectRead->isDefaultProject()) {
		auto index = FU::MB::button_button_cancel(FU::MB::Icon::Warning, text.checkCurrentProject, text.saveAndExit, text.exitWithoutSaving, text.cancel);
		if (index == 0) {//save

		}
		else if (index == 1) {//without saving

		}
		else {//cancel
			ImGui::PopStyleColor();
			return;
		}
	}

	*windowWrite->getCloseFlag() = true;
	ImGui::PopStyleColor();
}

void FS::MenuBar::editGui() {
	if (ImGui::BeginMenu(text.edit)) {

		ImGui::EndMenu();
	}
}

void FS::MenuBar::calc() {
	if (ImGui::BeginMenu(text.calc)) {

		ImGui::Spacing();
		if (ImGui::MenuItem(text.run)) {

		}
		ImGui::Spacing();
		if (ImGui::MenuItem(text.runIgnoreStep)) {

		}
		ImGui::Spacing();
		if (ImGui::MenuItem(text.calcInfo)) {

		}

		ImGui::EndMenu();
	}

}

void FS::MenuBar::viewGui() {
	if (ImGui::BeginMenu(text.view)) {

		ImGui::EndMenu();
	}
}

void FS::MenuBar::helpGui() {
	if (!ImGui::BeginMenu(text.help))
		return;

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


	ImGui::EndMenu();

}






















