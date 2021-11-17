#include "exit.h"
#include "../../Menu/Project/saveas.h"

FS::Bar::Exit::Exit(
	const FD::WindowRead* const windowRead,
	FD::WindowWrite* const windowWrite,
	const FD::SceneRead* const sceneRead,
	const FD::ProjectRead* const projectRead,
	FD::ExitWrite* const exitWrite,
	const FD::Coding::TabRead* const tabRead,
	FD::Coding::TabWrite* const tabWrite
)
	:
	windowWrite(windowWrite),
	windowRead(windowRead),
	sceneRead(sceneRead),
	projectRead(projectRead),
	exitWrite(exitWrite),
	tabRead(tabRead),
	tabWrite(tabWrite)
{
	GLog.add<FD::Log::Type::None>("Construct Bar::ExitScene.");
}

FS::Bar::Exit::~Exit() noexcept {
	try {
		GLog.add<FD::Log::Type::None>("Destruct Bar::ExitScene.");
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
		GLog.add<FD::Log::Type::None>("Request add Bar::SaveAsScene.");
		Scene::addScene<Bar::SaveAs>();
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
	GLog.add<FD::Log::Type::None>("Request delete Bar::ExitScene.");
	Scene::deleteScene<Exit>();
}
