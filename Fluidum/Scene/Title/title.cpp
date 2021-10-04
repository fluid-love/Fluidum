#include "title.h"

//bar
#include "../Bar/Left/leftbar.h"
#include "../Bar/Top/topbar.h"
#include "../Bar/Menu/menubar.h"
#include "../Bar/Status/statusbar.h"
#include "../Bar/Title/titlebar.h"

//layout
#include "../Layout/Left/layout.h"
#include "../Layout/Right/layout.h"

//project
#include "../Bar/Menu/Project/newproject.h"

//utils
#include "../Utils/Popup/message.h"

//file dialog
#include <nfd.h>

namespace FS::Internal {
	std::vector<FDR::ImGuiImage> createImages() {
		//画像をセット
		constexpr const char* imageFileNames[] = {
			"coding.png",
			"node.png",
			"analysis.png",
			"project.png",
			"animation.png",
			"genome.png",
			"console.png",
			"plot.png",
			"func.png",
		};

		std::vector<FDR::ImGuiImage> result{};

		for (uint16_t i = 0; i < std::extent_v<decltype(imageFileNames), 0>; i++) {
			std::string path = Resource::LeftBarIconsFilePath;
			path += imageFileNames[i];

			result.emplace_back(FDR::createImGuiImage(path.c_str()));
		}

		return result;
	}
}

void FS::Title::call() {

	std::call_once(this->once, &Title::writeGuiData, this);

	//if IsMouseButton -> next scene
	this->changeScene();

	this->drawTitleImage();
	this->selectProject();
}

FS::Title::Title(
	FD::ProjectWrite* const projectWrite,
	const FD::ProjectRead* const projectRead,
	const FD::SceneRead* const sceneRead,
	FD::GuiWrite* const guiWrite,
	const FD::GuiRead* const guiRead
) : projectWrite(projectWrite), projectRead(projectRead), sceneRead(sceneRead), guiWrite(guiWrite), guiRead(guiRead), recentProjectInfos(projectRead->getProjectHistory())
{
	GLog.add<FD::Log::Type::None>("Construct TitleScene.");
}

FS::Title::~Title() noexcept {
	try {
		GLog.add<FD::Log::Type::None>("Destruct TitleScene.");
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

//call once
void FS::Title::writeGuiData() {
	//operators
	using namespace FU::ImGui::Operators;

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

}

void FS::Title::changeScene() {
	if ((!isSelectProjectHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !sceneRead->isExist<Bar::NewProject>()) || !projectRead->isDefaultProject()) {
		GLog.add<FD::Log::Type::None>("Request delete TitleScene.");
		Scene::deleteScene<Title>();//このシーンを消す

		//Barを追加
		GLog.add<FD::Log::Type::None>("Request add MenuBarScene.");
		Scene::addScene<MenuBar>();
		GLog.add<FD::Log::Type::None>("Request add StatusBarScene.");
		Scene::addScene<StatusBar>();
		GLog.add<FD::Log::Type::None>("Request add TopBarScene.");
		Scene::addScene<TopBar>();
		GLog.add<FD::Log::Type::None>("Request add LeftBarScene.");
		Scene::addScene<LeftBar>(std::move(this->leftBarImages.value()));
		GLog.add<FD::Log::Type::None>("Request add TitleBarScene.");
		Scene::addScene<TitleBar>();

		GLog.add<FD::Log::Type::None>("Request add LeftLayoutScene.");
		Scene::addScene<LeftLayout>();
		GLog.add<FD::Log::Type::None>("Request add RightLayoutScene.");
		Scene::addScene<RightLayout>();
	}
}

void FS::Title::drawTitleImage() {
	constexpr ImGuiWindowFlags flag =
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_HorizontalScrollbar |
		ImGuiWindowFlags_NoTitleBar;

	//Padding 0
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());

	ImGui::SetNextWindowPos(this->style.windowPos);
	ImGui::SetNextWindowSize(this->style.imageHalfSize);

	ImGui::Begin("TitleImage", nullptr, flag);
	ImGui::PopStyleVar();//pop WindowPadding

	//overload ImGui::Image FluidumDraw
	ImGui::Image(this->image.value(), ImVec2(this->style.imageHalfSize));

	ImGui::End();
}

void FS::Title::selectProject() {
	ImGui::SetNextWindowPos(style.selectWindowPos);
	ImGui::SetNextWindowSize(style.selectWindowSize);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.05f);

	constexpr ImGuiWindowFlags flag =
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_HorizontalScrollbar |
		ImGuiWindowFlags_NoTitleBar;

	ImGui::Begin("SelectProject", nullptr, flag);
	isSelectProjectHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);

	ImGui::BeginChild("SelectProjectL", { style.selectWindowSize.x * 0.65f,0.0f }, false);

	ImGui::BeginChild("HistoryTitle", { 0.0f ,style.selectWindowSize.y * 0.15f });
	ImGui::Text(ICON_MD_HISTORY); ImGui::SameLine();
	ImGui::Text(text.recentProject);
	ImGui::EndChild();
	ImGui::BeginChild("History", ImVec2(), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);

	this->recentProject();

	ImGui::EndChild();
	ImGui::EndChild(); ImGui::SameLine();
	ImGui::BeginChild("SelectProjectR");

	//create NewProject
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

	bool document = ImGui::Button(ICON_MD_LANGUAGE); ImGui::SameLine();
	ImGui::Text(text.document);
	if (document)
		this->documentLink();

	ImGui::EndChild();
	ImGui::End();

	ImGui::PopStyleVar(3);
}

