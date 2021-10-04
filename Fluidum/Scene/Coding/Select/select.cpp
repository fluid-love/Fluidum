#include "select.h"
#include <imgui_internal.h>
#include "../TextEditor/texteditor.h"

using namespace FU::ImGui::Operators;

FS::CodingSelect::CodingSelect(const FD::ProjectRead* const projectRead, FD::ProjectWrite* const projectWrite, const FD::GuiRead* const guiRead)
	: projectRead(projectRead), projectWrite(projectWrite), guiRead(guiRead),
	newImage(FDR::createImGuiImage(Resource::CodingNewFilePath)), openImage(FDR::createImGuiImage(Resource::CodingOpenFilePath))
{
	GLog.add<FD::Log::Type::None>("Construct CodingSelectScene.");

	style.windowPos = guiRead->centerPos() - (guiRead->windowSize() / 3.0f);
	style.windowSize = guiRead->windowSize() - (style.windowPos * 2.0f);

	//templateの表示に使うため書きかえは不可
	textEditor.SetReadOnly(true);

	//capacity
	quickInfo.folderPath.reserve(200);
	quickInfo.fileName.reserve(200);

	//デフォルトでパスはプロジェクトフォルダ/Code
	quickInfo.folderPath = projectRead->getProjectFolderPath() + "Code/";
	quickInfo.fileName = "main";

}

FS::CodingSelect::~CodingSelect() {
	try {
		GLog.add<FD::Log::Type::None>("Destruct CodingSelectScene.");
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

void FS::CodingSelect::call() {

	ImGui::SetNextWindowPos(style.windowPos);
	ImGui::SetNextWindowSize(style.windowSize);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);

	constexpr auto flag =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove;

	ImGui::OpenPopup("Select");
	if (ImGui::BeginPopupModal("Select", nullptr, flag)) {

		this->create();
		this->quick();
		ImGui::SameLine();

		ImGui::BeginChild("Right", { 0.0f,style.windowSize.y * 0.8f });
		this->right();
		this->bottomRight();
		ImGui::EndChild();

		this->bottom();

		ImGui::EndPopup();
	}

	ImGui::PopStyleVar(3);

}

void FS::CodingSelect::create() {
	ImGui::BeginChild("SelectTop", ImVec2(style.windowSize.x, style.windowSize.y * 0.07f));

	ImGui::Text(text.selectPlease);
	ImGui::SetWindowFontScale(1.4f);

	ImGui::EndChild();
}

void FS::CodingSelect::quick() {
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

void FS::CodingSelect::selectTemplate(const ImVec2& size) {
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

	//Empty_AS
	if (selectType == Template::Empty_AS) pushCol(); //color
	if (ImGui::Button(text.empty_as, size)) {
		isFileSelected = true;
		this->selectType = Template::Empty_AS;
		quickInfo.extension = ".as";
		this->setEmptyFile();
	}
	if (isColPushed)	popCol();//color





	ImGui::PopStyleVar();

	ImGui::PopStyleColor();
}

void FS::CodingSelect::right() {
	const ImVec2 windowSize = ImVec2(style.windowSize.x * 0.25f, style.windowSize.y * 0.4f);
	ImGui::Spacing(); ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();
	ImGui::BeginChild("RightU", windowSize);

	const ImVec2 size = { windowSize.x , windowSize.y / 4.0f };

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());

	this->open(size);
	this->newFile(size);

	ImGui::PopStyleVar();

	ImGui::EndChild();
}

void FS::CodingSelect::open(const ImVec2& size) {
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
}

void FS::CodingSelect::newFile(const ImVec2& size) {
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
}

namespace FS {
	int inputTextCallback(ImGuiInputTextCallbackData* data) {
		std::string* str = static_cast<std::string*>(data->UserData);
		if (str->size() == data->BufTextLen)
			return 0;

		str->resize(data->BufTextLen);

		return 0;
	}
}

void FS::CodingSelect::bottomRight() {
	//クイックのときのみ有効
	if (selectType == Template::None)
		return;

	ImGui::BeginChild("RightB");

	ImGui::BulletText(text.folderPath); ImGui::Spacing(); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.95f);
	ImGui::InputText("##FolderPath", quickInfo.folderPath.data(), quickInfo.folderPath.capacity(), ImGuiInputTextFlags_CallbackAlways, inputTextCallback, &quickInfo.folderPath);

	ImGui::Spacing();

	ImGui::BulletText(text.fileName); ImGui::Spacing(); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.6f);
	ImGui::InputText("##FileName", quickInfo.fileName.data(), quickInfo.fileName.capacity(), ImGuiInputTextFlags_CallbackAlways, inputTextCallback, &quickInfo.fileName);
	ImGui::SameLine();
	ImGui::Text(quickInfo.extension.c_str());

	ImGui::EndChild();
}

