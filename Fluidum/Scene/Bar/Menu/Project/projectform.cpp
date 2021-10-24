#include "projectform.h"
#include "../../../Utils/Popup/popupselect.h"
#include "../../../Utils/Popup/backwindow.h"
#include "../../../Utils/Popup/message.h"
#include "newproject.h"
#include <nfd.h>

using namespace FU::ImGui::Operators;

FS::Bar::ProjectForm::ProjectForm(
	FD::ProjectWrite* const projectWrite,
	const FD::ProjectRead* const projectRead,
	const FD::GuiRead* const guiRead,
	const std::string& projectType
)
	: projectWrite(projectWrite), projectRead(projectRead), guiRead(guiRead), projectType(projectType)
{
	GLog.add<FD::Log::Type::None>("Construct ProjectFormScene.");

	style.windowPos = guiRead->centerPos() - (guiRead->windowSize() / 3.0f);
	style.windowSize = guiRead->windowSize() - (style.windowPos * 2.0f);


	folderPathStr.reserve(100);
	projectNameStr.reserve(100);

	folderPathStr = std::filesystem::current_path().string();
	folderPathStr += '/';

}

FS::Bar::ProjectForm::~ProjectForm() noexcept {
	try {
		GLog.add<FD::Log::Type::None>("Destruct ProjectFormScene.");

		GLog.add<FD::Log::Type::None>("Request trydelete Utils::MessageScene.");
		if (Scene::tryDeleteScene<Utils::Message>())
			GLog.add<FD::Log::Type::None>("Delete Utils::MessageScene. Scene::tryDelete() == true");
		else
			GLog.add<FD::Log::Type::None>("Utils::MessageScene does not exist. Scene::tryDelete() == false.");
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

void FS::Bar::ProjectForm::call() {

	ImGui::SetNextWindowFocus();
	ImGui::SetNextWindowPos(style.windowPos);
	ImGui::SetNextWindowSize(style.windowSize);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
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

void FS::Bar::ProjectForm::title() {
	ImGui::BeginChild("Title1", ImVec2(style.windowSize.x / 2.0f, style.windowSize.y * 0.07f));
	ImGui::SetWindowFontScale(1.7f);
	ImGui::Text(projectType.c_str());
	ImGui::EndChild();

	ImGui::Separator();

	ImGui::BeginChild("Title2", ImVec2(style.windowSize.x / 2.0f, style.windowSize.y * 0.07f));
	ImGui::SetWindowFontScale(1.3f);

	ImGui::Text(text.fillPlease);

	ImGui::EndChild();
}

void FS::Bar::ProjectForm::folderPath() {
	ImGui::Text(text.folderPath);
	ImGui::InputText("##ppath", folderPathStr.data(), folderPathStr.capacity());
	pos.projectFolder = ImGui::GetItemRectMin();

	ImGui::SameLine();
	if (!ImGui::Button(ICON_MD_FOLDER_OPEN))
		return;

	std::unique_ptr<nfdchar_t*> outPath = std::make_unique<nfdchar_t*>();
	GLog.add<FD::Log::Type::None>("Open file dialog.");
	const nfdresult_t result = NFD_PickFolder(NULL, outPath.get());
	if (result == NFD_OKAY) {
		this->folderPathStr = *outPath.get();
	}
	else if (result == NFD_CANCEL) {
		GLog.add<FD::Log::Type::None>("Cancel file dialog.");
	}
	else {//NFD_ERROR
		GLog.add<FD::Log::Type::Error>("Error file dialog.");
		throw std::runtime_error("NFD_OpenDialog() return NFD_ERROR.");
	}
}

void FS::Bar::ProjectForm::projectName() {
	ImGui::Spacing(); ImGui::Spacing();

	ImGui::Text(text.projectName);
	ImGui::InputText("##pname", projectNameStr.data(), projectNameStr.capacity());
	pos.projectName = ImGui::GetItemRectMin();
}

void FS::Bar::ProjectForm::bottom() {
	ImGui::Dummy({ 0.0f,(style.windowPos.y + style.windowSize.y) - style.innerPosHeight - 50.0f });
	ImGui::BeginChild("Bottom");

	const ImVec2 buttonSize = ImVec2(ImGui::GetWindowSize().x * 0.2f, 0.0f);

	//戻る
	if (ImGui::Button(text.back, buttonSize)) {
		GLog.add<FD::Log::Type::None>("Request delete ProjectFormScene.");
		Scene::deleteScene<ProjectForm>();//シーンを消す
	}

	ImGui::SameLine();

	//キャンセルボタン　赤字
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.05f, 0.05f, 0.5f));
	if (ImGui::Button(text.cancel, buttonSize)) {
		GLog.add<FD::Log::Type::None>("Request delete ProjectFormScene.");
		Scene::deleteScene<ProjectForm>();
		GLog.add<FD::Log::Type::None>("Request delete NewProjectScene.");
		Scene::deleteScene<Bar::NewProject>();
	}
	ImGui::PopStyleColor();

	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.02f, 0.35f, 0.02f, 1.0f));
	if (ImGui::Button(text.create, buttonSize)) {//作成ボタン
		pos.create = ImGui::GetItemRectMin();

		bool success = this->createProject();
		if (success) {
			GLog.add<FD::Log::Type::None>("Request delete ProjectFormScene.");
			Scene::deleteScene<ProjectForm>();
			GLog.add<FD::Log::Type::None>("Request delete NewProjectScene.");
			Scene::deleteScene<Bar::NewProject>();
		}
	}
	ImGui::PopStyleColor();

	ImGui::EndChild();
}

