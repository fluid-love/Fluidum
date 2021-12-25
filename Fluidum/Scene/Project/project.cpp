#include "project.h"
#include "../Utils/Popup/message.h"
#include "../Coding/Tab/tab.h"
#include "../Coding/TextEditor/texteditor.h"
#include <imgui_internal.h>
#include "Add/select.h"

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

FS::Project::Explorer::Explorer(
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

FS::Project::Explorer::~Explorer() noexcept {
	FluidumScene_Log_Destructor_("Project");
}

void FS::Project::Explorer::call() {
	//lock
	auto userFilesLock = userFilesWrite->getLock();
	auto projectFilesLock = projectFilesWrite->getLock();
	auto luaLock = luaFilesWrite->getLock();

	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.27f, 0.67f, 0.87f, 0.34f));

	ImGui::Begin(text.explorer, &windowFlag);
	ImGui::SetWindowFontScale(0.945f);

	this->topBar();

	ImGui::Spacing();

	this->tab();

	ImGui::End();

	ImGui::PopStyleColor();

	this->openPopup();
	this->catchAdd();
}

void FS::Project::Explorer::closeWindow() {
	if (!this->windowFlag)
		return;

	FluidumScene_Log_RequestDeleteScene("Project");
	Scene::deleteScene<Explorer>();
}

void FS::Project::Explorer::topBar() {

	ImGui::BeginChild("ProjectTopBar", { ImGui::GetWindowSize().x,style.topBarHeight });

	//sync
	if (select.tab == TabType::Fluidum) {//dummy
		//const int32_t count = FU::ImGui::pushStyleColor<ImGuiCol_Button, ImGuiCol_ButtonHovered, ImGuiCol_ButtonActive>({ 0.05f,0.05f,0.05f,1.0f });
		ImVec4 colDisabled = ImGui::GetStyle().Colors[ImGuiCol_Button];
		ImGui::PushStyleColor(ImGuiCol_Button, colDisabled);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colDisabled);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, colDisabled);
		ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f,1.0f,1.0f,0.3f });
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
	FU::ImGui::tooltip(anime.collapseAll, text.collapseAll);

	ImGui::SameLine();
	ImGui::Spacing();
	ImGui::SameLine();

	//show
	this->showCodeButton();
	FU::ImGui::tooltip(anime.displayCode, text.displayCode);


	ImGui::EndChild();
}

void FS::Project::Explorer::showCodeButton() {
	auto* current = select.current();

	if (!current || current->type != FD::Project::FileList::Type::Supported) {
		ImVec4 colDisabled = ImGui::GetStyle().Colors[ImGuiCol_Button];
		ImGui::PushStyleColor(ImGuiCol_Button, colDisabled);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colDisabled);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, colDisabled);
		ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f,1.0f,1.0f,0.3f });
		ImGui::Button(ICON_MD_CODE);
		ImGui::PopStyleColor(4);
		return;
	}

	if (ImGui::Button(ICON_MD_CODE))
		this->displayCode();
}

