#include "project.h"
#include "../Utils/Popup/message.h"
#include "../Coding/Tab/tab.h"
#include "../Coding/TextEditor/texteditor.h"
#include <imgui_internal.h>

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
	FD::LuaFilesWrite_Lock* const luaFilesWrite,
	FD::FluidumFilesWrite* const fluidumFilesWrite,
	const FD::FluidumFilesRead* const fluidumFilesRead,
	FD::ProjectFilesWrite_Lock* const projectFilesWrite,
	const FD::ProjectFilesRead* const projectFilesRead,
	FD::UserFilesWrite_Lock* const userFilesWrite,
	const FD::UserFilesRead* const userFilesRead,
	const FD::SceneRead* const sceneRead,
	FD::Coding::TabWrite* const tabWrite
) :
	projectWrite(projectWrite),
	projectRead(projectRead),
	luaFilesWrite(luaFilesWrite),
	fluidumFilesWrite(fluidumFilesWrite),
	fluidumFilesRead(fluidumFilesRead),
	projectFilesWrite(projectFilesWrite),
	projectFilesRead(projectFilesRead),
	userFilesWrite(userFilesWrite),
	userFilesRead(userFilesRead),
	sceneRead(sceneRead),
	tabWrite(tabWrite)
{
	FluidumScene_Log_Constructor("Project");

	style.topBarHeight = ImGui::CalcTextSize(ICON_MD_FOLDER_OPEN).x + 2.0f;

	//sync project files
	projectFilesWrite->sync(projectRead->getProjectFolderPath());
	auto d = projectFilesWrite->fileList();

	changeName.name.reserve(100);
}

FS::Project::~Project() noexcept {
	FluidumScene_Log_Destructor_("Project");
}

void FS::Project::Project::call() {
	//lock
	auto userFilesLock = userFilesWrite->getLock();
	auto projectFilesLock = projectFilesWrite->getLock();
	auto luaLock = luaFilesWrite->getLock();

	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.07f, 0.07f, 0.07f, 0.14f));

	ImGui::Begin(text.project);
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
	if (select.tab == TabType::Fluidum) {//dummy
		//const int32_t count = FU::ImGui::pushStyleColor<ImGuiCol_Button, ImGuiCol_ButtonHovered, ImGuiCol_ButtonActive>({ 0.05f,0.05f,0.05f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.01f,0.01f,0.01f,0.7f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.01f,0.01f,0.01f,0.7f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.01f,0.01f,0.01f,0.7f });
		ImGui::PushStyleColor(ImGuiCol_Border, { 0.01f,0.01f,0.01f,0.7f });
		ImGui::Button(ICON_MD_SYNC);
		ImGui::PopStyleColor(4);
	}
	else {
		if (ImGui::Button(ICON_MD_SYNC)) {
			pos.sync = ImGui::GetItemRectMin();
			this->syncProjectFiles();
		}
	}
	FU::ImGui::tooltip(anime.sync, text.sync);

	ImGui::SameLine();

	//Collapse all folders and files
	if (ImGui::Button(ICON_MD_FOLDER)) {
		this->collapseAll();
	}
	FU::ImGui::tooltip(anime.collapseAll, text.tooltip_collpsedAll);

	ImGui::SameLine();
	ImGui::Spacing();
	ImGui::SameLine();

	//show
	this->showCodeButton();
	FU::ImGui::tooltip(anime.displayCode, text.displayCode);


	ImGui::EndChild();
}

void FS::Project::Project::showCodeButton() {
	auto* current = select.current();
	
	if (!current || current->type != FD::Project::List::Type::Code) {
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.01f,0.01f,0.01f,0.7f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.01f,0.01f,0.01f,0.7f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.01f,0.01f,0.01f,0.7f });
		ImGui::PushStyleColor(ImGuiCol_Border, { 0.01f,0.01f,0.01f,0.7f });
		ImGui::Button(ICON_MD_CODE);
		ImGui::PopStyleColor(4);
		return;
	}

	if (ImGui::Button(ICON_MD_CODE))
		this->showCode();
}

void FS::Project::Project::tab() {
	if (!ImGui::BeginTabBar("ProjectTab"))
		return;

	ImGui::Spacing();

	ImGui::PushStyleColor(ImGuiCol_TabActive, { 0.2f,0.3f,0.75f,0.3f });

	if (ImGui::BeginTabItem("Fluidum")) {
		select.tab = TabType::Fluidum;
		this->fluidumFiles();
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem(text.project)) {
		select.tab = TabType::Project;
		this->projectFiles();
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem(text.user)) {
		select.tab = TabType::User;
		this->userFiles();
		ImGui::EndTabItem();
	}

	ImGui::PopStyleColor();

	ImGui::EndTabBar();
}

namespace FS {

