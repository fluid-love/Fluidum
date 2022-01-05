#include "form.h"
#include "../../Utils/Popup/backwindow.h"
#include "../../Utils/Popup/message.h"
#include "new.h"
#include <nfd.h>

using namespace FU::ImGui::Operators;

FS::Project::File::Form::Form(
	FD::ProjectWrite* const projectWrite,
	const FD::ProjectRead* const projectRead,
	const FD::GuiRead* const guiRead,
	std::shared_ptr<SharedInfo>& info
) :
	projectWrite(projectWrite),
	projectRead(projectRead),
	guiRead(guiRead),
	info(info)
{
	FluidumScene_Log_Constructor(::FS::Project::File::Form);

	style.windowPos = guiRead->centerPos() - (guiRead->windowSize() / 3.0f);
	style.windowSize = guiRead->windowSize() - (style.windowPos * 2.0f);

	const auto maxSize = FU::File::maxPathSize();
	str.name.reserve(maxSize);

	str.directoryPath = (std::filesystem::current_path().string() + '/');
	str.directoryPath.reserve(maxSize);

}

FS::Project::File::Form::~Form() noexcept {
	FluidumScene_Log_Destructor(::FS::Project::File::Form);

	FluidumScene_Log_RequestTryDeleteScene(::FS::Utils::Message);
	if (Scene::tryDeleteScene<Utils::Message>())
		GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Delete Utils::MessageScene.");
	else
		GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Utils::MessageScene does not exist. Scene::tryDelete() == false.");
}

void FS::Project::File::Form::call() {

	ImGui::SetNextWindowFocus();
	ImGui::SetNextWindowPos(style.windowPos);
	ImGui::SetNextWindowSize(style.windowSize);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.1f);
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);

	constexpr auto flag =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove;

	//animation
	ImAnime::PushStyleVar(anime.counter, 0.5f, 0.0f, 1.0f, ImAnimeType::LINEAR, ImGuiStyleVar_Alpha);

	ImGui::Begin("ProjectForm", nullptr, flag);

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

	ImAnime::PopStyleVar();
	ImGui::PopStyleVar(3);
}

void FS::Project::File::Form::title() {
	ImGui::BeginChild("Title1", ImVec2(style.windowSize.x / 2.0f, style.windowSize.y * 0.07f));
	ImGui::SetWindowFontScale(1.7f);
	ImGui::Text(info->name.c_str());
	ImGui::EndChild();

	ImGui::Separator();

	ImGui::BeginChild("Title2", ImVec2(style.windowSize.x / 2.0f, style.windowSize.y * 0.07f));
	ImGui::SetWindowFontScale(1.3f);

	ImGui::Text(text.fillPlease);

	ImGui::EndChild();
}

void FS::Project::File::Form::folderPath() {
	ImGui::BulletText(text.folderPath);
	ImGui::InputText("##ppath", str.directoryPath.data(), str.directoryPath.capacity());
	pos.projectFolder = ImGui::GetItemRectMin();

	ImGui::SameLine();
	if (!ImGui::Button(ICON_MD_FOLDER_OPEN))
		return;

	std::unique_ptr<nfdchar_t*> outPath = std::make_unique<nfdchar_t*>();
	GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Open file dialog.");
	const nfdresult_t result = NFD_PickFolder(NULL, outPath.get());
	if (result == NFD_OKAY) {
		this->str.directoryPath = *outPath.get();
	}
	else if (result == NFD_CANCEL) {
		GLog.add_str<FU::Log::Type::None>("Cancel file dialog.");
	}
	else {//NFD_ERROR
		GLog.add_str<FU::Log::Type::Warning>("Error file dialog.");
	}
}

void FS::Project::File::Form::projectName() {
	ImGui::Spacing(); ImGui::Spacing();

	ImGui::BulletText(text.projectName);
	ImGui::InputText("##pname", str.name.data(), str.name.capacity());
	pos.projectName = ImGui::GetItemRectMin();
}

