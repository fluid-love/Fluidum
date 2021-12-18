#include "select.h"
#include <imgui_internal.h>
#include "../TextEditor/texteditor.h"
#include "new.h"
#include "../../Utils/Popup/message.h"
#include "../../Utils/Popup/backwindow.h"
#include <nfd.h>

using namespace FU::ImGui::Operators;

FS::Coding::Select::Select(
	FD::Coding::TabWrite* const tabWrite,
	const FD::ProjectRead* const projectRead,
	FD::ProjectWrite* const projectWrite,
	const FD::FluidumFilesRead* const fluidumFilesRead,
	FD::FluidumFilesWrite* const fluidumFilesWrite,
	const FD::GuiRead* const guiRead,
	const FD::SceneRead* const sceneRead
) :
	tabWrite(tabWrite),
	projectRead(projectRead),
	projectWrite(projectWrite),
	fluidumFilesRead(fluidumFilesRead),
	fluidumFilesWrite(fluidumFilesWrite),
	sceneRead(sceneRead),
	newImage(FDR::createImGuiImage(Resource::CodingNewFilePath)), openImage(FDR::createImGuiImage(Resource::CodingOpenFilePath))
{
	FluidumScene_Log_Constructor("Coding::Select");

	style.windowPos = guiRead->centerPos() - (guiRead->windowSize() / 3.0f);
	style.windowSize = guiRead->windowSize() - (style.windowPos * 2.0f);

	textEditor.SetReadOnly(true);

	//capacity
	quickInfo.folderPath.reserve(200);
	quickInfo.fileName.reserve(200);

	//default path -> Project/Src/
	quickInfo.folderPath = projectRead->getProjectFolderPath() + "Src/";
	quickInfo.fileName = "main";

	GLog.add<FD::Log::Type::None>("Request add PopupBackWindowScene.");
	Scene::addScene<PopupBackWindow>();
}

FS::Coding::Select::~Select() {
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

void FS::Coding::Select::call() {

	if (!sceneRead->isExist<Coding::New>())
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

	this->create();
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

void FS::Coding::Select::create() {
	ImGui::BeginChild("SelectTop", ImVec2(style.windowSize.x, style.windowSize.y * 0.07f));

	ImGui::Text(text.selectPlease);
	ImGui::SetWindowFontScale(1.4f);

	ImGui::EndChild();
}

void FS::Coding::Select::quick() {
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

void FS::Coding::Select::selectTemplate(const ImVec2& size) {
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

void FS::Coding::Select::right() {
	const ImVec2 windowSize = ImVec2(style.windowSize.x * 0.25f, style.windowSize.y * 0.4f);
	ImGui::Spacing(); ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();
	ImGui::BeginChild("RightU", windowSize);

	const ImVec2 size = { windowSize.x , windowSize.y / 4.0f };

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());

	this->open(size); ImGui::Spacing();
	this->newFile(size);

	ImGui::PopStyleVar();

	ImGui::EndChild();
}

void FS::Coding::Select::open(const ImVec2& size) {
	const ImVec2 imageDymmy = { 0.0f, size.y * 0.15f };
	const ImVec2 textDymmy = { 0.0f, (size.y - ImGui::CalcTextSize(text.openFile).y) / 2.0f };

	if (style.isOpenWindowHovered) {
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.44f, 0.44f, 0.44f, 1.0f));
		ImGui::BeginChild("Open", size, true);
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.14f, 0.14f, 0.14f, 1.0f));
		ImGui::BeginChild("Open", size, true);
	}

	ImGui::SetWindowFontScale(1.25f);

	ImGui::Dummy(imageDymmy);
	ImGui::Image(openImage, ImVec2(size.y * 0.7f, size.y * 0.7f));
	ImGui::SameLine();

	if (style.isOpenWindowHovered) {
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.44f, 0.44f, 0.44f, 1.0f));
		ImGui::BeginChild("OpenR");
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.14f, 0.14f, 0.14f, 1.0f));
		ImGui::BeginChild("OpenR");
	}


	ImGui::Text(text.openFile);
	ImGui::PushFont(FDR::getDefaultFontMiniSize());
	ImGui::Text(text.openFileDescription);
	ImGui::PopFont();

	ImGui::EndChild();
	ImGui::PopStyleColor();

	style.isOpenWindowHovered = ImGui::IsMouseHoveringRect(ImGui::GetWindowPos(), ImGui::GetWindowPos() + ImGui::GetWindowSize());

	ImGui::EndChild();
	ImGui::PopStyleColor();

	//click button 
	if (style.isOpenWindowHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		this->openDialog();
}

