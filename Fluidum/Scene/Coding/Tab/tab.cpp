#include "tab.h"
#include <nfd.h>
#include "../../Utils/Popup/message.h"

namespace FS::Coding {

	std::vector<std::string> substring(const std::vector<std::string>& pathes) {
		std::vector<std::string> result(pathes.size());
		for (std::size_t i = 0, size = result.size(); i < size; i++) {
			std::filesystem::path path = pathes[i];
			result[i] = path.filename().string();
		}
		return result;
	}

}

FS::Coding::Tab::Tab(
	FD::Coding::TabWrite* const tabWrite,
	const FD::Coding::TabRead* const tabRead,
	const FD::ProjectRead* const projectRead
) : tabWrite(tabWrite), tabRead(tabRead), projectRead(projectRead)
{
	GLog.add<FD::Log::Type::None>("Construct Coding::TabScene.");

	style.topBarSize = { 0.0f,ImGui::CalcTextSize(ICON_MD_FOLDER_OPEN).y * 1.7f };

	this->updateInfo();
}

FS::Coding::Tab::~Tab() noexcept {
	try {
		GLog.add<FD::Log::Type::None>("Destruct Coding::TabScene.");
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

void FS::Coding::Tab::call() {
	using namespace FU::ImGui::Operators;

	this->update();
	this->updateTextSaved();

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 0.0f));

	ImGui::Begin("CodingTab", &windowCloseFlag, ImGuiWindowFlags_NoDocking);
	pos.center = ImGui::GetWindowPos() / 2.3f;

	this->fileList();

	ImGui::End();

	ImGui::PopStyleVar();

	checkWindowShouldClose();
}

void FS::Coding::Tab::checkWindowShouldClose() {
	if (!this->windowCloseFlag)
		return;

	GLog.add<FD::Log::Type::None>("Request delete Coding::Tab.");
	Scene::deleteScene<Tab>();
}

//void FS::Coding::Tab::include() {
//	std::unique_ptr<nfdchar_t*> outPath = std::make_unique<nfdchar_t*>();
//	GLog.add<FD::Log::Type::None>("Open file dialog.");
//	const nfdresult_t result = NFD_OpenDialog(".lua,.py,.as", NULL, outPath.get());
//	if (result == NFD_OKAY) {
//		GLog.add<FD::Log::Type::None>("Selected file path is {}.", *outPath.get());
//	}
//	else if (result == NFD_CANCEL) {
//		GLog.add<FD::Log::Type::None>("Cancel file dialog.");
//		return;
//	}
//	else {//NFD_ERROR
//		GLog.add<FD::Log::Type::Error>("Error file dialog.");
//		throw std::runtime_error("NFD_OpenDialog() return NFD_ERROR.");
//	}
//
//	using enum FD::Coding::TabWrite::Exception;
//	try {
//		tabWrite->addFile(*outPath.get());
//	}
//	catch (const FD::Coding::TabWrite::Exception type) {
//		if (type == AlreadyExist) {
//			GLog.add<FD::Log::Type::None>("Request add Utils::MessageScene.");
//			Scene::addScene<Utils::Message>(text.error_alreadyExist, pos.center);
//		}
//		else if (type == LimitFileSizeMax) {
//			GLog.add<FD::Log::Type::None>("Request add Utils::MessageScene.");
//			Scene::addScene<Utils::Message>(text.error_limitMaxSize, pos.center);
//		}
//		else {
//			GLog.add<FD::Log::Type::Error>("abort() has been called. File {}.", __FILE__);
//			abort();
//		}
//	}
//}

void FS::Coding::Tab::update() {
	if (!tabRead->update())
		return;
	this->updateInfo();
}

void FS::Coding::Tab::updateInfo() {
	std::vector<std::string> paths = tabRead->getFilePathes();
	std::vector<std::string> names = substring(paths);
	info.files.resize(paths.size());
	for (std::size_t i = 0, size = paths.size(); i < size; i++) {
		info.files[i] = { std::move(paths[i]),std::move(names[i]) };
	}
}

void FS::Coding::Tab::updateTextSaved() {
	if (!tabRead->update_isTextSaved())
		return;

	for (auto& x : info.files) {
		if (tabRead->isTextSaved(x.path)) {
			if (x.asterisk)
				x.name.pop_back();
			x.asterisk = false;
		}
		else {
			if (x.asterisk)
				x.name += '*';
			x.asterisk = true;
		}
	}
}

void FS::Coding::Tab::fileList() {
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4());
	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2());
	const ImVec2 size = { ImGui::GetWindowSize().x - (2.0f * ImGui::GetStyle().WindowPadding.x) ,0.0f };

	ImGui::Spacing();

	for (uint16_t i = 0; auto & x : info.files) {
		if (i == select.index) {
			ImGui::PushStyleColor(ImGuiCol_Button, { 0.2f,0.3f,0.9f,0.5f });

			if (ImGui::Button(x.name.c_str(), size)) {
				select.index = i;
				this->display();
			}
			ImGui::PopStyleColor();
		}
		else {
			if (ImGui::Button(x.name.c_str(), size)) {
				select.index = i;
				this->display();
			}
		}
		ImGui::Spacing();
		i++;
	}

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
}

void FS::Coding::Tab::display() {
	const std::string path = info.files.at(select.index).path;

	if (tabRead->getDisplayFilePaths().at(0) == path)
		return;

	GLog.add<FD::Log::Type::None>("Erase display file {}.", path);
	tabWrite->eraseDisplayFile(tabRead->getDisplayFilePaths().at(0));

	GLog.add<FD::Log::Type::None>("Add display file {}.", tabRead->getDisplayFilePaths().at(0));
	tabWrite->addDisplayFile(path);
}

