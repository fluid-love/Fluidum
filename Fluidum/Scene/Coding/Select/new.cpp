#include "new.h"
#include <imgui_internal.h>
#include "../TextEditor/texteditor.h"
#include "../../Utils/Popup/message.h"
#include <nfd.h>

using namespace FU::ImGui::Operators;

namespace FS::Coding::Internal {
	std::vector<FDR::ImGuiImage> makeImages() {
		constexpr const char* names[] = {
			"lua.png",
			"python.png",
			"angelscript.png"
		};
		std::vector<FDR::ImGuiImage> result;
		for (uint16_t i = 0; i < std::extent_v<decltype(names), 0>; i++) {
			std::string path = Resource::CodingNewIconsFolderPath;
			path += names[i];
			result.emplace_back(FDR::createImGuiImage(path.c_str()));
		}

		return result;
	}
}

FS::Coding::New::New(
	FD::Coding::TabWrite* const tabWrite,
	const FD::ProjectRead* const projectRead,
	FD::ProjectWrite* const projectWrite,
	const FD::FluidumFilesRead* const fluidumFilesRead,
	FD::FluidumFilesWrite* const fluidumFilesWrite,
	const FD::GuiRead* const guiRead,
	const FD::Log::FileRead* const fileRead
) :
	tabWrite(tabWrite),
	projectRead(projectRead),
	projectWrite(projectWrite),
	fluidumFilesRead(fluidumFilesRead),
	fluidumFilesWrite(fluidumFilesWrite),
	recentButtons(initRecentFileTypes(fileRead->recent())),
	emptyFiles({
			ButtonInfo{images.at(0), "_Empty", text.empty, text.empty_Description},
			ButtonInfo{images.at(1), "_ELua", text.emptyLua, text.emptyLua_Description},
			ButtonInfo{images.at(2), "_EPy", text.emptyPython, text.emptyPython_Description},
			ButtonInfo{images.at(3), "_EAS", text.emptyAngelScript, text.emptyAngelScript_Description}

		})
{
	GLog.add<FD::Log::Type::None>("Construct Coding::NewScene.");

	style.windowPos = guiRead->centerPos() - (guiRead->windowSize() / 3.0f);
	style.windowSize = guiRead->windowSize() - (style.windowPos * 2.0f);

	style.buttonSize = ImVec2(style.windowSize.x / 2.2f, style.windowSize.y * 0.09f);

	//capacity
	folderPath.reserve(200);
	fileName.reserve(200);

	//default
	folderPath = projectRead->getProjectFolderPath() + "Src/";

}