	constexpr inline std::pair<const char*, const char*> LuaStandardFluidumLibrary[] = {
		{ICON_FA_FILE_CODE " fluidum.system",Resource::LuaFluidumStandardLibraryFolderPath},
		{ICON_FA_FILE_CODE " fluidum.array",Resource::LuaFluidumStandardLibraryFolderPath},
		//{ICON_FA_FILE_CODE " genome.lua",Resource::LuaSLLGenomePath},
		//{ICON_FA_FILE_CODE " piano.lua",Resource::LuaSLLPianoPath},
		//{ICON_FA_FILE_CODE " math.lua",Resource::LuaSLLMathPath},
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
		ImGui::BulletText(text.mainFileDoesNotExist);
		return;
	}

	if (!ImGui::BeginTable("##MainFileInfo", 2, ImGuiTableFlags_Borders))
		return;


	ImGui::Text(text.path); ImGui::NextColumn();
	ImGui::Text(path.c_str()); ImGui::NextColumn();

	const std::string name = FU::File::fileName(path);
	ImGui::Text(text.name); ImGui::NextColumn();
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
		if (ImGui::TreeNodeEx(LuaStandardFluidumLibrary[i].first, nodeFlag)) {
			ImGui::TreePop();
		}
	}

	ImGui::PopStyleColor();
	ImGui::PopStyleVar();

	ImGui::TreePop();
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
		else if (info->type == FD::Project::List::Type::Unsupported) {
			label = ICON_FA_FILE  " ";
		}
		label += info->name;
		return label;
	}
}

std::pair<ImVec2, ImVec2> FS::Project::projectFilesTree(std::vector<FD::Project::List::FileInfo>* node, FD::Project::List::FileInfo* info) {

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

	if (select.projectFiles == info) {
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
		select.projectFiles = info;
		pos.selectedTree = ImGui::GetItemRectMax();
	}

	//right
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered()) {
		pos.selectedTree = ImGui::GetItemRectMax();
		select.projectFiles = info;
		if (info->type == FD::Project::List::Type::Directory)
			popup.dir = true;
		else if (info->type == FD::Project::List::Type::Code)
			popup.code = true;
	}

	//double click
	if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect(nRect.Min, nRect.Max)) {
		if (info->type == FD::Project::List::Type::Directory)
			info->open ^= 1;//if opened -> close, if closed -> open 

	}


	if (recurse) {
		const ImColor col = ImGui::GetColorU32(ImGuiCol_Text);
		const float offsetX = 11.0f;
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		ImVec2 verticalLineStart = ImGui::GetCursorScreenPos();
		verticalLineStart.x += offsetX;
		ImVec2 verticalLineEnd = verticalLineStart;

		for (auto& x : *node) {
			const auto tempRect = projectFilesTree(&(x.dir_internal), &x);
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

void FS::Project::Project::projectFiles() {
	ImGui::BulletText(text.projectFolder);
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered()) {
		popup.top = true;
		select.projectFiles = nullptr;
	}

	auto* data = projectFilesWrite->fileList();
	for (auto& x : *data) {
		this->projectFilesTree(&x.dir_internal, &x);
	}
}

void FS::Project::Project::createDirectory() {
	assert(select.tab == TabType::Project);

	//std::filesystem::create_directory(select.projectFiles->path);

	FD::Project::List::DirectoryInfo dir{};

	//make temp path
	std::string path{};
	std::string tempName{};
	for (uint16_t i = 1; i <= FD::Project::Limits::ProjectFiles::DirectorySizeMax; i++) {
		tempName = "NewFolder" + std::to_string(i);
		if (!std::filesystem::exists(select.projectFiles->path + tempName)) {
			path = select.projectFiles->path + tempName;
			break;
		}
	}

	//limit
	if (path.empty()) {
		FU::MB::error(text.mainFile);
		return;
	}

	if (select.projectFiles)
		select.projectFiles->open = true;

	select.projectFiles = projectFilesWrite->addFile(select.projectFiles ? select.projectFiles->path : std::string{}, path, dir);

	changeName.name = std::move(tempName);
	changeName.popup = true;
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
		FU::MB::error(text.error_maxSize);
		return;
	}

	if (select.userFiles)
		select.userFiles->open = true;

	select.userFiles = userFilesWrite->addFile(select.userFiles ? select.userFiles->path : std::string{}, tempPath, dir);

	changeName.name = std::move(tempName);
	changeName.popup = true;
}

void FS::Project::eraseVirtualFolder() {
	assert(select.userFiles != nullptr);

	//confirm
	const auto result = FU::MB::ok_cancel(text.confirm_releaseVirtualFolder);
	assert(result == 0 || result == 1);
	if (result == 1) {//cancel
		return;
	}

	userFilesWrite->eraseFile(select.userFiles->path);
	select.userFiles = nullptr;
}

