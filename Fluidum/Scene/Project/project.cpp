#include "project.h"
#include "../Utils/Popup/message.h"
#include "../Coding/Tab/tab.h"
#include "../Coding/TextEditor/texteditor.h"

namespace FS {
	std::function<void(Project::Proj::Info)> displayCodeFunc{};
}

FS::Project::Project(
	FD::ProjectWrite* const projectWrite,
	const FD::ProjectRead* const projectRead,
	const FD::SceneRead* const sceneRead,
	FD::Coding::TabWrite* const tabWrite
) : projectWrite(projectWrite), projectRead(projectRead), sceneRead(sceneRead), tabWrite(tabWrite)
{
	GLog.add<FD::Log::Type::None>("Construct ProjectScene.");

	style.topBarHeight = ImGui::CalcTextSize(ICON_MD_FOLDER_OPEN).x + 2.0f;

	displayCodeFunc = std::bind(&Project::displayCode, this, std::placeholders::_1);

	this->syncListOfFiles();
}

FS::Project::~Project() noexcept {
	try {
		GLog.add<FD::Log::Type::None>("Destruct ProjectScene.");
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

void FS::Project::Project::call() {
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.07f, 0.07f, 0.07f, 0.14f));

	ImGui::Begin("Project");
	ImGui::SetWindowFontScale(0.93f);
	this->topBar();
	this->body1();
	this->body2();
	ImGui::End();

	ImGui::PopStyleColor();
}

void FS::Project::Project::topBar() {

	ImGui::BeginChild("ProjectTopBar", { ImGui::GetWindowSize().x,style.topBarHeight });

	//sync
	if (ImGui::Button(ICON_MD_SYNC)) {
		pos.sync = ImGui::GetItemRectMin();
		this->syncListOfFiles();
	}


	ImGui::SameLine();
	ImGui::Spacing();
	ImGui::SameLine();

	/*if (ImGui::Button(ICON_MD_CODE))
		this->tab_displayCode();*/

	ImGui::EndChild();

	ImGui::Separator();
	ImGui::Spacing();

}

namespace FS {

	constexpr inline std::pair<const char*, const char*> LuaStandardFluidumLibrary[] = {
		{ICON_FA_FILE_CODE " plot.lua",Resource::LuaSLLPlotPath},
		{ICON_FA_FILE_CODE " genome.lua",Resource::LuaSLLGenomePath},
		{ICON_FA_FILE_CODE " piano.lua",Resource::LuaSLLPianoPath},
		{ICON_FA_FILE_CODE " math.lua",Resource::LuaSLLMathPath},
	};

}

void FS::Project::Project::body1() {
	if (!ImGui::TreeNode(text.standardFluidumLibrary)) {
		ImGui::Separator();
		ImGui::Spacing();
		return;
	}

	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4());
	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2());
	const float buttonWidth = ImGui::GetWindowSize().x;

	constexpr ImGuiDockNodeFlags nodeFlag =
		ImGuiTreeNodeFlags_FramePadding |
		ImGuiTreeNodeFlags_OpenOnDoubleClick |
		ImGuiTreeNodeFlags_SpanAvailWidth
		;

	for (int8_t i = 0; i < std::extent_v<decltype(LuaStandardFluidumLibrary), 0>; i++) {
		ImGui::Spacing();
		if (ImGui::TreeNodeEx(LuaStandardFluidumLibrary[i].first, nodeFlag)) {
			ImGui::TreePop();
		}
	}
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();

	ImGui::TreePop();
	ImGui::Separator();
	ImGui::Spacing();
}

#include <imgui_internal.h>
namespace FS {

	template<typename T>
	ImRect dirTree(T* node, const typename T::value_type& info) {

		constexpr ImGuiDockNodeFlags nodeFlag =
			ImGuiTreeNodeFlags_OpenOnArrow |
			ImGuiTreeNodeFlags_SpanAvailWidth;

		std::string label{};
		if (info.isDirectory) {
			label = info.open ? ICON_MD_FOLDER_OPEN " " : ICON_MD_FOLDER " ";
		}
		else {
			label = ICON_MD_DESCRIPTION  " ";
		}
		label += info.name;

		ImGui::Spacing(); ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();
		const bool recurse = ImGui::TreeNodeEx(label.c_str(), nodeFlag);
		const ImRect nRect = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());

		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect(nRect.Min, nRect.Max))
			displayCodeFunc(info);

		if (recurse) {
			const ImColor col = ImGui::GetColorU32(ImGuiCol_Text);
			const float offsetX = 11.0f;
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			ImVec2 verticalLineStart = ImGui::GetCursorScreenPos();
			verticalLineStart.x += offsetX;
			ImVec2 verticalLineEnd = verticalLineStart;

			for (auto& x : *node) {
				const ImRect childRect = dirTree(&(x.dir_internal), x);
				const float lineSize = 8.0f;
				const float midpoint = (childRect.Min.y + childRect.Max.y) / 2.0f;
				drawList->AddLine(ImVec2(verticalLineStart.x, midpoint), ImVec2(verticalLineStart.x + lineSize, midpoint), col);
				verticalLineEnd.y = midpoint;
			}

			drawList->AddLine(verticalLineStart, verticalLineEnd, col);
		}

		if (recurse)
			ImGui::TreePop();
		return nRect;
	}
}

void FS::Project::Project::body2() {
	ImGui::BulletText(text.projectFolder);
	for (auto& x : project.listOfPaths)
		dirTree<std::vector<Proj::Info>>(&x.dir_internal, x);
}

void FS::Project::Project::syncListOfFiles() {
	const auto path = projectRead->getProjectFolderPath();

	auto backup = std::move(project.listOfPaths);
	project.listOfPaths = {};

	using Info = Proj::Info;

	std::function<std::vector<Info>(const std::string&)> func = [&](const std::string& dir) -> std::vector<Info> {

		std::vector<Info> result{};

		for (const auto& entry : std::filesystem::directory_iterator(dir)) {

			Info info{
				.dir_internal = entry.is_directory() ? func(entry.path().string()) : std::vector<Info>{},
				.isDirectory = entry.is_directory(),
				.open = false,
				.path = entry.path().string(),
				.name = info.isDirectory ? FU::File::directoryName(entry.path().string()) : entry.path().filename().string()
			};


			result.emplace_back(std::move(info));
		}
		return result;
	};


	try {
		project.listOfPaths = func(path);
	}
	catch (const std::exception&) {
		project.listOfPaths = std::move(backup);

		GLog.add<FD::Log::Type::None>("Request add Utils::MessageScene.");
	}

}

void FS::Project::displayCode(const Proj::Info& info) {
	if (!sceneRead->isExist<TextEditor>()) {
		GLog.add<FD::Log::Type::None>("Request add TextEditorScene.");
		tabWrite->addDisplayFile(info.path);
		Scene::addScene<TextEditor>();
	}
	tabWrite->addFile(info.path.c_str());
	if (!sceneRead->isExist<Coding::Tab>()) {
		GLog.add<FD::Log::Type::None>("Request add TabScene.");
		Scene::addScene<Coding::Tab>();
	}
}

