FS::Coding::New::~New() {
	try {
		GLog.add<FD::Log::Type::None>("Destruct Coding::NewScene.");
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

void FS::Coding::New::call() {

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

	//animation
	ImAnime::PushStyleVar(anime.counter, 0.5f, 0.0f, 1.0f, ImAnimeType::LINEAR, ImGuiStyleVar_Alpha);

	ImGui::Begin("CodingNew", nullptr, flag);

	this->title();

	ImGui::Spacing(); ImGui::Spacing();

	this->recent();
	ImGui::SameLine();
	this->list();

	ImGui::Spacing();
	this->form();
	ImGui::Spacing();

	this->bottom();

	ImGui::End();

	ImAnime::PopStyleVar();
	ImGui::PopStyleVar(3);
}

void FS::Coding::New::title() {
	ImGui::BeginChild("Title1", ImVec2(style.windowSize.x / 2.0f, style.windowSize.y * 0.07f));
	ImGui::SetWindowFontScale(1.7f);

	ImGui::Text(projectType.c_str());
	ImGui::EndChild();

	ImGui::Separator();

	ImGui::BeginChild("Title2", ImVec2(style.windowSize.x / 2.0f, style.windowSize.y * 0.07f));
	ImGui::SetWindowFontScale(1.3f);

	ImGui::Text("text");

	ImGui::EndChild();
}

void FS::Coding::New::recent() {

	ImGui::BeginChild("Recent", ImVec2(style.windowSize.x / 2.5f, style.windowSize.y / 2.0f));

	ImGui::Text(text.recent);
	ImGui::SetWindowFontScale(1.35f);
	ImGui::Spacing();

	ImGui::BeginChild("RecentInner");
	ImGui::PushFont(FDR::getDefaultFontMiniSize());

	if (recentButtons.empty())
		ImGui::TextDisabled(text.recent_empty);
	else {
		for (auto& x : recentButtons)
			bool click = this->button(x.image, x.label, x.title, x.description);
	}

	ImGui::PopFont();
	ImGui::EndChild();

	ImGui::EndChild();
}

void FS::Coding::New::list() {
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());

	//slect空間全体
	ImGui::BeginChild("Select", ImVec2(style.windowSize.x / 2.0f, style.windowSize.y * 0.7f), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	//dummy空間
	ImGui::BeginChild("SelectU", ImVec2(0.0f, style.windowSize.y * 0.07f), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
	this->filter();

	ImGui::EndChild();

	ImGui::BeginChild("SelectB", ImVec2(style.windowSize.x / 2.0f, style.windowSize.y * 0.7f), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);


	//buttons
	this->buttons();

	ImGui::EndChild();//bottom

	ImGui::EndChild();//全体

	ImGui::PopStyleVar(2);
}

void FS::Coding::New::filter() {
	int32_t layoutIndex = 0;

	//constexpr const char* icons[] = { ICON_MD_INBOX" ",ICON_FA_DNA"  " };
	//std::string empty = icons[0];
	//empty += text.empty;
	//std::string algo = icons[1];
	//algo += text.algorithm;

	//const char* filter1[3] = { text.all, empty.data() , algo.data() };

	//ImGui::Text(ICON_FA_FILTER); ImGui::SameLine();
	//ImGui::SetNextItemWidth(ImGui::GetWindowSize().x * 0.3f);


	//if (ImGui::BeginCombo("##filter", filter1[layoutIndex], ImGuiComboFlags_HeightLargest)) {
	//	for (int n = 0; n < IM_ARRAYSIZE(filter1); n++) {
	//		const bool is_selected = (layoutIndex == n);
	//		if (ImGui::Selectable(filter1[n], is_selected))
	//			layoutIndex = n;

	//		if (is_selected)
	//			ImGui::SetItemDefaultFocus();
	//	}
	//	ImGui::EndCombo();
	//}
}

void FS::Coding::New::buttons() {

	ImGui::BulletText(text.empty);
	for (auto& x : emptyFiles) {
		bool click = this->button(x.image, x.label, x.title, x.description);
		//this->formScene(click, x.title);
	}

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

void FS::Coding::New::form() {
	if (!select.ptr)
		return;

	ImGui::BeginChild("Form");

	ImGui::BulletText("test"); ImGui::Spacing(); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.95f);
	ImGui::InputText("##FolderPath", folderPath.data(), folderPath.capacity(), ImGuiInputTextFlags_CallbackAlways, inputTextCallback, &folderPath);
	pos.folderPath = ImGui::GetItemRectMin();

	ImGui::Spacing();

	ImGui::BulletText("sile"); ImGui::Spacing(); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.6f);
	ImGui::InputText("##FileName", fileName.data(), fileName.capacity(), ImGuiInputTextFlags_CallbackAlways, inputTextCallback, &fileName);
	pos.fileName = ImGui::GetItemRectMin();
	ImGui::SameLine();
	ImGui::Text(extension.c_str());

	ImGui::EndChild();
}

void FS::Coding::New::bottom() {

	const ImVec2 buttonSize = ImVec2(ImGui::GetWindowSize().x * 0.2f, 0.0f);

	//cancel
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.05f, 0.05f, 0.5f));
	if (ImGui::Button(text.cancel, buttonSize)) {

		GLog.add<FD::Log::Type::None>("Request delete Coding::NewScene.");
		Scene::deleteScene<New>();
	}
	ImGui::PopStyleColor();

	if (!select.ptr)
		return;

	//create
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.7f, 0.1f, 1.0f));
	if (ImGui::Button(text.create, buttonSize)) {
		pos.create = ImGui::GetItemRectMin();

		if (this->check()) {
			FU::Cursor::setCursorType(FU::Cursor::Type::Wait);
			this->create();
		}
	}
	ImGui::PopStyleColor();
}

void FS::Coding::New::create() {
	using enum FD::Log::File::Type;
	assert(!fullPath.empty());

	std::ofstream ofs(this->fullPath, std::ios::out);

	if (!ofs)
		throw std::runtime_error("Failed to create file.");

	if (select.type == Empty)
		;
	if (select.type == Empty_Lua)
		ofs << FD::File::Template::EmptyLua << std::endl;
	else if (select.type == Empty_Python)
		ofs << FD::File::Template::EmptyPy << std::endl;
	else if (select.type == Empty_AngelScript)
		ofs << FD::File::Template::EmptyAS << std::endl;
	else {
		GLog.add<FD::Log::Type::Error>("abort() has been called. File {}.", __FILE__);
		abort();
	}


	if (!fluidumFilesRead->isMainCodeFileExist()) {
		GLog.add<FD::Log::Type::None>("Set MainCodeFile({}).", this->fullPath);
		fluidumFilesWrite->setMainCodePath(fullPath);
		fluidumFilesWrite->save();
	}
	tabWrite->addFile(fullPath);
	tabWrite->addDisplayFile(fullPath);
	tabWrite->save();
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
		if (folderPath.empty()) {
			return { ErrorType::EmptyFolderPath,{} };
		}

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
	void tryPushSlash_(std::string& path) {
#ifdef BOOST_OS_WINDOWS
		if (path.back() != '/' && path.back() != '\\')
			path.push_back('/');
#else
		if (path.back() != '/')
			path.push_back('/');
#endif
}
}