void FS::Project::eraseUserFile() {
	assert(select.userFiles != nullptr);

	//confirm
	const auto result = FU::MB::ok_cancel(text.confirm_releaseFile);
	assert(result == 0 || result == 1);
	if (result == 1) {//cancel
		return;
	}

	userFilesWrite->eraseFile(select.userFiles->path);
	select.userFiles = nullptr;
}

void FS::Project::showCode() {
	const std::string& path = select.current()->path;

	if (!sceneRead->isExist<TextEditor>()) {
		tabWrite->addDisplayFile(path);
		GLog.add<FD::Log::Type::None>("Request add TextEditorScene.");
		Scene::addScene<TextEditor>();
	}
	tabWrite->addFile(path);
	if (!sceneRead->isExist<Coding::Tab>()) {
		GLog.add<FD::Log::Type::None>("Request add TabScene.");
		Scene::addScene<Coding::Tab>();
	}
}

void FS::Project::flipOpen() {
	assert(select.current() != nullptr);
	select.current()->open ^= 1;
}

void FS::Project::addFile() {
	assert(!newFile.info);
	newFile.info = std::make_shared<Coding::New::SharedInfo>();
	GLog.add<FD::Log::Type::None>("Request add Coding::NewScene.");
	Scene::addScene<Coding::New>(newFile.info);

	newFile.popup = true;
}

void FS::Project::Project::catchAddFile() {
	if (!newFile.popup)
		return;

	if (newFile.info.use_count() > 1)
		return;

	//Coding::New scene is closed

	//canceled
	if (!newFile.info->create)
		return;

	//created
	FD::Project::List::CodeInfo info{};
	select.userFiles = userFilesWrite->addFile(select.userFiles->path, newFile.info->path, info);

	newFile.info.reset();
	newFile.popup = false;
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

	if (select.tab == TabType::User) {
		if (ImGui::Selectable(text.createVirtualFolder))
			this->addVirtualFolder();
	}
	else if (select.tab == TabType::Project) {
		if (ImGui::Selectable(text.createFolder))
			this->addVirtualFolder();
	}

	if (ImGui::Selectable(text.createFile))
		this->addFile();

	ImGui::EndPopup();
}

void FS::Project::Project::directoryPopup() {

	if (!ImGui::BeginPopup(popup.Dir))
		return;

	if (ImGui::Selectable(select.current()->open ? text.close : text.open))
		this->flipOpen();

	ImGui::Separator();

	if (ImGui::Selectable(text.createVirtualFolder))
		this->addVirtualFolder();

	if (ImGui::Selectable(text.createFile))
		this->addFile();

	ImGui::Separator();

	if (ImGui::Selectable(text.delete_release))
		this->eraseVirtualFolder();

	ImGui::EndPopup();
}

void FS::Project::Project::codePopup() {
	if (!ImGui::BeginPopup(popup.Code))
		return;

	if (ImGui::Selectable(text.displayCode))
		this->showCode();

	ImGui::Separator();

	if (ImGui::Selectable(text.delete_release))
		this->eraseUserFile();

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

	if (!this->checkChangeName()) {
		//reset
		changeName.name = std::string();
		changeName.name.reserve(100);

		changeName.once = false;
		return;
	}

	std::string name = changeName.name.c_str();

	if (select.tab == TabType::User) {
		//bool result = userFilesWrite->tryChangeName(select.userFiles->path, name);
	}
	else if (select.tab == TabType::User) {
		//exist same name file -> false
		bool result = userFilesWrite->tryChangeName(select.userFiles->path, name);
		if (!result) {
			FU::MB::error(text.error_sameName);
		}
	}


	//reset
	changeName.name = std::string();
	changeName.name.reserve(100);

	changeName.once = false;
}

bool FS::Project::Project::checkChangeName() {
	const std::string name = changeName.name.c_str();

	if (name.empty()) {
		FU::MB::error(text.error_emptyName);
		return false;
	}

	//e.x. Windows / | > < ...
	if (FU::File::containForbiddenCharactor(name)) {
		FU::MB::error(text.error_forbiddenCharactor);
		return false;
	}

	return true;
}

void FS::Project::Project::syncProjectFiles() {
	assert(select.tab != TabType::Fluidum);

	FU::Cursor::setCursorType(FU::Cursor::Type::Wait);

	if (select.tab == TabType::Project)
		projectFilesWrite->sync(projectRead->getProjectFolderPath());
	else if (select.tab == TabType::User)
		userFilesWrite->sync();
	else {
		GLog.add<FD::Log::Type::Error>("abort() has been called. File {}.", __FILE__);
		abort();
	}
}

void FS::Project::Project::collapseAll() {
	FU::Cursor::setCursorType(FU::Cursor::Type::Wait);

	if (select.tab == TabType::Fluidum) {
		luaFilesWrite->closeAll();
		//..
	}
	else if (select.tab == TabType::Project)
		projectFilesWrite->closeAll();
	else if (select.tab == TabType::User)
		userFilesWrite->closeAll();
}
