#include "project.h"
#include "../Utils/Popup/message.h"
#include "../Coding/Tab/tab.h"
#include "../Coding/TextEditor/texteditor.h"
#include <imgui_internal.h>

namespace FS {
	std::function<void(Project::Proj::Info)> displayCodeFunc{};
}

namespace FS {
	class Index final {
	public:
		static std::size_t get() {
			if (i == std::numeric_limits<std::size_t>::max())
				throw std::runtime_error("The limit has been exceeded.");
			return ++i;
		}
	private:
		static inline std::size_t i = 0;
	};
}

FS::Project::Project(
	FD::ProjectWrite* const projectWrite,
	const FD::ProjectRead* const projectRead,
	FD::FluidumFilesWrite* const fluidumFilesWrite,
	const FD::FluidumFilesRead* const fluidumFilesRead,
	FD::ProjectFilesWrite* const projectFilesWrite,
	const FD::ProjectFilesRead* const projectFilesRead,
	FD::UserFilesWrite* const userFilesWrite,
	const FD::UserFilesRead* const userFilesRead,
	const FD::SceneRead* const sceneRead,
	FD::Coding::TabWrite* const tabWrite
) :
	projectWrite(projectWrite),
	projectRead(projectRead),
	fluidumFilesWrite(fluidumFilesWrite),
	fluidumFilesRead(fluidumFilesRead),
	projectFilesWrite(projectFilesWrite),
	projectFilesRead(projectFilesRead),
	userFilesWrite(userFilesWrite),
	userFilesRead(userFilesRead),
	sceneRead(sceneRead),
	tabWrite(tabWrite)
{
	GLog.add<FD::Log::Type::None>("Construct ProjectScene.");

	style.topBarHeight = ImGui::CalcTextSize(ICON_MD_FOLDER_OPEN).x + 2.0f;

	displayCodeFunc = std::bind(&Project::displayCode, this, std::placeholders::_1);

	this->syncListOfFiles();

	changeName.name.reserve(100);
	changeName.name = "aaa";
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
	auto userFilesLock = userFilesWrite->getLock();

	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.07f, 0.07f, 0.07f, 0.14f));

	ImGui::Begin("Project");
	ImGui::SetWindowFontScale(0.945f);

	this->topBar();

	ImGui::Spacing();

	this->tab();

	ImGui::End();

	ImGui::PopStyleColor();

	this->openPopup();
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

	//display
	//if (select && !select->isDirectory)
	//	if (ImGui::Button(ICON_MD_CODE))
	//		this->displayCode(*select);

	ImGui::EndChild();
}

void FS::Project::Project::tab() {
	if (!ImGui::BeginTabBar("ProjectTab"))
		return;

	ImGui::Spacing();

	ImGui::PushStyleColor(ImGuiCol_TabActive, { 0.2f,0.3f,0.75f,0.3f });

	if (ImGui::BeginTabItem("FluidumFiles")) {
		select.tab = TabType::Fluidum;
		this->fluidumFiles();
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("ProjectFiles")) {
		select.tab = TabType::Project;
		this->projectFiles();
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("UserFiles")) {
		select.tab = TabType::User;
		this->userFiles();
		ImGui::EndTabItem();
	}

	ImGui::PopStyleColor();

	ImGui::EndTabBar();
}

namespace FS {

	constexpr inline std::pair<const char*, const char*> LuaStandardFluidumLibrary[] = {
		{ICON_FA_FILE_CODE " plot.lua",Resource::LuaSLLPlotPath},
		{ICON_FA_FILE_CODE " genome.lua",Resource::LuaSLLGenomePath},
		{ICON_FA_FILE_CODE " piano.lua",Resource::LuaSLLPianoPath},
		{ICON_FA_FILE_CODE " math.lua",Resource::LuaSLLMathPath},
	};

}

void FS::Project::Project::fluidumFiles() {

	this->mainCodeFile();

	ImGui::Separator();
	ImGui::Spacing();

	this->standardFluidumLibrary();

}