void FS::Coding::Select::openDialog() {
	std::unique_ptr<nfdchar_t*> outPath = std::make_unique<nfdchar_t*>();
	GLog.add<FD::Log::Type::None>("Open file dialog.");
	const nfdresult_t result = NFD_OpenDialog("lua,py,cpp", NULL, outPath.get());
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

	//NFD_OKAY
	if (!fluidumFilesRead->isMainCodeFileExist()) {
		GLog.add<FD::Log::Type::None>("Set MainCodeFile({}).", quickInfo.fullPath);
		fluidumFilesWrite->setMainCodePath(quickInfo.fullPath.c_str());
		fluidumFilesWrite->save();
	}

	tabWrite->addDisplayFile(quickInfo.fullPath);
	tabWrite->addFile(quickInfo.fullPath);
	tabWrite->save();
	this->close();
}

void FS::Coding::Select::newFile(const ImVec2& size) {
	const ImVec2 imageDymmy = { 0.0f, size.y * 0.15f };
	const ImVec2 textDymmy = { 0.0f, (size.y - ImGui::CalcTextSize(text.newFile).y) / 2.0f };

	if (style.isNewFileWindowHovered) {
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.44f, 0.44f, 0.44f, 1.0f));
		ImGui::BeginChild("New", size, true);
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.14f, 0.14f, 0.14f, 1.0f));
		ImGui::BeginChild("New", size, true);
	}

	ImGui::SetWindowFontScale(1.25f);

	ImGui::Dummy(imageDymmy);
	ImGui::Image(newImage, ImVec2(size.y * 0.7f, size.y * 0.7f));
	ImGui::SameLine();

	if (style.isNewFileWindowHovered) {
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.44f, 0.44f, 0.44f, 1.0f));
		ImGui::BeginChild("NewR");
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.14f, 0.14f, 0.14f, 1.0f));
		ImGui::BeginChild("NewR");
	}


	ImGui::Text(text.newFile);
	ImGui::PushFont(FDR::getDefaultFontMiniSize());
	ImGui::Text(text.newFileDescription);
	ImGui::PopFont();

	ImGui::EndChild();
	ImGui::PopStyleColor();

	style.isNewFileWindowHovered = ImGui::IsMouseHoveringRect(ImGui::GetWindowPos(), ImGui::GetWindowPos() + ImGui::GetWindowSize());

	ImGui::EndChild();
	ImGui::PopStyleColor();

	//click button 
	if (style.isNewFileWindowHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
		GLog.add<FD::Log::Type::None>("Request add Coding::NewScene.");
		Scene::addScene<Coding::New>();
	}
}

void FS::Coding::Select::bottomRight() {
	if (selectType == Template::None)
		return;

	ImGui::BeginChild("RightB");

	ImGui::BulletText(text.folderPath); ImGui::Spacing(); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.95f);
	ImGui::InputText("##FolderPath", quickInfo.folderPath.data(), quickInfo.folderPath.capacity(), ImGuiInputTextFlags_CallbackAlways, inputTextCallback, &quickInfo.folderPath);
	pos.folderPath = ImGui::GetItemRectMin();

	ImGui::Spacing();

	ImGui::BulletText(text.fileName); ImGui::Spacing(); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.6f);
	ImGui::InputText("##FileName", quickInfo.fileName.data(), quickInfo.fileName.capacity(), ImGuiInputTextFlags_CallbackAlways, inputTextCallback, &quickInfo.fileName);
	pos.fileName = ImGui::GetItemRectMin();
	ImGui::SameLine();
	ImGui::Text(quickInfo.extension.c_str());

	ImGui::EndChild();
}

void FS::Coding::Select::bottom() {

	const ImVec2 buttonSize = ImVec2(ImGui::GetWindowSize().x * 0.2f, 0.0f);

	//cancel
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.05f, 0.05f, 0.5f));
	if (ImGui::Button(text.cancel, buttonSize)) {
		ImGui::CloseCurrentPopup();	
		FluidumScene_Log_RequestDeleteScene("Coding::Select");
		Scene::deleteScene<Coding::Select>();
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
			this->createNewFileQuick();
			this->close();
		}
	}
	ImGui::PopStyleColor();
}

