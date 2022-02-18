#include "title.h"

//bar
#include "../Bar/Left/leftbar.h"
#include "../Bar/Top/topbar.h"
#include "../Bar/Menu/menubar.h"
#include "../Bar/Status/statusbar.h"
#include "../Bar/Title/titlebar.h"

//layout
#include "../Layout/layout.h"

//project
#include "../Project/File/new.h"

//utils
#include "../Utils/Popup/message.h"
#include "../Utils/Scene/add.h"

//file dialog
#include <nfd.h>

namespace FS::Internal {

	[[nodiscard]] std::vector<FDR::ImGuiImage> createImages() {

		constexpr const char* imageFileNames[] = {
			"coding.png",
			"node.png",
			"analysis.png",
			"genome.png",
			"animation.png",
			"project.png",
			"console.png",
			"plot.png",
			"func.png",
		};

		std::vector<FDR::ImGuiImage> result{};

		for (UIF16 i = 0; i < std::extent_v<decltype(imageFileNames), 0>; i++) {
			std::string path = Resource::LeftBarIconsFilePath;
			path += imageFileNames[i];

			result.emplace_back(FDR::createImGuiImage(path.c_str()));
		}

		return result;
	}

}

void FS::Title::call() {
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {

		std::cout << "a";
	}
	std::call_once(this->flag.once, &Title::writeGuiData, this);

	//if IsMouseButton -> next scene
	this->changeScene();

	this->drawTitleImage();
	this->selectProject();
	this->recentProjectPopup();
	this->recentProjectButtonPopup();
}

FS::Title::Title(
	FD::ProjectWrite* const projectWrite,
	const FD::ProjectRead* const projectRead,
	const FD::SceneRead* const sceneRead,
	FD::GuiWrite* const guiWrite,
	const FD::GuiRead* const guiRead,
	const FD::Style::VarRead* const varRead,
	FD::Style::ThemeWrite* const themeWrite
) :
	projectWrite(projectWrite),
	projectRead(projectRead),
	sceneRead(sceneRead),
	guiWrite(guiWrite),
	guiRead(guiRead),
	varRead(varRead),
	themeWrite(themeWrite),

	recentProjectInfo(getProjectHistory())
{
	FluidumScene_Log_Constructor(::FS::Title);
}

FS::Title::~Title() noexcept {
	FluidumScene_Log_Destructor(::FS::Title);
}

std::array<FD::ProjectRead::HistoryInfo, FD::Project::Limits::HistoryLogMax> FS::Title::getProjectHistory() noexcept {
	try {
		return projectRead->loadProjectHistory();
	}
	catch (...) {
		flag.projectHistoryError = true;
	}
	return {};
}

//call once
void FS::Title::writeGuiData() {

	//operators
	using namespace FU::ImGui::Operators;

	//ImGui Theme
	themeWrite->setTheme();

	const auto [width, height] = FDR::getWindowSize();
	const ImVec2 size = { static_cast<float>(width),static_cast<float>(height) };

	guiWrite->windowSize(size);

	ImVec2 centerPos = size / 2.0f;
	guiWrite->centerPos(centerPos);

	this->style.windowPos = centerPos - (style.imageHalfSize / 2.0f);
	this->style.windowPos.y -= style.imageHalfSize.y / 10.0f;

	this->image = FDR::createImGuiImage(Resource::TitleImageFilePath);
	this->projectIcon = FDR::createImGuiImage(Resource::TitleBarIconFilePath);

	this->leftBarImages = Internal::createImages();


	style.selectWindowPos = { style.windowPos.x,style.windowPos.y + (style.imageHalfSize.y) };
	style.selectWindowSize = { style.imageHalfSize.x,style.imageHalfSize.y / 3.0f };

	pos.recentWindow = style.selectWindowPos + (style.selectWindowSize / 2.0f);

	//failed to load history
	if (flag.projectHistoryError) {
		FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
		Scene::addScene<Utils::Message>(text.error_loadProjectHistory, pos.recentWindow);
	}
}

void FS::Title::changeScene() {

	const bool hovered = flag.isSelectProjectHovered;
	const bool clicked = ImGui::IsMouseClicked(ImGuiMouseButton_Left);
	const bool exists = sceneRead->exist<Project::File::New>();
	const bool defaultProject = projectRead->isDefaultProject();
	const bool anyPopup = ImGui::IsPopupOpen("", ImGuiPopupFlags_AnyPopup);

	const bool change = ((!hovered && clicked && !exists && !anyPopup) || !defaultProject);

	if (!change)
		return;

	FluidumScene_Log_RequestDeleteScene(::FS::Title);
	Scene::deleteScene<Title>();//delete this

	//Bar scenes
	FluidumScene_Log_RequestAddScene(::FS::MenuBar);
	Scene::addScene<MenuBar>();
	FluidumScene_Log_RequestAddScene(::FS::MenuBar);
	Scene::addScene<StatusBar>();
	FluidumScene_Log_RequestAddScene(::FS::TopBar);
	Scene::addScene<TopBar>();
	FluidumScene_Log_RequestAddScene(::FS::LeftBar);
	Scene::addScene<LeftBar>(std::move(this->leftBarImages.value()));
	FluidumScene_Log_RequestAddScene(::FS::TitleBar);
	Scene::addScene<TitleBar>();

	//Layout scene
	FluidumScene_Log_RequestAddScene(::FS::Layout);
	Scene::addScene<Layout>();

	const auto codes = projectRead->loadSceneFile();
	FluidumScene_Log_RequestAddScene(::FS::Utils::AddScenes);
	Scene::addScene<Utils::AddScenes>(codes);

}

