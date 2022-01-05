#include "select.h"
#include "check_path.h"
#include "directory.h"
#include <imgui_internal.h>
#include "../../Coding/TextEditor/texteditor.h"
#include "../../Utils/Popup/message.h"
#include "../../Utils/Popup/backwindow.h"
#include <nfd.h>

using namespace FU::ImGui::Operators;

FS::Project::Add::Select::Select(
	const FD::ProjectRead* const projectRead,
	const FD::GuiRead* const guiRead,
	const FD::SceneRead* const sceneRead,
	std::shared_ptr<SharedInfo>& sharedInfo
) :
	projectRead(projectRead),
	sceneRead(sceneRead),
	sharedInfo(sharedInfo),
	newImage(FDR::createImGuiImage(Resource::ProjectNewFilePath)),
	openImage(FDR::createImGuiImage(Resource::ProjectOpenFilePath)),
	dirImage(FDR::createImGuiImage(Resource::ProjectDirectoryPath))
{
	FluidumScene_Log_Constructor("Coding::Select");

	assert(sharedInfo);

	style.windowPos = guiRead->centerPos() - (guiRead->windowSize() / 3.0f);
	style.windowSize = guiRead->windowSize() - (style.windowPos * 2.0f);

	textEditor.SetReadOnly(true);

	style.project = sharedInfo->project;

	//capacity
	quickInfo.directoryPath.reserve(200);
	quickInfo.fileName.reserve(200);

	//default path -> Project/Src/
	quickInfo.directoryPath = sharedInfo->path;

	quickInfo.fileName = "main";

	GLog.add<FD::Log::Type::None>("Request add PopupBackWindowScene.");
	Scene::addScene<PopupBackWindow>();
}

FS::Project::Add::Select::~Select() {
	try {
		FluidumScene_Log_RequestDeleteScene("PopupBackWindow");
		Scene::deleteScene<PopupBackWindow>();

		FluidumScene_Log_Destructor("Coding::Select");
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

void FS::Project::Add::Select::call() {

	if (!sceneRead->exist_or<NewFile, Directory>())
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

	ImGui::Begin("Select", nullptr, flag);

	ImAnime::PushStyleVar(anime.counter, 0.5f, 0.0f, 1.0f, ImAnimeType::LINEAR, ImGuiStyleVar_Alpha);

	this->title();
	this->quick();
	ImGui::SameLine();

	ImGui::BeginChild("Right", { 0.0f,style.windowSize.y * 0.8f });
	this->right();
	this->bottomRight();
	ImGui::EndChild();

	this->bottom();

	ImAnime::PopStyleVar();

	ImGui::End();

	ImGui::PopStyleVar(3);

}

void FS::Project::Add::Select::title() {
	ImGui::BeginChild("SelectTop", ImVec2(style.windowSize.x, style.windowSize.y * 0.07f));

	ImGui::Text(text.selectPlease);
	ImGui::SetWindowFontScale(1.4f);

	ImGui::EndChild();
}

void FS::Project::Add::Select::quick() {
	const ImVec2 windowSize = ImVec2(style.windowSize.x * 0.7f, style.windowSize.y * 0.8f);

	ImGui::BeginChild("Template", ImVec2(style.windowSize.x * 0.6f, style.windowSize.y * 0.85f));
	ImGui::BeginChild("TemplateL", ImVec2(windowSize.x * 0.3f, windowSize.y), true);

	ImGui::Text(ICON_MD_SCHEDULE); ImGui::SameLine();
	ImGui::Text(text.quick);

	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.74f, 0.74f, 0.74f, 0.5f));

	ImGui::Separator(); ImGui::Spacing();

	ImGui::PopStyleColor();

	const ImVec2 size = { (windowSize.x * 0.3f) - (ImGui::GetStyle().WindowPadding.x * 2.0f), windowSize.y / 13.0f };

	this->selectTemplate(size);

	ImGui::EndChild(); ImGui::SameLine();
	ImGui::BeginChild("TemplateR", ImVec2(windowSize.x * 0.55f, windowSize.y), true);

	textEditor.Render("Template");

	ImGui::EndChild();
	ImGui::EndChild();
}

void FS::Project::Add::Select::selectTemplate(const ImVec2& size) {
	bool isColPushed = false;
	auto pushCol = [&]() {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.6f, 0.6f, 0.5f));
		this->isFileSelected = true;
		isColPushed = true;
	};
	auto popCol = [&]() {
		ImGui::PopStyleColor();
		this->isFileSelected = false;
		isColPushed = false;
	};

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.04f, 0.04f, 0.04f, 0.5f));

	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);

	//Empty_Lua
	if (selectType == Template::Empty_Lua) pushCol(); //color
	if (ImGui::Button(text.empty_lua, size)) {
		isFileSelected = true;
		this->selectType = Template::Empty_Lua;
		quickInfo.extension = ".lua";
		this->setEmptyFile();
	}
	if (isColPushed)	popCol();//color

	//Empty_Py
	if (selectType == Template::Empty_Py) pushCol(); //color
	if (ImGui::Button(text.empty_py, size)) {
		isFileSelected = true;
		this->selectType = Template::Empty_Py;
		quickInfo.extension = ".py";
		this->setEmptyFile();
	}
	if (isColPushed)	popCol();//color

	//Empty_Cpp
	if (selectType == Template::Empty_Cpp) pushCol(); //color
	if (ImGui::Button(text.empty_cpp, size)) {
		isFileSelected = true;
		this->selectType = Template::Empty_Cpp;
		quickInfo.extension = ".cpp";
		this->setEmptyFile();
	}
	if (isColPushed)	popCol();//color





	ImGui::PopStyleVar();

	ImGui::PopStyleColor();
}

