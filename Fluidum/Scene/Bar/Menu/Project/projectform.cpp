#include "projectform.h"
#include "../../../Utils/Popup/popupselect.h"
#include "../../../Utils/Popup/backwindow.h"
#include "../../../Title/title.h"
#include "newproject.h"

using namespace FU::ImGui::Operators;

FS::Internal::Bar::ProjectForm::ProjectForm(
	FD::ProjectWrite* const projectWrite,
	const FD::ProjectRead* const projectRead,
	const FD::GuiRead* const guiRead,
	const std::string& projectType
)
	: projectWrite(projectWrite), projectRead(projectRead), guiRead(guiRead), projectType(projectType)
{
	Internal::GLog.add<FD::Log::Type::None>("Construct ProjectFormScene.");


	style.windowPos = guiRead->centerPos() - (guiRead->windowSize() / 3.0f);
	style.windowSize = guiRead->windowSize() - (style.windowPos * 2.0f);


	folderPathStr.reserve(100);
	projectNameStr.reserve(100);

	folderPathStr = std::filesystem::current_path().string();
	folderPathStr += '/';

}

FS::Internal::Bar::ProjectForm::~ProjectForm() noexcept {
	try {
		Internal::GLog.add<FD::Log::Type::None>("Destruct ProjectFormScene.");
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

void FS::Internal::Bar::ProjectForm::call() {

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

	ImGui::PopStyleVar(3);
}

void FS::Internal::Bar::ProjectForm::title() {
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

void FS::Internal::Bar::ProjectForm::folderPath() {
	ImGui::Text(text.folderPath);
	ImGui::InputText("##ppath", folderPathStr.data(), folderPathStr.capacity());

}

void FS::Internal::Bar::ProjectForm::projectName() {
	ImGui::Spacing(); ImGui::Spacing();

	ImGui::Text(text.projectName);
	ImGui::InputText("##pname", projectNameStr.data(), projectNameStr.capacity());
}

void FS::Internal::Bar::ProjectForm::bottom() {
	ImGui::Dummy({ 0.0f,(style.windowPos.y + style.windowSize.y) - style.innerPosHeight - 50.0f });
	ImGui::BeginChild("Bottom");

	const ImVec2 buttonSize = ImVec2(ImGui::GetWindowSize().x * 0.2f, 0.0f);

	//戻る
	if (ImGui::Button(text.back, buttonSize)) {
		Internal::GLog.add<FD::Log::Type::None>("Request delete ProjectFormScene.");
		Scene::deleteScene<ProjectForm>();//シーンを消す
	}

	ImGui::SameLine();

	//キャンセルボタン　赤字
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.05f, 0.05f, 0.5f));
	if (ImGui::Button(text.cancel, buttonSize)) {
		Internal::GLog.add<FD::Log::Type::None>("Request delete ProjectFormScene.");
		Scene::deleteScene<ProjectForm>();
		Internal::GLog.add<FD::Log::Type::None>("Request delete NewProjectScene.");
		Scene::deleteScene<NewProject>();
	}
	ImGui::PopStyleColor();

	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.7f, 0.1f, 1.0f));
	if (ImGui::Button(text.create, buttonSize)) {//作成ボタン
		bool success = this->createProject();
		if (success) {
			Internal::GLog.add<FD::Log::Type::None>("Request delete ProjectFormScene.");
			Scene::deleteScene<ProjectForm>();
			Internal::GLog.add<FD::Log::Type::None>("Request delete NewProjectScene.");
			Scene::deleteScene<NewProject>();
			//TitleSceneで追加されたらTitleを消しておく
			Internal::GLog.add<FD::Log::Type::None>("try delete TitleScene.");
			Scene::tryDeleteScene<Title>();
		}
	}
	ImGui::PopStyleColor();

	ImGui::EndChild();
}

bool FS::Internal::Bar::ProjectForm::createProject() {
	//error表示はまた実装する．

	//現在のプロジェクトが保存されていない場合は
	//保存して作成　保存せずに作成　キャンセル　
	if (projectRead->isDataChanged()) {
		Internal::GLog.add<FD::Log::Type::None>("Request add PopupSelectScene.");
		Scene::addScene<PopupSelect>(Utils::PopupSelectIconType::Warning, text.notSaved, text.cancel, text.createWithoutSaving, text.saveAndCreate);

		return false;
	}

	//チェック
	if (std::string(folderPathStr.c_str()).empty()) {
		Internal::GLog.add<FD::Log::Type::None>("Failed to create new project. Folder path is empty.");
		return false;
	}
	if (std::string(projectNameStr.c_str()).empty()) {
		Internal::GLog.add<FD::Log::Type::None>("Failed to create new project. Project name is empty.");
		return false;
	}


	FD::Project::CreateProjectInfo info;
	info.projectFolderPath = folderPathStr.c_str();
	info.projectName = projectNameStr.c_str();

	Internal::GLog.add<FD::Log::Type::None>("Request create new project. Project name \"{}\". Project folder path \"{}\".", projectNameStr.c_str(), folderPathStr.c_str());
	try {
		projectWrite->createNewProject(info);
	}
	catch (const FD::Project::ExceptionType type) {
		Internal::GLog.add<FD::Log::Type::None>("Failed to create new project. Project name \"{}\". Project folder path \"{}\".", projectNameStr.c_str(), folderPathStr.c_str());

		//既に存在している(同じ名前のフォルダが存在する)
		if (type == FD::Project::ExceptionType::AlreadyProjectFolderExist)
			return false;
		else {
			Internal::GLog.add<FD::Log::Type::Error>("abort() has been called. File {}.", __FILE__);
			abort();
		}
	}

	Internal::GLog.add<FD::Log::Type::None>("Create new project. Project name \"{}\". Project folder path \"{}\".", projectNameStr, folderPathStr);
	return true;
}