void FS::Project::Project::mainCodeFile() {
	const std::string path = fluidumFilesRead->mainCodeFilePath();

	if (path.empty()) {
		ImGui::BulletText("no main file");
		return;
	}

	if (!ImGui::BeginTable("test", 2, ImGuiTableFlags_Borders))
		return;


	ImGui::Text("path"); ImGui::NextColumn();
	ImGui::Text(path.c_str()); ImGui::NextColumn();

	const std::string name = FU::File::fileName(path);
	ImGui::Text("name"); ImGui::NextColumn();
	ImGui::Text(name.c_str()); ImGui::NextColumn();


	ImGui::EndTable();
}

void FS::Project::Project::standardFluidumLibrary() {
	ImGui::BulletText("Lua");

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
}

namespace FS {

	template<typename T>
	ImRect dirTree(std::size_t& select, T* node, typename T::value_type& info) {

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

		if (info.open)
			ImGui::SetNextTreeNodeOpen(info.open);

		//selected color
		bool recurse = ImGui::TreeNodeEx(label.c_str(), nodeFlag);

		const ImRect nRect = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
		if (select == info.index) {
			constexpr ImU32 col = FU::ImGui::convertImVec4ToImU32(0.1f, 0.2f, 0.8f, 0.4f);
			ImGui::GetWindowDrawList()->AddRectFilled({ nRect.Min.x ,nRect.Min.y - 4.0f }, { nRect.Max.x,nRect.Max.y + 4.0f }, col);
		}

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect({ nRect.Min.x + 18.0f,nRect.Min.y }, nRect.Max)) {
			select = info.index;
		}
		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect(nRect.Min, nRect.Max)) {
			if (!info.isDirectory)
				displayCodeFunc(info);
			else
				info.open ^= 1;//if opened -> close, if closed -> open 
		}

		if (recurse) {
			const ImColor col = ImGui::GetColorU32(ImGuiCol_Text);
			const float offsetX = 11.0f;
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			ImVec2 verticalLineStart = ImGui::GetCursorScreenPos();
			verticalLineStart.x += offsetX;
			ImVec2 verticalLineEnd = verticalLineStart;

			for (auto& x : *node) {
				const ImRect childRect = dirTree(select, &(x.dir_internal), x);
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

void FS::Project::Project::projectFiles() {
	ImGui::BulletText(text.projectFolder);
	for (auto& x : project.listOfPaths)
		dirTree<std::vector<Proj::Info>>(this->selectedIndex, &x.dir_internal, x);


}

void FS::Project::Project::userFiles() {
	ImGui::BulletText(text.projectFolder);
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered()) {
		popup.top = true;

		select.userFiles = nullptr;
	}

	auto* data = userFilesWrite->fileList();
	for (auto& x : *data) {
		this->userFilesTree(&x.dir_internal, &x);
	}
}

namespace FS {

	std::string makeLabel(const FD::Project::List::FileInfo* info) {
		std::string label{};
		if (info->type == FD::Project::List::Type::Directory) {
			label = info->open ? ICON_MD_FOLDER_OPEN " " : ICON_MD_FOLDER " ";
		}
		else if (info->type == FD::Project::List::Type::Code) {
			label = ICON_MD_DESCRIPTION  " ";
		}
		label += info->name;
		return label;
	}
}

std::pair<ImVec2, ImVec2> FS::Project::userFilesTree(std::vector<FD::Project::List::FileInfo>* node, FD::Project::List::FileInfo* info) {

	constexpr ImGuiDockNodeFlags nodeFlag =
		ImGuiTreeNodeFlags_OpenOnArrow |
		ImGuiTreeNodeFlags_SpanAvailWidth;

	const std::string label = makeLabel(info);

	ImGui::Spacing(); ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();

	if (info->open)
		ImGui::SetNextTreeNodeOpen(info->open);

	//selected color
	bool recurse = ImGui::TreeNodeEx(label.c_str(), nodeFlag);

	const ImRect nRect = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());

	if (select.userFiles == info) {
		constexpr ImU32 col = FU::ImGui::convertImVec4ToImU32(0.18f, 0.26f, 0.83f, 0.3f);
		ImGui::GetWindowDrawList()->AddRectFilled({ nRect.Min.x ,nRect.Min.y - 3.0f }, { nRect.Max.x,nRect.Max.y + 3.0f }, col);

		if (changeName.popup) {
			using namespace FU::ImGui::Operators;
			changeName.pos = { nRect.Min.x + 48.0f ,nRect.Min.y - 1.0f };
			changeName.size = { nRect.Max.x - changeName.pos.x,nRect.Max.y - changeName.pos.y + 1.0f };
		}
	}

	//select
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect({ nRect.Min.x + 18.0f,nRect.Min.y }, nRect.Max)) {
		select.userFiles = info;
		pos.selectedTree = ImGui::GetItemRectMax();
	}

	//right
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered()) {
		pos.selectedTree = ImGui::GetItemRectMax();
		select.userFiles = info;
		if (info->type == FD::Project::List::Type::Directory)
			popup.dir = true;
		else if (info->type == FD::Project::List::Type::Code)
			popup.code = true;
	}

	//double click
	if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect(nRect.Min, nRect.Max)) {
		if (info->type == FD::Project::List::Type::Directory)
			info->open ^= 1;//if opened -> close, if closed -> open 
			//if (!info.isDirectory)
			//	displayCodeFunc(info);
			//else
	}


	if (recurse) {
		const ImColor col = ImGui::GetColorU32(ImGuiCol_Text);
		const float offsetX = 11.0f;
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		ImVec2 verticalLineStart = ImGui::GetCursorScreenPos();
		verticalLineStart.x += offsetX;
		ImVec2 verticalLineEnd = verticalLineStart;

		for (auto& x : *node) {
			const auto tempRect = userFilesTree(&(x.dir_internal), &x);
			const ImRect childRect = { tempRect.first,tempRect.second };
			const float lineSize = 8.0f;
			const float midpoint = (childRect.Min.y + childRect.Max.y) / 2.0f;
			drawList->AddLine(ImVec2(verticalLineStart.x, midpoint), ImVec2(verticalLineStart.x + lineSize, midpoint), col);
			verticalLineEnd.y = midpoint;
		}

		drawList->AddLine(verticalLineStart, verticalLineEnd, col);
	}

	if (recurse)
		ImGui::TreePop();

	//imgui_internal.h ImRect
	return { nRect.Min,nRect.Max };
}