bool FS::Bar::ProjectForm::createProject() {
	FU::Cursor::setCursorType(FU::Cursor::Type::Wait);

	ImGui::GetForegroundDrawList()->AddCircle(ImGui::GetCursorPos(), 5.0f, 0x3399FF);
	//現在のプロジェクトが保存されていない場合は
	//保存して作成　保存せずに作成　キャンセル　
	if (projectRead->isDataChanged()) {
		GLog.add<FD::Log::Type::None>("Request add PopupSelectScene.");
		Scene::addScene<Utils::PopupSelect>(Utils::PopupSelectIconType::Warning, text.notSaved, text.cancel, text.createWithoutSaving, text.saveAndCreate);

		return false;
	}

	//チェック
	if (std::string(folderPathStr.c_str()).empty()) {
		GLog.add<FD::Log::Type::None>("Failed to create new project. Folder path is empty.");
		GLog.add<FD::Log::Type::None>("Request add Utils::MessageScene.");
		Scene::addScene<Utils::Message>(text.error_emptyForm, pos.projectFolder);
		return false;
	}
	if (std::string(projectNameStr.c_str()).empty()) {
		GLog.add<FD::Log::Type::None>("Failed to create new project. Project name is empty.");
		GLog.add<FD::Log::Type::None>("Request add Utils::MessageScene.");
		Scene::addScene<Utils::Message>(text.error_emptyForm, pos.projectName);
		return false;
	}


	FD::Project::CreateProjectInfo info;
	info.projectFolderPath = folderPathStr.c_str();
	info.projectName = projectNameStr.c_str();

	GLog.add<FD::Log::Type::None>("Request create new project. Project name \"{}\". Project folder path \"{}\".", projectNameStr.c_str(), folderPathStr.c_str());
	try {
		projectWrite->createNewProject(info);
	}
	catch (const FD::Project::ExceptionType type) {
		GLog.add<FD::Log::Type::None>("Failed to create new project. Project name \"{}\". Project folder path \"{}\".", projectNameStr.c_str(), folderPathStr.c_str());

		//既に存在している(同じ名前のフォルダが存在する)
		if (type == FD::Project::ExceptionType::AlreadyProjectFolderExist) {
			GLog.add<FD::Log::Type::None>("Request add Utils::MessageScene.");
			Scene::addScene<Utils::Message>(text.error_emptyForm, pos.create);
			return false;
		}
		else {
			GLog.add<FD::Log::Type::Error>("abort() has been called. File {}.", __FILE__);
			abort();
		}
	}
	catch (const std::exception& e) {
		GLog.add<FD::Log::Type::None>(e.what());
		GLog.add<FD::Log::Type::None>("Request add Utils::MessageScene.");
		Scene::addScene<Utils::Message>(text.error_failedToCreate, pos.create);

		return false;
	}

	GLog.add<FD::Log::Type::None>("Create new project. Project name \"{}\". Project folder path \"{}\".", projectNameStr, folderPathStr);
	return true;
}
