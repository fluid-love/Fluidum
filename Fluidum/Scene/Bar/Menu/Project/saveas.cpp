#include "saveas.h"
#include "../../../Utils/Popup/backwindow.h"
#include "../../../Utils/Popup/Message.h"
#include "../../../Utils/Popup/popupselect.h"
#include <nfd.h>

using namespace FU::ImGui::Operators;

FS::Bar::SaveAs::SaveAs(
	FD::ProjectWrite* const projectWrite,
	const FD::ProjectRead* const projectRead,
	const FD::GuiRead* const guiRead
) : projectWrite(projectWrite), projectRead(projectRead), guiRead(guiRead)
{
	GLog.add<FD::Log::Type::None>("Construct Bar::SaveAsScene.");


	style.windowPos = guiRead->centerPos() - (guiRead->windowSize() / 3.0f);
	style.windowSize = guiRead->windowSize() - (style.windowPos * 2.0f);

	folderPathStr.reserve(100);
	projectNameStr.reserve(100);

	GLog.add<FD::Log::Type::None>("Request add PopupBackWindowScene.");
	Scene::addScene<PopupBackWindow>();
}

FS::Bar::SaveAs::~SaveAs() noexcept {
	try {
		GLog.add<FD::Log::Type::None>("Request delete PopupBackWindowScene.");
		Scene::deleteScene<PopupBackWindow>();

		GLog.add<FD::Log::Type::None>("Destruct Bar::SaveAsScene.");
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

void FS::Bar::SaveAs::call() {

	ImGui::SetNextWindowFocus();
	ImGui::SetNextWindowPos(style.windowPos);
	ImGui::SetNextWindowSize(style.windowSize);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);

	constexpr auto flag =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoSavedSettings
		;


	ImGui::Begin("SaveAs", nullptr, flag);

	this->title();

	ImGui::Spacing(); ImGui::Spacing();

	ImGui::BeginChild("Inner", { 0.0f,style.windowSize.y * 0.5f });
	ImGui::PushItemWidth(style.windowSize.x * 0.4f);
	this->folderPath();
	this->projectName();
	ImGui::PopItemWidth();
	style.innerPosHeight = ImGui::GetWindowPos().y + ImGui::GetWindowSize().y;
	ImGui::EndChild();

	this->bottom();

	ImGui::End();

	ImGui::PopStyleVar(3);
}

void FS::Bar::SaveAs::title() {
	ImGui::BeginChild("Title1", ImVec2(style.windowSize.x / 2.0f, style.windowSize.y * 0.07f));
	ImGui::SetWindowFontScale(1.7f);
	ImGui::Text(text.title);
	ImGui::EndChild();

	ImGui::Separator();
}

void FS::Bar::SaveAs::folderPath() {
	ImGui::Text(text.folderPath);
	bool input = ImGui::InputText("##ppath", folderPathStr.data(), folderPathStr.capacity());
	pos.projectFolder = ImGui::GetItemRectMin();

	ImGui::SameLine();
	if (!ImGui::Button(ICON_MD_FOLDER_OPEN))
		return;

	std::unique_ptr<nfdchar_t*> outPath = std::make_unique<nfdchar_t*>();
	GLog.add<FD::Log::Type::None>("Open file dialog.");
	const nfdresult_t result = NFD_PickFolder(nullptr, outPath.get());
	if (result == NFD_OKAY) {
		GLog.add<FD::Log::Type::None>("Pick folder({}).", *outPath.get());
		folderPathStr = *outPath.get();
	}
	else if (result == NFD_CANCEL) {
		GLog.add<FD::Log::Type::None>("NFD_OpenDialog() return NFD_CANCEL.");
	}
	else {//NFD_ERROR
		GLog.add<FD::Log::Type::Error>("Error file dialog.");
		throw std::runtime_error("NFD_OpenDialog() return NFD_ERROR.");
	}
}

void FS::Bar::SaveAs::projectName() {
	ImGui::Spacing(); ImGui::Spacing();

	ImGui::Text(text.projectName);
	ImGui::InputText("##pname", projectNameStr.data(), projectNameStr.capacity());
	pos.projectName = ImGui::GetItemRectMin();
}

void FS::Bar::SaveAs::bottom() {
	ImGui::Dummy({ 0.0f,(style.windowPos.y + style.windowSize.y) - style.innerPosHeight - 50.0f });
	ImGui::BeginChild("Bottom");

	const ImVec2 buttonSize = ImVec2(ImGui::GetWindowSize().x * 0.2f, 0.0f);

	//cancel
	if (ImGui::Button(text.cancel, buttonSize)) {
		GLog.add<FD::Log::Type::None>("Request delete Bar::SaveAsScene.");
		Scene::deleteScene<SaveAs>();
	}

	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.02f, 0.35f, 0.02f, 1.0f));
	if (ImGui::Button(text.save, buttonSize)) {//save
		pos.save = ImGui::GetItemRectMin();

		bool success = this->save();
		if (success) {
			GLog.add<FD::Log::Type::None>("Request delete Bar::SaveAsScene.");
			Scene::deleteScene<SaveAs>();
		}
	}
	ImGui::PopStyleColor();

	ImGui::EndChild();
}