void FS::CodingSelect::bottom() {

	const ImVec2 buttonSize = ImVec2(ImGui::GetWindowSize().x * 0.2f, 0.0f);

	//キャンセルボタン　赤字
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.05f, 0.05f, 0.5f));
	if (ImGui::Button(text.cancel, buttonSize)) {
		ImGui::CloseCurrentPopup();//popupを終了	
		GLog.add<FD::Log::Type::None>("Request delete CodingSelectScene.");
		Scene::deleteScene<CodingSelect>();//シーンを消す
	}
	ImGui::PopStyleColor();


	if (this->selectType == Template::None)
		return;

	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.7f, 0.1f, 1.0f));
	if (ImGui::Button(text.select, buttonSize)) {//作成ボタン

		//作成する前に確認する
		if (checkQuickInfo()) {//問題なし
			this->createNewFileQuick();
			ImGui::CloseCurrentPopup();//popupを終了			
			GLog.add<FD::Log::Type::None>("Request delete CodingSelectScene.");
			Scene::deleteScene<CodingSelect>();//シーンを消す
			GLog.add<FD::Log::Type::None>("Request add CodingScene.");
			Scene::addScene<TextEditor>();
		}
	}
	ImGui::PopStyleColor();
}

namespace FS::Internal::Coding {
	constexpr const char* EmptyLua = "--EmptyFile";
	constexpr const char* EmptyPy = "#EmptyFile";
	constexpr const char* EmptyAS = "//EmptyFile";
}

void FS::CodingSelect::setEmptyFile() {
	using namespace Internal::Coding;

	if (this->selectType == Template::Empty_Lua)
		textEditor.SetText(EmptyLua);
	else if (this->selectType == Template::Empty_Py)
		textEditor.SetText(EmptyPy);
	else if (this->selectType == Template::Empty_AS)
		textEditor.SetText(EmptyAS);
	else
		abort();
}

namespace FS {

	enum class ErrorType : uint8_t {
		None,
		EmptyFileName,
		EmptyFolderPath,
		AlreadyExist,
		NotFound
	};

	std::pair<ErrorType, std::string> checkFile(const std::string& folderPath, const std::string& fileName, const std::string& extension) {
		//空かどうか
		if (folderPath.empty())
			return { ErrorType::EmptyFolderPath,{} };

		if (fileName.empty())
			return { ErrorType::EmptyFileName,{} };

		//フォルダーが存在するか
		bool result = !std::filesystem::is_directory(folderPath);
		if (result)
			return { ErrorType::NotFound,{} };

		std::string filePath = folderPath + fileName + extension;
		result = std::filesystem::exists(filePath);
		if (result)
			return { ErrorType::AlreadyExist,filePath };


		return { ErrorType::None,filePath };
	}

	//入力された文字列に'/'があればそのまま，なければつける
	void tryAddSlash(std::string& path) {
		if (path.back() != '/' || path.back() != '\\')
			path.push_back('/');
	}
}

void FS::CodingSelect::createNewFileQuick() {
	using namespace Internal::Coding;

	std::ofstream ofs(quickInfo.fullPath, std::ios::out);

	if (!ofs)
		throw std::runtime_error("Failed to create file.");

	//コードを入れる
	if (this->selectType == Template::Empty_Lua)
		ofs << Internal::Coding::EmptyLua << std::endl;
	else if (this->selectType == Template::Empty_Py)
		ofs << Internal::Coding::EmptyPy << std::endl;
	else if (this->selectType == Template::Empty_AS)
		ofs << Internal::Coding::EmptyAS << std::endl;
	else
		abort();

	GLog.add<FD::Log::Type::None>("Set MainCodeFile({}).", quickInfo.fullPath);
	projectWrite->setMainCodePath(quickInfo.fullPath.c_str());
}

bool FS::CodingSelect::checkQuickInfo() {
	GLog.add<FD::Log::Type::None>("Check QuickInfo.");

	tryAddSlash(quickInfo.folderPath);
	auto [err, path] = checkFile(quickInfo.folderPath, quickInfo.fileName, quickInfo.extension);

	//問題なければtrue
	if (err == ErrorType::None) {
		GLog.add<FD::Log::Type::None>("QuickInfo NoError.");
		quickInfo.fullPath = std::move(path);
		return true;
	}

	//問題があれば問題に応じた文字を出す
	if (err == ErrorType::EmptyFolderPath) {
		GLog.add<FD::Log::Type::None>("Error EmptyFolderPath.");
		error.errorPopupMessage = text.error_emptyForm;
	}
	else if (err == ErrorType::EmptyFileName) {
		GLog.add<FD::Log::Type::None>("Error EmptyFileName.");
		error.errorPopupMessage = text.error_emptyForm;
	}
	else if (err == ErrorType::NotFound) {
		GLog.add<FD::Log::Type::None>("Error NotFoundDirectory. Typed directory is {}.", quickInfo.folderPath);
		error.errorPopupMessage = text.error_notFoundDirectory;
	}
	else if (err == ErrorType::AlreadyExist) {
		GLog.add<FD::Log::Type::None>("Error AlreadyExist. Typed filename is {}({}).", quickInfo.fileName, path);
		error.errorPopupMessage = text.error_notFoundDirectory;
	}
	return false;
}