void FS::Title::drawTitleImage() {

	constexpr ImGuiWindowFlags windowFlags = FD::Style::VarRead::PopupWindowFlags() | ImGuiWindowFlags_NoBackground;

	//Padding 0
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());

	ImGui::SetNextWindowPos(this->style.windowPos);
	ImGui::SetNextWindowSize(this->style.imageHalfSize);

	ImGui::Begin("TitleImage", nullptr, windowFlags);
	ImGui::PopStyleVar();//pop WindowPadding

	//overload: ImGui::Image FluidumDraw
	ImGui::Image(this->image.value(), this->style.imageHalfSize);

	ImGui::End();
}

void FS::Title::selectProject() {
	ImGui::SetNextWindowPos(style.selectWindowPos);
	ImGui::SetNextWindowSize(style.selectWindowSize);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, varRead->popupWindowBorderSize());

	constexpr ImGuiWindowFlags windowFlags = FD::Style::VarRead::PopupWindowFlags();

	ImGui::Begin("SelectProject", nullptr, windowFlags);
	this->flag.isSelectProjectHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);

	ImGui::BeginChild("SelectProjectL", { style.selectWindowSize.x * 0.6f,0.0f }, false);

	ImGui::BeginChild("HistoryTitle", { 0.0f ,style.selectWindowSize.y * 0.15f });
	ImGui::Text(ICON_MD_HISTORY); ImGui::SameLine();
	ImGui::Text(text.recentProject);

	//popup
	if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		flag.recentProjectButtonPopup = true;

	ImGui::EndChild();
	ImGui::BeginChild("History", ImVec2(), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);

	//popup
	if (!flag.recentProjectButtonHovered && ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		flag.recentProjectPopup = true;

	this->recentProject();

	ImGui::EndChild();
	ImGui::EndChild(); ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();
	ImGui::BeginChild("SelectProjectR");

	//create project
	if (ImGui::Button(ICON_MD_ADD))
		this->newProject();
	ImGui::SameLine();
	ImGui::Text(text.newProject);

	ImGui::Spacing();

	//open a project file which exist
	this->openProjectButton();
	ImGui::SameLine();
	ImGui::Text(text.openProject);

	ImGui::Spacing(); ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing(); ImGui::Spacing();

	bool document = ImGui::Button(ICON_MD_LANGUAGE);
	if (document)
		pos.document = FU::ImGui::messagePos();

	ImGui::SameLine();

	ImGui::Text(text.document);
	if (document)
		this->documentLink();

	ImGui::EndChild();
	ImGui::End();

	ImGui::PopStyleVar(3);
}

void FS::Title::recentProject() {
	//failed to load file.
	if (flag.projectHistoryError) {
		ImGui::TextDisabled(text.error_loadProjectHistory);
		return;
	}

	//popup
	bool once = false;

	for (auto& x : recentProjectInfo) {
		if (x.projectName.empty())
			break;

		std::string buttonText = ICON_MD_FOLDER_OPEN + x.projectName + '\n' + x.projectFilePath + '\n' + x.ymd_h;
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2());
		bool clicked = ImGui::Button(buttonText.c_str(), { ImGui::GetWindowWidth() * 0.97f,0.0f });

		//popup
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
			flag.recentProjectButtonPopup = true;
			buttonPopup.info = &x;
		}
		if (!once && ImGui::IsItemHovered()) {
			flag.recentProjectButtonHovered = true;
			once = true;
		}

		pos.recentButton = ImGui::GetItemRectMin();
		ImGui::PopStyleVar();
		ImGui::Spacing();

		if (clicked) {
			FU::Cursor::setCursorType(FU::Cursor::Type::Wait);
			ImGui::SetMouseCursor(ImGuiMouseCursor_None);
			this->openProject(x.projectFilePath.c_str(), pos.recentButton);
		}
	}
}

void FS::Title::recentProjectPopup() {

	if (flag.recentProjectPopup) {
		ImGui::OpenPopup("RecentProject");
		flag.recentProjectPopup = false;
	}

	if (!ImGui::BeginPopup("RecentProject"))
		return;

	if (ImGui::Selectable(text.clear_icon)) {
		this->recentPopupItem_clear();
	}

	ImGui::EndPopup();
}

void FS::Title::recentProjectButtonPopup() {

	if (flag.recentProjectButtonPopup) {
		ImGui::OpenPopup("RecentProjectButton");
		flag.recentProjectButtonPopup = false;
	}

	if (!ImGui::BeginPopup("RecentProjectButton"))
		return;

	if (ImGui::Selectable(text.erase_icon)) {
		this->recentPopupItem_erase();
	}

	ImGui::EndPopup();
}

