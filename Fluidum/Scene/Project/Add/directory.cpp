#include "directory.h"
#include "../../Utils/Popup/message.h"
#include "select.h"

FS::Project::Add::Directory::Directory(
	const FD::GuiRead* const guiRead,
	const FD::SceneRead* const sceneRead,
	std::shared_ptr<SharedInfo>& sharedInfo
) :
	sceneRead(sceneRead),
	sharedInfo(sharedInfo)
{
	FluidumScene_Log_Constructor("Project::Add::Directory");

	assert(sharedInfo);

	using namespace FU::ImGui::Operators;

	style.windowPos = guiRead->centerPos() - (guiRead->windowSize() / 3.0f);
	style.windowSize = guiRead->windowSize() - (style.windowPos * 2.0f);

	str.parent = sharedInfo->path;

	str.name.reserve(100);
}

FS::Project::Add::Directory::~Directory() {
	FluidumScene_Log_Destructor_("Project::Add::Directory");
}

void FS::Project::Add::Directory::call() {

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
		ImGuiWindowFlags_NoSavedSettings;


	ImAnime::PushStyleVar(anime.counter, 0.5f, 0.0f, 1.0f, ImAnimeType::LINEAR, ImGuiStyleVar_Alpha);

	ImGui::Begin("Add::Dir", nullptr, flag);

	this->title();

	ImGui::Spacing(); ImGui::Spacing();

	ImGui::BeginChild("Inner", { 0.0f,style.windowSize.y * 0.5f });
	ImGui::PushItemWidth(style.windowSize.x * 0.4f);

	ImGui::Spacing(); ImGui::Spacing();
	this->parent();
	ImGui::Spacing(); ImGui::Spacing();
	this->name();

	ImGui::PopItemWidth();
	style.innerPosHeight = ImGui::GetWindowPos().y + ImGui::GetWindowSize().y;
	ImGui::EndChild();

	this->bottom();

	ImGui::End();

	ImAnime::PopStyleVar();
	ImGui::PopStyleVar(3);

}

void FS::Project::Add::Directory::title() {
	ImGui::BeginChild("DirTitle", ImVec2(style.windowSize.x / 2.0f, style.windowSize.y * 0.07f));
	ImGui::SetWindowFontScale(1.7f);

	ImGui::Text(text.title);

	ImGui::EndChild();
}

void FS::Project::Add::Directory::parent() {
	ImGui::BulletText(text.parent);
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
	ImGui::InputText("##Parent", str.parent.data(), str.parent.capacity(), ImGuiInputTextFlags_ReadOnly);
	ImGui::PopStyleVar();
}

void FS::Project::Add::Directory::name() {
	ImGui::BulletText(text.name);
	ImGui::InputText("##DirName", str.name.data(), str.name.capacity());
	pos.name = ImGui::GetItemRectMin();
}

void FS::Project::Add::Directory::bottom() {
	ImGui::Dummy({ 0.0f,(style.windowPos.y + style.windowSize.y) - style.innerPosHeight - 50.0f });
	ImGui::BeginChild("Bottom");

	const ImVec2 buttonSize = ImVec2(ImGui::GetWindowSize().x * 0.2f, 0.0f);

	//back
	if (ImGui::Button(text.back, buttonSize)) {
		sharedInfo->create = false;
		this->deleteThisScene();
	}

	ImGui::SameLine();

	//cancel
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.05f, 0.05f, 0.5f));
	if (ImGui::Button(text.cancel, buttonSize)) {
		sharedInfo->create = false;
		this->deleteThisScene();
		if (sceneRead->exist<Select>()) {
			FluidumScene_Log_RequestDeleteScene("Project::Add::Select");
			Scene::deleteScene<Select>();
		}
	}
	ImGui::PopStyleColor();

	ImGui::SameLine();

	//create
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.02f, 0.35f, 0.02f, 1.0f));
	if (ImGui::Button(text.create, buttonSize)) {
		pos.create = ImGui::GetItemRectMin();
		this->create();
	}
	ImGui::PopStyleColor();

	ImGui::EndChild();
}

void FS::Project::Add::Directory::deleteThisScene() {
	FluidumScene_Log_RequestDeleteScene("Project::Add::Directory");
	Scene::deleteScene<Directory>();
}

void FS::Project::Add::Directory::create() {
	GLog.add<FD::Log::Type::None>("[Project::Add::Directory] Create directory.");

	const std::string typedName = str.name.c_str();
	std::string path = str.parent + typedName;

	if (typedName.empty()) {
		GLog.add<FD::Log::Type::None>("[Project::Add::Directory] Directory name id empty.");
		FluidumScene_Log_RequestAddScene("Utils::Message");
		Scene::addScene<Utils::Message>(text.error_fill, pos.name);
		return;
	}

	if (std::filesystem::exists(path)) {
		GLog.add<FD::Log::Type::None>("[Project::Add::Directory] Directory path({}) already exists.", path);
		FluidumScene_Log_RequestAddScene("Utils::Message");
		Scene::addScene<Utils::Message>(text.error_alreadyExist, pos.create);
		return;
	}

	if (FU::File::containForbiddenCharactor(typedName)) {
		GLog.add<FD::Log::Type::None>("[Project::Add::Directory] Directoryname({}) contains forbidden character.", typedName);
		FluidumScene_Log_RequestAddScene("Utils::Message");
		Scene::addScene<Utils::Message>(text.error_forbiddenCharactor, pos.name);
		return;
	}

	if (!std::filesystem::create_directory(path)) {
		GLog.add<FD::Log::Type::None>("[Project::Add::Directory] Failed to create directory.");
		FluidumScene_Log_RequestAddScene("Utils::Message");
		Scene::addScene<Utils::Message>(text.error_unexpected, pos.create);
		return;
	}


	//info
	{
		sharedInfo->type = SharedInfo::Type::Directory;
		sharedInfo->path = std::move(path);
		sharedInfo->create = true;
	}

	this->deleteThisScene();
	if (sceneRead->exist<Select>()) {
		FluidumScene_Log_RequestDeleteScene("Project::Add::Select");
		Scene::deleteScene<Select>();
	}
}