void FS::Coding::Select::setEmptyFile() {
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

void FS::Coding::Select::createNewFileQuick() {
	using namespace FD::File::Template;

	std::ofstream ofs(quickInfo.fullPath, std::ios::out);

	if (!ofs)
		throw std::runtime_error("Failed to create file.");

	if (this->selectType == Template::Empty_Lua)
		ofs << EmptyLua << std::endl;
	else if (this->selectType == Template::Empty_Py)
		ofs << EmptyPy << std::endl;
	else if (this->selectType == Template::Empty_Cpp)
		ofs << EmptyCpp << std::endl;
	else
		abort();

	if (!fluidumFilesRead->isMainCodeFileExist()) {
		GLog.add<FD::Log::Type::None>("Set MainCodeFile({}).", quickInfo.fullPath);
		fluidumFilesWrite->setMainCodePath(quickInfo.fullPath);
	}
	tabWrite->addFile(quickInfo.fullPath);
	tabWrite->addDisplayFile(quickInfo.fullPath);
	tabWrite->save();
}

bool FS::Coding::Select::checkQuickInfo() {

	GLog.add<FD::Log::Type::None>("Check QuickInfo.");

	FU::File::tryPushSlash(quickInfo.folderPath);
	auto [err, path] = Check::checkFile(quickInfo.folderPath, quickInfo.fileName, quickInfo.extension);

	//no_error -> return true
	if (err == Check::ErrorType::None) {
		GLog.add<FD::Log::Type::None>("QuickInfo NoError.");
		quickInfo.fullPath = std::move(path);
		return true;
	}

	//error
	if (err == Check::ErrorType::EmptyFolderPath) {
		GLog.add<FD::Log::Type::None>("Error EmptyFolderPath.");
		FluidumScene_Log_RequestAddScene("Utils::Message");
		Scene::addScene<Utils::Message>(text.error_emptyForm, pos.folderPath);
	}
	else if (err == Check::ErrorType::EmptyFileName) {
		GLog.add<FD::Log::Type::None>("Error EmptyFileName.");
		FluidumScene_Log_RequestAddScene("Utils::Message");
		Scene::addScene<Utils::Message>(text.error_emptyForm, pos.fileName);
	}
	else if (err == Check::ErrorType::NotFound) {
		GLog.add<FD::Log::Type::None>("Error NotFoundDirectory. Typed directory is {}.", quickInfo.folderPath);
	}
	else if (err == Check::ErrorType::AlreadyExist) {
		GLog.add<FD::Log::Type::None>("Error AlreadyExist. Typed filename is {}({}).", quickInfo.fileName, path);
		FluidumScene_Log_RequestAddScene("Utils::Message");
		Scene::addScene<Utils::Message>(text.error_alreadyExistFile, pos.create);
	}
	return false;
}

void FS::Coding::Select::close() {
	FluidumScene_Log_RequestDeleteScene("Coding::Select");
	Scene::deleteScene<Coding::Select>();
	FluidumScene_Log_RequestAddScene("TextEditor");
	Scene::addScene<TextEditor>();
}

int FS::Coding::Select::inputTextCallback(ImGuiInputTextCallbackData* data) {
	std::string* str = static_cast<std::string*>(data->UserData);
	if (str->size() == data->BufTextLen)
		return 0;

	str->resize(data->BufTextLen);

	return 0;
}

std::pair<FS::Coding::Select::Check::ErrorType, std::string> FS::Coding::Select::Check::checkFile(const std::string& folderPath, const std::string& fileName, const std::string& extension) {

	if (folderPath.empty()) {
		return { ErrorType::EmptyFolderPath,{} };
	}

	if (fileName.empty())
		return { ErrorType::EmptyFileName,{} };

	//folder
	bool result = !std::filesystem::is_directory(folderPath);
	if (result)
		return { ErrorType::NotFound,{} };

	std::string filePath = folderPath + fileName + extension;
	result = std::filesystem::exists(filePath);
	if (result)
		return { ErrorType::AlreadyExist,filePath };


	return { ErrorType::None,filePath };
}