bool FS::Bar::SaveAs::save() {

	if (projectRead->isDataChanged()) {
		GLog.add<FD::Log::Type::None>("Request add PopupSelectScene.");
		Scene::addScene<Utils::PopupSelect>(Utils::PopupSelectIconType::Warning, text.checkCurrentProject, text.cancel, text.ignore, text.saveAndWrite);
		return false;
	}

	//is empty
	if (std::string(folderPathStr.c_str()).empty()) {
		GLog.add<FD::Log::Type::None>("Failed to save project. Folder path is empty.");
		GLog.add<FD::Log::Type::None>("Request add Utils::MessageScene.");
		Scene::addScene<Utils::Message>(text.error_empty, pos.projectFolder);
		return false;
	}
	if (std::string(projectNameStr.c_str()).empty()) {
		GLog.add<FD::Log::Type::None>("Failed to create new project. Project name is empty.");
		GLog.add<FD::Log::Type::None>("Request add Utils::MessageScene.");
		Scene::addScene<Utils::Message>(text.error_empty, pos.projectName);
		return false;
	}

	GLog.add<FD::Log::Type::None>("Request save project. Project name \"{}\". Project folder path \"{}\".", projectNameStr.c_str(), folderPathStr.c_str());
	try {
		projectWrite->saveAs(projectNameStr.c_str(), projectNameStr.c_str());
	}
	catch (const FD::Project::ExceptionType type) {
		GLog.add<FD::Log::Type::None>("Failed to create new project. Project name \"{}\". Project folder path \"{}\".", projectNameStr.c_str(), folderPathStr.c_str());

		//既に存在している(同じ名前のフォルダが存在する)
		if (type == FD::Project::ExceptionType::AlreadyProjectFolderExist) {
			GLog.add<FD::Log::Type::None>("Request add Utils::MessageScene.");
			Scene::addScene<Utils::Message>(text.error_alreadyExist, pos.projectFolder);
			return false;
		}
		else if (type == FD::Project::ExceptionType::NotFoundProjectFolder) {
			GLog.add<FD::Log::Type::None>("Request add Utils::MessageScene.");
			Scene::addScene<Utils::Message>(text.error_notFound, pos.projectFolder);
			return false;
		}
		else {
			GLog.add<FD::Log::Type::Error>("abort() has been called. File {}.", __FILE__);
			abort();
		}
	}
	catch (const std::exception& e) {
		GLog.add<FD::Log::Type::None>("Request add Utils::MessageScene.");
		Scene::addScene<Utils::Message>(text.error_failed, pos.save);
	}

	GLog.add<FD::Log::Type::None>("Project has been saved.");
	return true;
}