void FS::Project::Explorer::tab() {
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

void FS::Project::Explorer::fluidumFiles() {

	this->mainCodeFile();

	ImGui::Separator();
	ImGui::Spacing();

	this->standardFluidumLibrary();

}

void FS::Project::Explorer::mainCodeFile() {
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

void FS::Project::Explorer::standardFluidumLibrary() {
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

	std::string makeLabel(const FD::Project::FileList::FileInfo* info) {
		std::string label{};
		if (info->type == FD::Project::FileList::Type::Directory) {
			label = info->open ? ICON_MD_FOLDER_OPEN " " : ICON_MD_FOLDER " ";
		}
		else if (info->type == FD::Project::FileList::Type::Supported) {
			label = ICON_FA_FILE_ALT  " ";
		}
		else if (info->type == FD::Project::FileList::Type::Unsupported) {
			label = ICON_FA_FILE  " ";
		}
		label += info->name;
		return label;
	}
}

std::pair<ImVec2, ImVec2> FS::Project::Explorer::projectFilesTree(std::vector<FD::Project::FileList::FileInfo>* node, FD::Project::FileList::FileInfo* info) {

	constexpr ImGuiDockNodeFlags nodeFlag =
		ImGuiTreeNodeFlags_OpenOnArrow |
		ImGuiTreeNodeFlags_SpanAvailWidth;


	const std::string label = makeLabel(info);

	ImGui::Spacing(); ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();

	ImGui::SetNextTreeNodeOpen(info->open, ImGuiCond_Always);

	const bool recurse = ImGui::TreeNodeEx(label.c_str(), nodeFlag);
	info->open = recurse;

	const ImRect nRect = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());

	if (select.projectFiles == info) {
		constexpr ImU32 col = FU::ImGui::convertImVec4ToImU32(0.27f, 0.67f, 0.87f, 0.14f);
		ImGui::GetWindowDrawList()->AddRectFilled({ nRect.Min.x ,nRect.Min.y - 2.0f }, { nRect.Max.x,nRect.Max.y + 2.0f }, col);

		if (changeName.popup) {
			using namespace FU::ImGui::Operators;
			changeName.pos = { nRect.Min.x + 48.0f ,nRect.Min.y - 1.0f };
			changeName.size = { nRect.Max.x - changeName.pos.x - 1.0f,nRect.Max.y - changeName.pos.y + 1.0f };
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
		if (info->type == FD::Project::FileList::Type::Directory)
			popup.dir = true;
		else if (info->type == FD::Project::FileList::Type::Supported)
			popup.supported = true;
	}

	//double click
	if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect(nRect.Min, nRect.Max)) {
		if (info->type == FD::Project::FileList::Type::Directory) {
			info->open ^= 1;//if opened -> close, if closed -> open 
			projectFilesWrite->save();
		}
		else if (info->type == FD::Project::FileList::Type::Supported) {
			using enum FD::Project::File::SupportedFileType;
			const auto type = FD::Project::File::getSupportedFileType(info->name);
			if (type == Lua)
				this->displayCode();
		}
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

void FS::Project::Explorer::projectFiles() {
	ImGui::BulletText(text.project);
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered()) {
		pos.selectedTree = ImGui::GetItemRectMin();
		popup.top = true;
		select.projectFiles = nullptr;
	}

	auto* data = projectFilesWrite->fileList();
	for (auto& x : *data) {
		if (x.name != ".fluidum")
			this->projectFilesTree(&x.dir_internal, &x);
	}
}

void FS::Project::Explorer::userFiles() {
	ImGui::BulletText(text.user);
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered()) {
		popup.top = true;
		select.userFiles = nullptr;
	}

	auto* data = userFilesWrite->fileList();
	for (auto& x : *data) {
		this->userFilesTree(&x.dir_internal, &x);
	}
}

std::pair<ImVec2, ImVec2> FS::Project::Explorer::userFilesTree(std::vector<FD::Project::FileList::FileInfo>* node, FD::Project::FileList::FileInfo* info) {

	constexpr ImGuiDockNodeFlags nodeFlag =
		ImGuiTreeNodeFlags_OpenOnArrow |
		ImGuiTreeNodeFlags_SpanAvailWidth;

	const std::string label = makeLabel(info);

	ImGui::Spacing(); ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();

	ImGui::SetNextTreeNodeOpen(info->open, ImGuiCond_Always);

	const bool recurse = ImGui::TreeNodeEx(label.c_str(), nodeFlag);
	info->open = recurse;

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
		if (info->type == FD::Project::FileList::Type::Directory)
			popup.dir = true;
		else if (info->type == FD::Project::FileList::Type::Supported)
			popup.supported = true;
	}

	//double click
	if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect(nRect.Min, nRect.Max)) {
		if (info->type == FD::Project::FileList::Type::Directory) {
			info->open ^= 1;//if opened -> close, if closed -> open 
			userFilesWrite->save();
		}
		else if (info->type == FD::Project::FileList::Type::Supported) {
			using enum FD::Project::File::SupportedFileType;
			const auto type = FD::Project::File::getSupportedFileType(info->name);
			if (type == Lua)
				this->displayCode();
		}
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

void FS::Project::Explorer::addDirectory() {
	assert(select.tab != TabType::Fluidum);

	const bool project = (select.tab == TabType::Project);
	const bool popupTop = (popup.type == PopupType::Top);
	const std::size_t maxSizeLimit = project ? FD::Project::Limits::ProjectFiles::DirectorySizeMax : FD::Project::Limits::UserFiles::DirectorySizeMax;

	FD::Project::FileList::DirectoryInfo dir{};

	//make temp path
	std::string path{};
	std::string tempName{};
	std::string parent{};

	if (popupTop) {
		if (project)
			parent = projectRead->getProjectFolderPath();
		else
			parent = "Fluidum:/";
	}
	else {
		parent = select.current()->path;
	}


	//limit
	{
		const std::size_t size = project ? projectFilesRead->numOfDirectories(parent) : userFilesRead->numOfVirtualFolder(parent);
		if (size >= maxSizeLimit) {
			FU::MB::error(text.error_maxSize);
			return;
		}
	}

	//make temp name
	{
		for (std::size_t i = 1; i <= FD::Project::Limits::ProjectFiles::DirectorySizeMax; i++) {
			tempName = "NewFolder" + std::to_string(i);
			if (!std::filesystem::exists(parent + tempName)) {
				path = parent + tempName;
				break;
			}
		}
	}

	if (path.empty()) {
		FU::MB::error(text.error_addDirectory);
		return;
	}

	if (!popupTop)
		select.current()->open = true;
	changeName.tempName = tempName;

	if (project) {

		if (!std::filesystem::create_directory(path)) {
			//failed
			FU::MB::error(text.error_addDirectory);
			return;
		}
		select.projectFiles = projectFilesWrite->add(popupTop ? std::string{} : parent, path, dir);
	}
	else {
		select.userFiles = userFilesWrite->add(popupTop ? std::string{} : parent, path, dir);
	}


	changeName.name = std::move(tempName);
	changeName.popup = true;
}

void FS::Project::Explorer::addFile() {
	assert(!add.info);

	const bool project = (select.tab == TabType::Project);
	const bool popupTop = (popup.type == PopupType::Top);

	Add::SharedInfo info{};

	if (project) {
		info.path = popupTop ? projectRead->getProjectFolderPath() : select.current()->path;
	}
	info.project = project;

	add.info = std::make_shared<Add::SharedInfo>(std::move(info));
	FluidumScene_Log_RequestAddScene("Project::NewFile");
	Scene::addScene<Add::NewFile>(add.info);

	add.popup = true;
}

void FS::Project::Explorer::addSelect() {
	assert(!add.info);

	const bool project = (select.tab == TabType::Project);
	const bool popupTop = (popup.type == PopupType::Top);

	Add::SharedInfo info{};

	if (project) {
		info.path = popupTop ? projectRead->getProjectFolderPath() : select.current()->path;
	}
	info.project = project;

	add.info = std::make_shared<Add::SharedInfo>(std::move(info));
	FluidumScene_Log_RequestAddScene("Project::NewFile");
	Scene::addScene<Add::Select>(add.info);

	add.popup = true;
}

void FS::Project::Explorer::addFileQuick() {
	assert(select.tab != TabType::Fluidum);

	const bool project = (select.tab == TabType::Project);

	FD::Project::FileList::UnsupportedInfo unsupported{};

	std::string path{};
	std::string tempName{};
	std::string parent{};

	const bool popupTop = (popup.type == PopupType::Top);

	if (popupTop) {
		parent = projectRead->getProjectFolderPath();
	}
	else {
		parent = select.current()->path;
	}

	//limit
	if (project) {
		if (projectFilesRead->numOfFiles(parent) >= FD::Project::Limits::ProjectFiles::FileSizeMax) {
			FU::MB::error(text.error_maxSize);
			return;
		}
	}
	else {
		if (userFilesRead->numOfFiles(parent) >= FD::Project::Limits::UserFiles::FileSizeMax) {
			FU::MB::error(text.error_maxSize);
			return;
		}
	}


	for (uint16_t i = 1; i <= std::numeric_limits<uint16_t>::max(); i++) {
		tempName = "NewFile" + std::to_string(i);
		if (!std::filesystem::exists(parent + tempName)) {
			path = parent + tempName;
			std::ofstream ofs(path);
			if (!ofs) {
				//failed
				FU::MB::error(text.error_addFile);
			}
			break;
		}
	}

	if (path.empty()) {
		FU::MB::error(text.error_addFile);
		return;
	}

	if (!popupTop)
		select.current()->open = true;
	changeName.tempName = tempName;

	if (project)
		select.projectFiles = projectFilesWrite->add(popupTop ? std::string{} : parent, path, unsupported);
	else
		select.userFiles = userFilesWrite->add(popupTop ? std::string{} : parent, path, unsupported);

	changeName.name = std::move(tempName);
	changeName.popup = true;
}

void FS::Project::Explorer::catchAdd() {
	if (!add.popup)
		return;

	if (add.info.use_count() != 1)
		return;

	//canceled
	if (!add.info->create) {
		add.info.reset();
		add.popup = false;
		return;
	}

	//created

	const bool project = (select.tab == TabType::Project);
	const bool popupTop = (popup.type == PopupType::Top);

	const std::string parent = popupTop ? std::string{} : select.current()->path;

	//add file
	if (add.info->type == Add::SharedInfo::Type::File) {
		if (FD::Project::File::isFileFormatSupported(add.info->path)) {
			FD::Project::FileList::SupportedInfo info{};
			if (project)
				select.projectFiles = projectFilesWrite->add(parent, add.info->path, info);
			else
				select.userFiles = userFilesWrite->add(parent, add.info->path, info);
		}
		else {
			FD::Project::FileList::UnsupportedInfo info{};
			if (project)
				select.projectFiles = projectFilesWrite->add(parent, add.info->path, info);
			else
				select.userFiles = userFilesWrite->add(parent, add.info->path, info);
		}
	}

	//add directory
	else if (add.info->type == Add::SharedInfo::Type::Directory) {
		FD::Project::FileList::DirectoryInfo info{};
		if (project)
			select.projectFiles = projectFilesWrite->add(parent, add.info->path, info);
		else
			select.userFiles = userFilesWrite->add(parent, add.info->path, info);
	}

	add.info.reset();
	add.popup = false;
}

void FS::Project::Explorer::removeDirectory() {
	assert(select.tab != TabType::Fluidum);

	const bool project = (select.tab == TabType::Project);

	//confirm
	{
		int32_t result;
		if (project) {
			result = FU::MB::ok_cancel(text.confirm_removeDirectory);
		}
		else {
			result = FU::MB::ok_cancel(text.confirm_releaseVirtualFolder);
		}
		assert(result == 0 || result == 1);
		if (result == 1) {//cancel
			return;
		}
	}

	if (project) {
		try {
			std::filesystem::remove_all(select.projectFiles->path);
		}
		catch (const std::filesystem::filesystem_error&) {
			FU::MB::error(text.error_removeDirectory);
			return;
		}

		projectFilesWrite->remove(select.projectFiles->path);
		projectFilesWrite->save();
		select.projectFiles = nullptr;
	}
	else {
		userFilesWrite->remove(select.userFiles->path);
		userFilesWrite->save();
		select.userFiles = nullptr;
	}

}

void FS::Project::Explorer::removeFile() {
	assert(select.tab != TabType::Fluidum);

	const bool project = (select.tab != TabType::Project);

	//confirm
	{
		int32_t result;
		if (project) {
			result = FU::MB::ok_cancel(text.confirm_removeFile);
		}
		else {
			result = FU::MB::ok_cancel(text.confirm_releaseFile);
		}
		assert(result == 0 || result == 1);
		if (result == 1) {//cancel
			return;
		}
	}

	if (project) {
		if (!std::filesystem::remove(select.projectFiles->path)) {
			FU::MB::error(text.error_removeFile);
			return;
		}
		projectFilesWrite->remove(select.projectFiles->path);
		projectFilesWrite->save();
		select.projectFiles = nullptr;
	}
	else {
		userFilesWrite->remove(select.userFiles->path);
		userFilesWrite->save();
		select.userFiles = nullptr;
	}

}

void FS::Project::Explorer::displayCode() {
	const std::string& path = select.current()->path;

	uintmax_t fileSize;

	try {
		if (!std::filesystem::exists(path)) {
			FU::MB::error(text.error_existFile);
			return;
		}
		fileSize = std::filesystem::file_size(path);
	}
	catch (const std::filesystem::filesystem_error&) {
		FU::MB::error(text.error_unexpected);
		return;
	}

	//too huge
	if (fileSize >= FD::Coding::Limits::FileSizeMax) {
		FluidumScene_Log_RequestAddScene("Utils::Message");
		Scene::addScene<Utils::Message>(text.error_tabFileSize, pos.selectedTree);
		return;
	}

	try {
		tabWrite->addFile(path);
	}
	catch (const FD::Coding::TabWrite::Exception val) {//exist
		using enum FD::Coding::TabWrite::Exception;
		if (val == AlreadyExist)
			return;
		else if (val == NotFound) {
			FU::MB::error(text.error_existFile);
			return;
		}
		else if (val == LimitFileSizeMax) {
			FU::MB::error(text.error_tabFileSize);
			return;
		}
	}

	if (!sceneRead->exist<TextEditor>()) {
		tabWrite->addDisplayFile(path);
		FluidumScene_Log_RequestAddScene("TextEditor");
		Scene::addScene<TextEditor>();
	}

	if (!sceneRead->exist<Coding::Tab>()) {
		FluidumScene_Log_RequestAddScene("Coding::Tab");
		Scene::addScene<Coding::Tab>();
	}
}

void FS::Project::Explorer::flipOpen() {
	assert(select.current() != nullptr);
	select.current()->open ^= 1;
	//save
	if (select.tab == TabType::Project)
		projectFilesWrite->save();
	else
		userFilesWrite->save();
}

void FS::Project::Explorer::collapseAll() {
	FU::Cursor::setCursorType(FU::Cursor::Type::Wait);

	if (select.tab == TabType::Fluidum) {
		luaFilesWrite->collapseAll();
		//..
	}
	else if (select.tab == TabType::Project) {
		projectFilesWrite->collapseAll();
		projectFilesWrite->save();
	}
	else if (select.tab == TabType::User) {
		userFilesWrite->collapseAll();
		userFilesWrite->save();
	}
}

void FS::Project::Explorer::openPopup() {
	//delay(one frame)
	this->changeNamePopup();

	if (popup.top) {
		ImGui::OpenPopup(popup.Top);
		popup.type = PopupType::Top;
		popup.top = false;
	}
	else if (popup.dir) {
		ImGui::OpenPopup(popup.Dir);
		popup.type = PopupType::Dir;
		popup.dir = false;
	}
	else if (popup.supported) {
		ImGui::OpenPopup(popup.Supported);
		popup.type = PopupType::Supported;
		popup.supported = false;
	}
	else if (changeName.popup) {
		ImGui::OpenPopup(popup.ChangeName);
		changeName.popup = false;
		changeName.once = true;
	}

	this->topPopup();
	this->directoryPopup();
	this->supportedPopup();

	//change name
	this->tryChangeName();
}

void FS::Project::Explorer::topPopup() {
	if (!ImGui::BeginPopup(popup.Top))
		return;

	//add file, folder, ...
	if (ImGui::BeginMenu(text.add_icon)) {
		this->popup_add();
		ImGui::EndMenu();
	}

	if (select.tab != TabType::Fluidum) {
		ImGui::Separator();

		if (ImGui::Selectable(text.collapseAll))
			this->collapseAll();
	}

	ImGui::EndPopup();
}

void FS::Project::Explorer::directoryPopup() {

	if (!ImGui::BeginPopup(popup.Dir))
		return;

	if (ImGui::Selectable(select.current()->open ? text.close : text.open))
		this->flipOpen();

	ImGui::Separator();

	if (ImGui::BeginMenu(text.add_icon)) {
		this->popup_add();
		ImGui::EndPopup();
	}

	ImGui::Separator();

	if (select.tab == TabType::Project) {
		if (ImGui::Selectable(text.remove))
			this->removeDirectory();
	}
	else if (select.tab == TabType::User) {
		if (ImGui::Selectable(text.release))
			this->removeDirectory();
	}

	ImGui::EndPopup();
}

void FS::Project::Explorer::supportedPopup() {
	if (!ImGui::BeginPopup(popup.Supported))
		return;

	if (ImGui::Selectable(text.displayCode))
		this->displayCode();

	ImGui::Separator();

	if (select.tab == TabType::Project) {
		if (ImGui::Selectable(text.remove))
			this->removeDirectory();
	}
	else if (select.tab == TabType::User) {
		if (ImGui::Selectable(text.release))
			this->removeDirectory();
	}

	ImGui::EndPopup();
}

void FS::Project::Explorer::popup_add() {
	const int32_t pop = this->popupSpacing();

	if (ImGui::Selectable(text.add_select))
		this->addSelect();

	if (ImGui::Selectable(text.add_selectNewFile))
		this->addFile();
	
	ImGui::Separator();

	if (select.tab == TabType::User) {
		if (ImGui::Selectable(text.virtualFolder))
			this->addDirectory();
	}
	else if (select.tab == TabType::Project) {
		if (ImGui::Selectable(text.directory_icon))
			this->addDirectory();
	}

	if (ImGui::Selectable(text.file_icon))
		this->addFileQuick();



	ImGui::PopStyleVar(pop);
}

void FS::Project::Explorer::changeNamePopup() {

	ImGui::SetNextWindowPos(changeName.pos);
	ImGui::SetNextWindowSize(changeName.size);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f,0.0f });
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0.0f,0.0f });

	ImGui::PushStyleColor(ImGuiCol_PopupBg, { 0.02f,0.02f,0.02f,1.0f });
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4());
	ImGui::PushStyleColor(ImGuiCol_Border, { 0.3f,0.3f,0.3f,1.0f });
	ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, { 0.1f,0.2f,0.96f,0.7f });

	if (ImGui::BeginPopup(popup.ChangeName)) {
		ImGui::SetWindowFontScale(1.05f);
		ImGui::Spacing(); ImGui::SameLine();
		ImGui::InputText("##ChangeName", changeName.name.data(), changeName.name.capacity(), ImGuiInputTextFlags_AutoSelectAll);

		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}


	ImGui::PopStyleVar(3);
	ImGui::PopStyleColor(4);
}