void FS::Project::Project::addVirtualFolder() {
	FD::Project::List::DirectoryInfo dir{};

	std::string tempName = userFilesWrite->makeTempName();
	std::string tempPath = (select.userFiles ? select.userFiles->path : "Fluidum:") + '/' + tempName;

	//limit
	if (tempName.empty()) {
		GLog.add<FD::Log::Type::None>("Request add Utils::MessageScene.");
		//Scene::addScene<Utils::Message>();
		return;
	}

	if (select.userFiles)
		select.userFiles->open = true;

	select.userFiles = userFilesWrite->addFile(select.userFiles ? select.userFiles->path : std::string{}, tempPath, dir);

	changeName.popup = true;
}

void FS::Project::eraseVirtualFolder() {
	assert(select.userFiles != nullptr);

	//MessageBox
	//kokoni

	userFilesWrite->eraseFile(select.userFiles->path);


	select.userFiles = nullptr;
	changeName.popup = true;
}

void FS::Project::addFile() {

}

void FS::Project::Project::openPopup() {
	//delay(one frame)
	this->changeNamePopup();

	if (popup.top) {
		ImGui::OpenPopup(popup.Top);
		popup.top = false;
	}
	else if (popup.dir) {
		ImGui::OpenPopup(popup.Dir);
		popup.dir = false;
	}
	else if (popup.code) {
		ImGui::OpenPopup(popup.Code);
		popup.code = false;
	}
	else if (changeName.popup) {
		ImGui::OpenPopup(popup.ChangeName);
		changeName.popup = false;
		changeName.once = true;
	}

	this->topPopup();
	this->directoryPopup();
	this->codePopup();

	//change name
	this->tryChangeName();
}