void FS::Project::File::Form::bottom() {
	ImGui::Dummy({ 0.0f,(style.windowPos.y + style.windowSize.y) - style.innerPosHeight - 50.0f });
	ImGui::BeginChild("Bottom");

	const ImVec2 buttonSize = ImVec2(ImGui::GetWindowSize().x * 0.2f, 0.0f);

	//back
	if (ImGui::Button(text.back, buttonSize)) {
		FluidumScene_Log_RequestDeleteScene(::FS::Project::File::Form);
		Scene::deleteScene<Form>();
	}

	ImGui::SameLine();

	//cancel
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.05f, 0.05f, 0.5f));
	if (ImGui::Button(text.cancel, buttonSize)) {
		FluidumScene_Log_RequestDeleteScene(::FS::Project::File::Form);
		Scene::deleteScene<Form>();
		FluidumScene_Log_RequestDeleteScene(::FS::Project::File::New);
		Scene::deleteScene<New>();
	}
	ImGui::PopStyleColor();

	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.02f, 0.35f, 0.02f, 1.0f));
	if (ImGui::Button(text.create, buttonSize)) {//make
		pos.create = ImGui::GetItemRectMin();

		bool success = this->createProject();
		if (success) {
			info->create = true;
			FluidumScene_Log_RequestDeleteScene(::FS::Project::File::Form);
			Scene::deleteScene<Form>();
		}
	}
	ImGui::PopStyleColor();

	ImGui::EndChild();
}

bool FS::Project::File::Form::createProject() {
	FU::Cursor::setCursorType(FU::Cursor::Type::Wait);

	const std::string name = str.name.c_str();
	const std::string directoryPath = str.directoryPath.c_str();
	const std::string fullPath = directoryPath + name;

	ImGui::GetForegroundDrawList()->AddCircle(ImGui::GetCursorPos(), 5.0f, 0x3399FF);
	//The current project is not saved.
	//save / ignore / cancelÅ@
//FU::MB

	//check
	if (directoryPath.empty()) {
		FluidumScene_Log_RequestDeleteScene(::FS::Utils::Message);
		Scene::addScene<Utils::Message>(text.error_emptyForm, pos.projectFolder);
		return false;
	}
	if (name.empty()) {
		FluidumScene_Log_RequestDeleteScene(::FS::Utils::Message);
		Scene::addScene<Utils::Message>(text.error_emptyForm, pos.projectName);
		return false;
	}

	//max size
	if (fullPath.size() >= FU::File::maxPathSize()) {
		FluidumScene_Log_RequestDeleteScene(::FS::Utils::Message);
		Scene::addScene<Utils::Message>(text.error_maxSize, pos.create);
		return false;
	}

	FD::ProjectWrite::CreateInfo info{
		.projectName = name,
		.projectDirectoryPath = directoryPath,
	};

	GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "[Request] Create new project. DirectoryPath:({}). Name:({}).", directoryPath, name);
	try {
		//strong
		projectWrite->createNewProject(info);
	}
	catch (const FD::ProjectWrite::Exception type) {

		//already exist.
		if (type == FD::ProjectWrite::Exception::AlreadyProjectDirctoryExist) {
			FluidumScene_Log_RequestDeleteScene(::FS::Utils::Message);
			Scene::addScene<Utils::Message>(text.error_emptyForm, pos.create);
		}
		else {
			FluidumScene_Log_InternalWarning();
		}
		return false;
	}
	catch (const std::exception& e) {
		FluidumScene_Log_StdExceptionError(e);
		FluidumScene_Log_RequestDeleteScene(::FS::Utils::Message);
		Scene::addScene<Utils::Message>(text.error_failedToCreate, pos.create);

		return false;
	}
	catch (...) {//unexpected
		FluidumScene_Log_InternalWarning();
		return false;
	}

	GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Successfully created project. DirectoryPath:({}). Name:({}).", directoryPath, name);
	return true;
}
