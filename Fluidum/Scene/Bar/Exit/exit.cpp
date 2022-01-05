#include "exit.h"
#include "../../Project/File/saveas.h"

FS::Bar::Exit::Exit(
	const FD::WindowRead* const windowRead,
	FD::WindowWrite* const windowWrite,
	const FD::SceneRead* const sceneRead,
	const FD::ProjectRead* const projectRead,
	FD::ExitWrite* const exitWrite,
	const FD::Coding::TabRead* const tabRead,
	FD::Coding::TabWrite* const tabWrite
) :
	windowWrite(windowWrite),
	windowRead(windowRead),
	sceneRead(sceneRead),
	projectRead(projectRead),
	exitWrite(exitWrite),
	tabRead(tabRead),
	tabWrite(tabWrite)
{
	FluidumScene_Log_Constructor(::FS::Bar::Exit);
}

FS::Bar::Exit::~Exit() noexcept {
	FluidumScene_Log_Destructor(::FS::Bar::Exit);
}

void FS::Bar::Exit::call() {
	this->exit();
	this->deleteThis();
}

void FS::Bar::Exit::exit() {

	bool tab = this->confirmTab();
	if (!tab)
		return;

	bool project = this->confirmDefaultProject();
	if (!project)
		return;

	*windowWrite->getCloseFlag() = true;
}

bool FS::Bar::Exit::confirmDefaultProject() {
	if (!projectRead->isDefaultProject())
		return true;

	auto index = FU::MB::button_button_cancel(FU::MB::Icon::Warning, text.popup_projectMessage, text.popup_saveAndExit, text.popup_withoutSaving, text.popup_cancel);
	if (index == 0) {//save
		FluidumScene_Log_RequestAddScene(::FS::Project::File::SaveAs);
		Scene::addScene<Project::File::SaveAs>();
		exitWrite->saveAsAndExit();
		return false;
	}
	else if (index == 1) {//without saving
		return true;
	}

	//canceled
	return false;
}

bool FS::Bar::Exit::confirmTab() {

	if (tabRead->isAllTextSaved())
		return true;

	std::string caption = text.popup_codingTabMessage.operator const char *();
	caption.reserve(500);

	caption += "\n\n";

	const std::vector<std::string> texts = tabRead->notSavedTexts();
	for (auto& x : texts) {
		(caption += x) += '\n';
	}
	if (!texts.empty())
		caption.pop_back();//remove '\n'

	auto index = FU::MB::button_button_cancel(FU::MB::Icon::Warning, caption.c_str(), text.popup_saveAndExit, text.popup_withoutSaving, text.popup_cancel);
	if (index == 0) {//save
		tabWrite->saveAllTexts();
		return true;
	}
	else if (index == 1) {//without saving
		return true;
	}

	//cancel
	return false;
}

void FS::Bar::Exit::deleteThis() {
	FluidumScene_Log_RequestDeleteScene(::FS::Bar::Exit);
	Scene::deleteScene<Exit>();
}