void FS::Project::Project::topPopup() {
	if (!ImGui::BeginPopup(popup.Top))
		return;

	//ImGui::Separator();

	if (ImGui::Selectable("addVFolder"))
		this->addVirtualFolder();

	if (ImGui::Selectable("addFile"))
		this->addFile();


	ImGui::EndPopup();
}

void FS::Project::Project::directoryPopup() {

	if (!ImGui::BeginPopup(popup.Dir))
		return;

	if (ImGui::Selectable(select.userFiles->open ? "close" : "open"))
		;// this->addVirtualFolder();

	ImGui::Separator();

	if (ImGui::Selectable("addVFolder"))
		this->addVirtualFolder();

	if (ImGui::Selectable("addFile"))
		this->addFile();

	ImGui::Separator();

	if (ImGui::Selectable("erase"))
		this->eraseVirtualFolder();

	ImGui::EndPopup();
}

void FS::Project::Project::codePopup() {
	if (!ImGui::BeginPopup(popup.Code))
		return;

	ImGui::EndPopup();
}

void FS::Project::Project::changeNamePopup() {

	ImGui::SetNextWindowPos(changeName.pos);
	ImGui::SetNextWindowSize(changeName.size);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f,0.0f });
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0.0f,0.0f });

	ImGui::PushStyleColor(ImGuiCol_Text, { 0.0f,0.0f,0.0f,1.0f });
	ImGui::PushStyleColor(ImGuiCol_PopupBg, { 0.8f,0.8f,0.8f,1.0f });
	ImGui::PushStyleColor(ImGuiCol_FrameBg, { 1.0f,1.0f,1.0f,0.0f });
	ImGui::PushStyleColor(ImGuiCol_Border, { 1.0f,1.0f,1.0f,0.0f });
	ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, { 0.1f,0.2f,0.96f,0.7f });

	if (ImGui::BeginPopup(popup.ChangeName)) {
		ImGui::SetWindowFontScale(1.05f);
		ImGui::Spacing(); ImGui::SameLine();
		ImGui::InputText("##ChangeName", changeName.name.data(), changeName.name.capacity(), ImGuiInputTextFlags_AutoSelectAll);

		ImGui::EndPopup();
	}


	ImGui::PopStyleVar(3);
	ImGui::PopStyleColor(5);
}

void FS::Project::Project::tryChangeName() {
	if (!changeName.once || ImGui::IsPopupOpen(popup.ChangeName))
		return;

	if (!this->checkChangeName())
		return;

	std::string name = changeName.name.c_str();
	//‹æØ‚è•¶Žš‹ÖŽ~@ŽÀ‘•
	bool result = userFilesWrite->tryChangeName(select.userFiles->path, name);
	if (!result) {
		//MessageBox
	}


	//reset
	changeName.name = std::string();
	changeName.name.reserve(100);

	changeName.once = false;
}

bool FS::Project::Project::checkChangeName() {
	const std::string name = changeName.name.c_str();

	if (name.empty()) {
		FU::MB::error("test");
		return false;
	}

	auto itr = std::find(name.begin(), name.end(), '/');
	if (itr != name.end()) {
		FU::MB::error("test");
		return false;
	}

}

void FS::Project::Project::syncListOfFiles() {
	FU::Cursor::setCursorType(FU::Cursor::Type::Wait);

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
				.name = info.isDirectory ? FU::File::directoryName(entry.path().string()) : entry.path().filename().string(),
				.index = Index::get()
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
		//Scene::addScene<Utils::Message>();
	}

}

//double clicked
void FS::Project::displayCode(const Proj::Info& info) {
	assert(!info.isDirectory);

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

