void FS::Title::recentPopupItem_clear() {

	const auto index = FU::MB::ok_cancel(text.confirm_clearHistory);
	if (index == 0) {//ok

	}
	else if (index == 1) { //cancel
		return;
	}
	else {
		FluidumScene_Log_InternalWarning();
		FU::MB::error(text_.unexpected.c_str());
	}

	//clear
	try {
		projectWrite->clearHistory();
	}
	catch (const FD::ProjectWrite::Exception val) {
		if (val == FD::ProjectWrite::Exception::Unexpected) {
		}
		else {
			FluidumScene_Log_InternalWarning();
		}
		FU::MB::error(text_.unexpected.c_str());
	}
	catch (...) {
		FluidumScene_Log_InternalWarning();
		FU::MB::error(text_.unexpected.c_str());
	}

	//clear
	this->recentProjectInfo = {};
}

void FS::Title::recentPopupItem_erase() {

	try {
		projectWrite->removeHistory(buttonPopup.info->projectFilePath);
	}
	catch (...) {
		FU::MB::error(text_.unexpected.c_str());
	}

	//update
	this->recentProjectInfo = projectRead->loadProjectHistory();

}

void FS::Title::newProject() {
	FluidumScene_Log_RequestAddScene(::FS::Project::File::New);
	Scene::addScene<Project::File::New>();
}

void FS::Title::openProjectButton() {
	if (!ImGui::Button(ICON_MD_FOLDER_OPEN))
		return;

	pos.open = ImGui::GetItemRectMin();

	std::unique_ptr<nfdchar_t*> outPath = std::make_unique<nfdchar_t*>();
	GLog.add_str<FU::Log::Type::None>("Open file dialog.");
	const nfdresult_t result = NFD_OpenDialog(".fproj", NULL, outPath.get());
	if (result == NFD_OKAY) {
		GLog.add_str<FU::Log::Type::None>("Open .fproj file({}).", *outPath.get());
		this->openProject(*outPath.get(), pos.open);
	}
	else if (result == NFD_CANCEL) {
		GLog.add_str<FU::Log::Type::None>("Cancel file dialog.");
	}
	else {//NFD_ERROR
		GLog.add_str<FU::Log::Type::Warning>("Error file dialog.");
	}

}

void FS::Title::openProject(const char* filePath, const ImVec2& pos) {
	GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Load .fproj file.");

	try {
		//strong
		projectWrite->loadProject(filePath);
	}
	catch (const FD::ProjectWrite::Exception type) {
		FluidumScene_Log_RequestAddScene(::FS::Utils::Message);

		//std::ifstream::operator bool() == false
		if (type == FD::ProjectWrite::Exception::FailedToOpenProjectFile) {
			Scene::addScene<Utils::Message>(text.error_openProjectFile, pos);
			try {
				projectWrite->removeHistory(filePath);
			}
			catch (...) {
				FluidumScene_Log_InternalWarning();
			}
			this->recentProjectInfo = projectRead->loadProjectHistory();
		}
		//wrong identifier 
		else if (type == FD::ProjectWrite::Exception::IllegalFile) {
			Scene::addScene<Utils::Message>(text.error_illegalFile, pos);
		}
		//broken file
		else if (type == FD::ProjectWrite::Exception::BrokenFile) {
			Scene::addScene<Utils::Message>(text.error_brokenFile, pos);
		}
		else {
			FluidumScene_Log_InternalWarning();
			FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
			Scene::addScene<Utils::Message>(text.error_unexpected, pos);
		}
		GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Failed to open .fproj file.");
		return;
	}
	catch (const std::exception&) {
		GLog.add_str<FU::Log::Type::None>(__FILE__, __LINE__, "Failed to open .fproj file.");
		FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
		Scene::addScene<Utils::Message>(text.error_unexpected, pos);
		return;
	}
	catch (...) {
		GLog.add_str<FU::Log::Type::None>(__FILE__, __LINE__, "Failed to open .fproj file.");
		FluidumScene_Log_InternalWarning();
		FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
		Scene::addScene<Utils::Message>(text.error_unexpected, pos);
		return;
	}

	GLog.add_str<FU::Log::Type::None>(__FILE__, __LINE__, "The file was successfully loaded.");
}

void FS::Title::documentLink() {
	if (!this->checkIsShellAvailable()) {
		FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
		Scene::addScene<Utils::Message>(text.error_notSupported, pos.document);
		return;
	}

#ifdef BOOST_OS_WINDOWS
	std::system("start https://fluidum.fluid-love.com/manual_beta/");
#elif BOOST_OS_MAC
	std::system("open https://fluidum.fluid-love.com/manual_beta/");
#else
#error Not Supported
#endif 
}

bool FS::Title::checkIsShellAvailable() const {
	int ret;

	try {
		ret = std::system(nullptr);
	}
	catch (...) {
		GLog.add<FU::Log::Type::Warning>(__FILE__, __LINE__, "Shell is not available.");
		return false;
	}

	if (ret == 0) {
		return false;
	}

	return true;
}