void FS::Project::Add::Select::right() {
	const ImVec2 windowSize = ImVec2(style.windowSize.x * 0.25f, style.windowSize.y * 0.4f);
	ImGui::Spacing(); ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();
	ImGui::BeginChild("RightU", windowSize);

	const ImVec2 size = { windowSize.x , windowSize.y / 4.0f };

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());

	this->open(size); ImGui::Spacing();
	this->newFile(size); ImGui::Spacing();
	this->dir(size);

	ImGui::PopStyleVar();

	ImGui::EndChild();
}

std::pair<bool, bool> FS::Project::Add::Select::button(
	const char* label,
	const char* description,
	const FDR::ImGuiImage& image,
	const bool hovered,
	const ImVec2& size
) {
	std::pair<bool, bool> ret;

	const ImVec2 imageDymmy = { 0.0f, size.y * 0.15f };
	const ImVec2 textDymmy = { 0.0f, (size.y - ImGui::CalcTextSize(label).y) / 2.0f };

	if (hovered) {
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.44f, 0.44f, 0.44f, 1.0f));
		ImGui::BeginChild(label, size, true);
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.14f, 0.14f, 0.14f, 1.0f));
		ImGui::BeginChild(label, size, true);
	}

	ImGui::SetWindowFontScale(1.25f);

	ImGui::Dummy(imageDymmy);
	ImGui::Image(image, ImVec2(size.y * 0.7f, size.y * 0.7f));
	ImGui::SameLine();

	if (hovered) {
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.44f, 0.44f, 0.44f, 1.0f));
		ImGui::BeginChild("OpenR");
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.14f, 0.14f, 0.14f, 1.0f));
		ImGui::BeginChild("OpenR");
	}


	ImGui::Text(label);
	ImGui::PushFont(FDR::getDefaultFontMiniSize());
	ImGui::Text(description);
	ImGui::PopFont();

	ImGui::EndChild();
	ImGui::PopStyleColor();

	ret.first = ImGui::IsMouseHoveringRect(ImGui::GetWindowPos(), ImGui::GetWindowPos() + ImGui::GetWindowSize());

	ImGui::EndChild();
	ImGui::PopStyleColor();

	//click button 
	ret.second = ret.first && ImGui::IsMouseClicked(ImGuiMouseButton_Left);

	return ret;
}

void FS::Project::Add::Select::open(const ImVec2& size) {
	if (style.project)
		return;

	const auto [hovered, left] = this->button(
		text.openFile,
		text.openFileDescription,
		openImage,
		style.isOpenWindowHovered,
		size
	);

	style.isOpenWindowHovered = hovered;

	if (left) {
		this->openDialog();
		this->selectType = Template::None;
	}
}

void FS::Project::Add::Select::openDialog() {
	std::unique_ptr<nfdchar_t*> outPath = std::make_unique<nfdchar_t*>();
	GLog.add<FD::Log::Type::None>("Open file dialog.");
	const nfdresult_t result = NFD_OpenDialog(nullptr, nullptr, outPath.get());
	if (result == NFD_OKAY) {
		GLog.add<FD::Log::Type::None>("Open file({}).", *outPath.get());
	}
	else if (result == NFD_CANCEL) {
		GLog.add<FD::Log::Type::None>("Cancel file dialog.");
		return;
	}
	else {//NFD_ERROR
		GLog.add<FD::Log::Type::Error>("Error file dialog.");
		throw std::runtime_error("NFD_OpenDialog() return NFD_ERROR.");
	}

	//info
	{
		sharedInfo->type = SharedInfo::Type::Open;
		sharedInfo->path = *outPath.get();
		sharedInfo->create = true;
	}

	this->close();
}

void FS::Project::Add::Select::newFile(const ImVec2& size) {
	const auto [hovered, left] = this->button(
		text.newFile,
		text.newFileDescription,
		newImage,
		style.isNewFileWindowHovered,
		size
	);

	style.isNewFileWindowHovered = hovered;

	if (left && !ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
		this->selectType = Template::None;
		FluidumScene_Log_RequestAddScene("Project::Add::NewFile");
		Scene::addScene<NewFile>(this->sharedInfo);
	}
}