void FS::Title::recentProject() {
	for (auto& x : recentProjectInfos) {
		if (x.projectName.empty())
			break;

		std::string buttonText = ICON_MD_FOLDER_OPEN + x.projectName + '\n' + x.projectFilePath + '\n' + x.ymd_h;
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2());
		bool clicked = ImGui::Button(buttonText.c_str(), { ImGui::GetWindowWidth() * 0.97f,0.0f });
		ImGui::PopStyleVar();
		ImGui::Spacing();

		if (clicked) {
			this->openProject(x.projectFilePath.c_str());
		}
	}
}

void FS::Title::newProject() {
	GLog.add<FD::Log::Type::None>("Request add NewProjectScene.");
	Scene::addScene<Bar::NewProject>();
}

void FS::Title::openProjectButton() {
	if (!ImGui::Button(ICON_MD_FOLDER_OPEN))
		return;

	pos.open = ImGui::GetItemRectMin();

	std::unique_ptr<nfdchar_t*> outPath = std::make_unique<nfdchar_t*>();
	GLog.add<FD::Log::Type::None>("Open file dialog.");
	const nfdresult_t result = NFD_OpenDialog(".fproj", NULL, outPath.get());
	if (result == NFD_OKAY) {
		GLog.add<FD::Log::Type::None>("Select .fproj file. Path is {}.", *outPath.get());
		this->openProject(*outPath.get());
	}
	else if (result == NFD_CANCEL) {
		GLog.add<FD::Log::Type::None>("Cancel file dialog.");
	}
	else {//NFD_ERROR
		GLog.add<FD::Log::Type::Error>("Error file dialog.");
		throw std::runtime_error("NFD_OpenDialog() return NFD_ERROR.");
	}

}

void FS::Title::openProject(const char* filePath) {
	GLog.add<FD::Log::Type::None>("Request load .fproj file.");
	try {
		projectWrite->loadExistProject(filePath);
	}
	catch (const FD::Project::ExceptionType type) {
		GLog.add<FD::Log::Type::None>("Request add Utils::MessageScene.");

		//std::ifstream::operator bool() == false
		if (type == FD::Project::ExceptionType::FailedToOpenProjectFile) {
			Scene::addScene<Utils::Message>(text.error_openProjectFile, pos.open);
		}
		//wrong identifier 
		else if (type == FD::Project::ExceptionType::IllegalFile) {
			Scene::addScene<Utils::Message>(text.error_illegalFile, pos.open);
		}
		//broken file
		else if (type == FD::Project::ExceptionType::BrokenFile) {
			Scene::addScene<Utils::Message>(text.error_brokenFile, pos.open);
		}
		else {
			GLog.add<FD::Log::Type::Error>("abort() has been called. File {}.", __FILE__);
			abort();
		}
		GLog.add<FD::Log::Type::Error>("Failed to open .fproj file.");
		return;
	}
	catch (const std::exception&) {
		GLog.add<FD::Log::Type::Error>("Failed to open .fproj file.");
		GLog.add<FD::Log::Type::None>("Request add Utils::MessageScene.");
		Scene::addScene<Utils::Message>(text.error_internal, pos.open);
		return;
	}

	GLog.add<FD::Log::Type::None>("Load .fproj file.");
}

void FS::Title::documentLink() {
#ifdef BOOST_OS_WINDOWS
	system("start https://github.com/fluid-love/Fluidum");
#elif BOOST_OS_MAC
	system("open https://github.com/fluid-love/Fluidum");
#else
#error Not Supported
#endif 
}