void FS::Project::Explorer::tryChangeName() {
	if (!changeName.once || ImGui::IsPopupOpen(popup.ChangeName))
		return;

	const bool project = (select.tab == TabType::Project);

	if (!this->checkChangeName()) {
		//save
		if (project)
			projectFilesWrite->save();
		else
			userFilesWrite->save();

		//reset
		changeName.name = std::string();
		changeName.name.reserve(100);

		changeName.once = false;
		return;
	}

	const std::string name = changeName.name.c_str();

	//reset
	changeName.name = std::string();
	changeName.name.reserve(100);

	changeName.once = false;

	//exist same name file -> false
	bool result;
	const std::string path = select.current()->path;

	std::string path_ = path;
	if (path_.back() == '/')
		path_.pop_back();

	//supported
	if (select.current()->type != FD::Project::FileList::Type::Directory &&
		FD::Project::File::isFileFormatSupported(name)
		)
	{
		select.current()->type = FD::Project::FileList::Type::Supported;
	}

	if (changeName.tempName == name) {
		//save
		if (project)
			projectFilesWrite->save();
		else
			userFilesWrite->save();
		return;
	}


	const std::string newPath = FU::File::directory(path_) + name;
	try {
		std::filesystem::rename(path, newPath);
	}
	catch (const std::filesystem::filesystem_error&) {
		FU::MB::error(text.error_changeName);
		return;
	}

	if (project) {
		result = projectFilesWrite->tryChangeName(path, name);
	}
	else {
		result = userFilesWrite->tryChangeName(path, name);
		assert(select.tab != TabType::Fluidum);
	}

	if (!result) {
		FU::MB::error(text.error_sameName);
		return;
	}

	//save
	if (project)
		projectFilesWrite->save();
	else
		userFilesWrite->save();
}

bool FS::Project::Explorer::checkChangeName() {
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

void FS::Project::Explorer::syncProjectFiles() {
	assert(select.tab != TabType::Fluidum);

	FU::Cursor::setCursorType(FU::Cursor::Type::Wait);

	if (select.tab == TabType::Project)
		projectFilesWrite->sync(projectRead->getProjectFolderPath());
	else if (select.tab == TabType::User)
		userFilesWrite->sync();
	else {
		FluidumScene_Log_Abort();
		abort();
	}
}

int32_t FS::Project::Explorer::popupSpacing() {
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { ImGui::GetStyle().ItemSpacing.x,ImGui::GetStyle().ItemSpacing.y * 2.2f });
	return 1;
}

