#include "saveas.h"
#include "../../Utils/Popup/backwindow.h"
#include "../../Utils/Popup/Message.h"
#include <nfd.h>

using namespace FU::ImGui::Operators;

FS::Project::File::SaveAs::SaveAs(
	FD::ProjectWrite* const projectWrite,
	const FD::ProjectRead* const projectRead,
	const FD::GuiRead* const guiRead,
	const FD::ExitRead* const exitRead,
	FD::WindowWrite* const windowWrite
) :
	projectWrite(projectWrite),
	projectRead(projectRead),
	guiRead(guiRead),
	windowWrite(windowWrite),
	exit(exitRead->saveAsAndExit())
{
	FluidumScene_Log_Constructor(::FS::Project::File::SaveAs);

	style.windowPos = guiRead->centerPos() - (guiRead->windowSize() / 3.0f);
	style.windowSize = guiRead->windowSize() - (style.windowPos * 2.0f);

	{
		const Size max = FU::File::maxPathSize();
		str.directoryPath.reserve(max);
		str.projectName.reserve(max);
	}

	FluidumScene_Log_RequestAddScene(::FS::Utils::PopupBackWindow);
	Scene::addScene<Utils::PopupBackWindow>();
}

FS::Project::File::SaveAs::~SaveAs() noexcept {
	FluidumScene_Log_RequestDeleteScene(::FS::Utils::PopupBackWindow);
	Scene::deleteScene<Utils::PopupBackWindow>();

	FluidumScene_Log_Destructor(::FS::Project::File::SaveAs);
}

void FS::Project::File::SaveAs::call() {

	ImGui::SetNextWindowFocus();
	ImGui::SetNextWindowPos(style.windowPos);
	ImGui::SetNextWindowSize(style.windowSize);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);

	constexpr auto windowFlags = FD::Style::VarRead::PopupWindowFlags();

	ImGui::Begin("SaveAs", nullptr, windowFlags);

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

void FS::Project::File::SaveAs::title() {
	ImGui::BeginChild("Title1", ImVec2(style.windowSize.x / 2.0f, style.windowSize.y * 0.07f));
	ImGui::SetWindowFontScale(1.7f);
	ImGui::Text(text.title);
	ImGui::EndChild();

	ImGui::Separator();
}

void FS::Project::File::SaveAs::folderPath() {
	ImGui::Text(text.directoryPath);
	bool input = ImGui::InputText("##ppath", str.directoryPath.data(), str.directoryPath.capacity());
	pos.projectFolder = ImGui::GetItemRectMin();

	ImGui::SameLine();
	if (!ImGui::Button(ICON_MD_FOLDER_OPEN))
		return;

	std::unique_ptr<nfdchar_t*> outPath = std::make_unique<nfdchar_t*>();
	GLog.add_str<FU::Log::Type::None>("Open file dialog.");
	const nfdresult_t result = NFD_PickFolder(nullptr, outPath.get());
	if (result == NFD_OKAY) {
		GLog.add_str<FU::Log::Type::None>("Pick folder({}).", *outPath.get());
		str.directoryPath = *outPath.get();
	}
	else if (result == NFD_CANCEL) {
		GLog.add_str<FU::Log::Type::None>("NFD_OpenDialog() return NFD_CANCEL.");
	}
	else {//NFD_ERROR
		GLog.add_str<FU::Log::Type::Warning>("Error file dialog.");
		throw std::runtime_error("NFD_OpenDialog() return NFD_ERROR.");
	}
}

void FS::Project::File::SaveAs::projectName() {
	ImGui::Spacing(); ImGui::Spacing();

	ImGui::Text(text.projectName);
	ImGui::InputText("##pname", str.projectName.data(), str.projectName.capacity());
	pos.projectName = ImGui::GetItemRectMin();
}

void FS::Project::File::SaveAs::bottom() {
	ImGui::Dummy({ 0.0f,(style.windowPos.y + style.windowSize.y) - style.innerPosHeight - 50.0f });
	ImGui::BeginChild("Bottom");

	const ImVec2 buttonSize = ImVec2(ImGui::GetWindowSize().x * 0.2f, 0.0f);

	//cancel
	if (ImGui::Button(text.cancel, buttonSize)) {
		FluidumScene_Log_RequestDeleteScene(::FS::Project::File::SaveAs);
		Scene::deleteScene<SaveAs>();
	}

	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.02f, 0.35f, 0.02f, 1.0f));
	if (ImGui::Button(text.save, buttonSize)) {//save
		pos.save = ImGui::GetItemRectMin();

		bool success = this->save();
		if (success) {
			FluidumScene_Log_RequestDeleteScene(::FS::Project::File::SaveAs);
			Scene::deleteScene<SaveAs>();
		}
	}
	ImGui::PopStyleColor();

	ImGui::EndChild();
}

bool FS::Project::File::SaveAs::save() {

	if (projectRead->isDataChanged()) {
		//Current project is not saved.

		//save ignore cancel
		const auto ret = FU::MB::button_button_cancel(
			FU::MB::Icon::Warning,
			text.confirm_notSaved,
			text.confirm_save,
			text.confirm_ignore
		);
		return false;
	}

	const std::string directoryPath = str.directoryPath.c_str();
	const std::string projectName = str.projectName.c_str();

	//empty
	if (directoryPath.empty()) {
		GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Failed to save project. Folder path is empty.");
		FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
		return false;
	}
	if (projectName.empty()) {
		GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Failed to save new project. Project name is empty.");
		FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
		Scene::addScene<Utils::Message>(text.error_empty, pos.projectName);
		return false;
	}

	GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "[Request] Save project({Path:{}, Name:{}}).", directoryPath, projectName);
	try {
		projectWrite->saveAs(directoryPath, projectName);
	}
	catch (const FD::Project::ExceptionType type) {
		GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Failed to create new project.");

		if (type == FD::Project::ExceptionType::AlreadyProjectFolderExist) {
			FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
			Scene::addScene<Utils::Message>(text.error_alreadyExist, pos.projectFolder);
			return false;
		}
		else if (type == FD::Project::ExceptionType::NotFoundProjectFolder) {
			FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
			Scene::addScene<Utils::Message>(text.error_notFound, pos.projectFolder);
			return false;
		}
		else {
			FluidumScene_Log_InternalError();
			return false;
		}
	}
	catch (const std::exception& e) {
		GLog.add<FU::Log::Type::Warning>(__FILE__, __LINE__, "Catch std::exception({}).", e.what());

		FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
		Scene::addScene<Utils::Message>(text.error_unexpected, pos.save);
	}

	GLog.add_str<FU::Log::Type::None>("The project has been saved successfully.");

	this->checkExit();

	return true;
}

void FS::Project::File::SaveAs::checkExit() {
	if (!this->exit)
		return;

	GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "[Request] Terminate.");
	*windowWrite->getCloseFlag() = true;
}