bool FS::Coding::New::check() {
	GLog.add<FD::Log::Type::None>("Check info.");

	tryPushSlash_(this->folderPath);
	auto [err, path] = checkFile(folderPath, fileName, extension);

	//no error -> return true
	if (err == ErrorType::None) {
		GLog.add<FD::Log::Type::None>("QuickInfo NoError.");
		this->fullPath = std::move(path);
		return true;
	}

	//問題があれば問題に応じて警告
	if (err == ErrorType::EmptyFolderPath) {
		GLog.add<FD::Log::Type::None>("Error EmptyFolderPath.");
		GLog.add<FD::Log::Type::None>("Request add Utils::MessageScene.");
		Scene::addScene<Utils::Message>(text.error_fill, pos.folderPath);
	}
	else if (err == ErrorType::EmptyFileName) {
		GLog.add<FD::Log::Type::None>("Error EmptyFileName.");
		GLog.add<FD::Log::Type::None>("Request add Utils::MessageScene.");
		Scene::addScene<Utils::Message>(text.error_fill, pos.fileName);
	}
	else if (err == ErrorType::NotFound) {
		GLog.add<FD::Log::Type::None>("Error NotFoundDirectory. Typed directory is {}.", folderPath);
		Scene::addScene<Utils::Message>(text.error_directoryNotFound, pos.fileName);
	}
	else if (err == ErrorType::AlreadyExist) {
		GLog.add<FD::Log::Type::None>("Error AlreadyExist. Typed filename is {}({}).", fileName, path);
		GLog.add<FD::Log::Type::None>("Request add Utils::MessageScene.");
		Scene::addScene<Utils::Message>(text.error_fileAlreadyExist, pos.create);
	}

	return false;
}

bool FS::Coding::New::button(const FDR::ImGuiImage& image, const char* label, const char* title, const char* description) {
	ImGui::BeginChild(label, style.buttonSize, true);

	const ImVec2 dummy = { 0.0f, style.buttonSize.y * 0.12f };
	ImGui::BeginChild("Left", { style.buttonSize.x * 0.17f,style.buttonSize.y }, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImGui::Dummy(dummy); ImGui::Spacing(); ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();
	ImGui::Image(image, ImVec2(style.buttonSize.y * 0.7f, style.buttonSize.y * 0.7f));
	ImGui::EndChild();

	ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();

	ImGui::BeginChild("Right");
	ImGui::SetWindowFontScale(1.35f);

	ImGui::Spacing();
	ImGui::Text(title);

	ImGui::PushFont(FDR::getDefaultFontMiniSize());
	ImGui::TextDisabled(description);
	ImGui::PopFont();

	ImGui::EndChild();

	bool click = false;
	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows)) {
		ImU32 col = ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
		ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetWindowPos(), ImGui::GetWindowPos() + ImGui::GetWindowSize(), col);
		click = ImGui::IsMouseClicked(ImGuiMouseButton_Left);
	}

	ImGui::EndChild();

	return click;
}

std::vector<FS::Coding::New::ButtonInfo> FS::Coding::New::initRecentFileTypes(const std::vector<FD::Log::File::Type>& types) {
	using enum FD::Log::File::Type;

	std::vector<ButtonInfo> result{};
	for (const auto x : types) {
		if (x == Empty)
			result.emplace_back(ButtonInfo{ images.at(0), "_Empty", text.empty, text.empty_Description });
		else if (x == Empty_Lua)
			result.emplace_back(ButtonInfo{ images.at(1), "_ELua", text.emptyLua, text.emptyLua_Description });
		else if (x == Empty_Python)
			result.emplace_back(ButtonInfo{ images.at(2), "_EPy", text.emptyPython, text.emptyPython_Description });
		else if (x == Empty_AngelScript)
			result.emplace_back(ButtonInfo{ images.at(3), "_EAS", text.emptyAngelScript, text.emptyAngelScript_Description });

		else {
			GLog.add<FD::Log::Type::Error>("abort() has been called. File {}.", __FILE__);
			abort();
		}
	}
	return result;
}