void FS::Project::Add::Select::dir(const ImVec2& size) {
	const auto [hovered, left] = this->button(
		text.newDirectory,
		text.newDirectoryDescription,
		dirImage,
		style.isDirWinfowHovered,
		size
	);

	style.isDirWinfowHovered = hovered;

	//double clicked -> [already added]
	if (left && !ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
		this->selectType = Template::None;
		FluidumScene_Log_RequestAddScene("Project::Add::Directory");
		Scene::addScene<Directory>(this->sharedInfo);
	}
}

void FS::Project::Add::Select::bottomRight() {
	if (selectType == Template::None) {
		anime.form.reset();
		return;
	}

	ImAnime::PushStyleVar(anime.form, 0.5f, 0.0f, 1.0f, ImAnimeType::LINEAR, ImGuiStyleVar_Alpha);

	ImGui::BeginChild("RightB");

	//directory path
	ImGui::BulletText(text.folderPath); ImGui::Spacing(); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.95f);
	ImGui::InputText("##FolderPath", quickInfo.directoryPath.data(), quickInfo.directoryPath.capacity());
	pos.directoryPath = ImGui::GetItemRectMin();

	ImGui::Spacing();

	//file path
	ImGui::BulletText(text.fileName); ImGui::Spacing(); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.6f);
	ImGui::InputText("##FileName", quickInfo.fileName.data(), quickInfo.fileName.capacity());
	pos.fileName = ImGui::GetItemRectMin();
	ImGui::SameLine();
	ImGui::Text(quickInfo.extension.c_str());

	ImGui::EndChild();

	ImAnime::PopStyleVar();
}

void FS::Project::Add::Select::bottom() {

	const ImVec2 buttonSize = ImVec2(ImGui::GetWindowSize().x * 0.2f, 0.0f);

	//cancel
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.05f, 0.05f, 0.5f));
	if (ImGui::Button(text.cancel, buttonSize)) {
		ImGui::CloseCurrentPopup();
		FluidumScene_Log_RequestDeleteScene("Coding::Select");
		Scene::deleteScene<Select>();
	}
	ImGui::PopStyleColor();


	if (this->selectType == Template::None)
		return;

	ImGui::SameLine();

	//make file
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.02f, 0.35f, 0.02f, 1.0f));
	if (ImGui::Button(text.select, buttonSize)) {
		//cursor wait
		FU::Cursor::setCursorType(FU::Cursor::Type::Wait);

		pos.create = ImGui::GetItemRectMin();

		if (checkQuickInfo()) {
			const bool success = this->createNewFileQuick();
			if (success)
				this->close();
		}
	}
	ImGui::PopStyleColor();
}

void FS::Project::Add::Select::setEmptyFile() {
	using namespace FD::File::Template;

	if (this->selectType == Template::Empty_Lua)
		textEditor.SetText(EmptyLua);
	else if (this->selectType == Template::Empty_Py)
		textEditor.SetText(EmptyPy);
	else if (this->selectType == Template::Empty_Cpp)
		textEditor.SetText(EmptyCpp);
	else
		abort();
}

bool FS::Project::Add::Select::createNewFileQuick() {
	using namespace FD::File::Template;

	std::ofstream ofs(quickInfo.fullPath, std::ios::out);

	if (!ofs || !std::filesystem::exists(quickInfo.fullPath)) {
		GLog.add<FD::Log::Type::None>("Error Failed to create file.");
		Scene::addScene<Utils::Message>(text.error_unexpected, pos.create);
		return false;
	}


	if (this->selectType == Template::Empty_Lua)
		ofs << EmptyLua << std::endl;
	else if (this->selectType == Template::Empty_Py)
		ofs << EmptyPy << std::endl;
	else if (this->selectType == Template::Empty_Cpp)
		ofs << EmptyCpp << std::endl;
	else {
		FluidumScene_Log_SeriousError_ThrowException();
	}

	//info
	{
		sharedInfo->type = SharedInfo::Type::File;
		sharedInfo->path = std::move(quickInfo.fullPath);
		sharedInfo->create = true;
	}

	return true;
}

bool FS::Project::Add::Select::checkQuickInfo() {

	std::string directoryPath = this->quickInfo.directoryPath.data();
	std::string fileName = this->quickInfo.fileName.data();
	std::string extension = this->quickInfo.extension.data();

	CheckPath::Info info{
		.project = sharedInfo->project,
		.directory = false,
		.parent = directoryPath,
		.name = fileName + extension,
		.pos_name = pos.fileName,
		.pos_path = pos.directoryPath,
		.pos_create = pos.create,
	};

	FluidumScene_Log_CallSceneConstructor("Project::Add::CheckPath");
	Scene::callConstructor<CheckPath>(info);

	if (!info.noerror) {
		quickInfo.fullPath = std::move(info.fullPath);
		return true;
	}

	//error
	return false;
}

void FS::Project::Add::Select::close() {
	FluidumScene_Log_RequestDeleteScene("Coding::Select");
	Scene::deleteScene<Select>();
	FluidumScene_Log_RequestAddScene("TextEditor");
	Scene::addScene<TextEditor>();
}
