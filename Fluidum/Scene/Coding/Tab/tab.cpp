#include "tab.h"
#include <nfd.h>
#include "../../Utils/Popup/message.h"

namespace FS::Coding {

	std::vector<std::string> substring(const std::vector<std::string>& pathes) {
		std::vector<std::string> result(pathes.size());
		for (std::size_t i = 0, size = result.size(); i < size; i++) {
			auto itr = std::find(pathes[i].rbegin(), pathes[i].rend(), '/');
			assert(itr != pathes[i].rend() && itr != pathes[i].rbegin());
			auto distance = std::distance(pathes[i].rbegin(), itr);
			result[i].resize(distance);
			std::copy(pathes[i].rbegin(), itr, result[i].rbegin());
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

	files.pathes = tabRead->getFilePathes();
	files.fileNames = substring(files.pathes);
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

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 0.0f));

	ImGui::Begin("CodingTab", &windowCloseFlag);
	pos.center = ImGui::GetWindowPos() / 2.3f;

	this->topBar();
	this->fileList();

	ImGui::End();

	ImGui::PopStyleVar();

	checkWindowShouldClose();
}

void FS::Coding::Tab::checkWindowShouldClose() {
	if (this->windowCloseFlag) {
		GLog.add<FD::Log::Type::None>("Request delete Coding::Tab.");
		Scene::deleteScene<Tab>();
	}
}

void FS::Coding::Tab::topBar() {
	ImGui::Separator();
	ImGui::BeginChild("TabChild", style.topBarSize);

	//open
	if (ImGui::Button(ICON_MD_FOLDER_OPEN))
		this->include();

	ImGui::SameLine();

	if (ImGui::Button(ICON_MD_NOTE_ADD))
		this->create();

	ImGui::SameLine();
	ImGui::Spacing();
	ImGui::SameLine();

	if (ImGui::Button(ICON_MD_SAVE))
		this->save();
	ImGui::SameLine();

	if (ImGui::Button(ICON_MD_SYNC))
		this->sync();
	ImGui::SameLine();

	if (ImGui::Button(ICON_MD_CODE))
		this->code();

	ImGui::EndChild();

	ImGui::Separator();
	ImGui::Spacing();
}

void FS::Coding::Tab::include() {
	std::unique_ptr<nfdchar_t*> outPath = std::make_unique<nfdchar_t*>();
	GLog.add<FD::Log::Type::None>("Open file dialog.");
	const nfdresult_t result = NFD_OpenDialog(".lua,.py,.as", NULL, outPath.get());
	if (result == NFD_OKAY) {
		GLog.add<FD::Log::Type::None>("Selected file path is {}.", *outPath.get());
	}
	else if (result == NFD_CANCEL) {
		GLog.add<FD::Log::Type::None>("Cancel file dialog.");
		return;
	}
	else {//NFD_ERROR
		GLog.add<FD::Log::Type::Error>("Error file dialog.");
		throw std::runtime_error("NFD_OpenDialog() return NFD_ERROR.");
	}

	using enum FD::Coding::TabWrite::Exception;
	try {
		tabWrite->addFile(*outPath.get());
	}
	catch (const FD::Coding::TabWrite::Exception type) {
		if (type == AlreadyExist) {
			GLog.add<FD::Log::Type::None>("Request add Utils::MessageScene.");
			Scene::addScene<Utils::Message>(text.error_alreadyExist, pos.center);
		}
		else if (type == LimitFileSizeMax) {
			GLog.add<FD::Log::Type::None>("Request add Utils::MessageScene.");
			Scene::addScene<Utils::Message>(text.error_limitMaxSize, pos.center);
		}
		else {
			GLog.add<FD::Log::Type::Error>("abort() has been called. File {}.", __FILE__);
			abort();
		}
	}
}

void FS::Coding::Tab::create() {

}

void FS::Coding::Tab::sync() {

}

void FS::Coding::Tab::code() {

}

void FS::Coding::Tab::save() {

}

void FS::Coding::Tab::update() {
	if (!tabRead->update())
		return;


}

void FS::Coding::Tab::fileList() {
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4());
	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2());
	const ImVec2 size = { ImGui::GetWindowSize().x - (2.0f * ImGui::GetStyle().WindowPadding.x) ,0.0f };

	for (auto& x : this->files.fileNames) {
		ImGui::Button(x.c_str(), size);
	}

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
}


