#include "new.h"
#include "check_path.h"
#include <imgui_internal.h>
#include "../../Coding/TextEditor/texteditor.h"
#include "select.h"
#include "../../Utils/Popup/message.h"
#include "../../Utils/Popup/backwindow.h"
#include <nfd.h>

using namespace FU::ImGui::Operators;

namespace FS::Project::Add::Internal {
	std::vector<FDR::ImGuiImage> makeImages() {
		constexpr const char* names[] = {
			"empty.png",
			"lua.png",
			"python.png",
			"cpp.png"
		};
		std::vector<FDR::ImGuiImage> result;
		for (uint16_t i = 0; i < std::extent_v<decltype(names), 0>; i++) {
			std::string path = Resource::ProjectNewIconsFolderPath;
			path += names[i];
			result.emplace_back(FDR::createImGuiImage(path.c_str()));
		}

		return result;
	}
}

FS::Project::Add::NewFile::NewFile(
	const FD::ProjectRead* const projectRead,
	const FD::GuiRead* const guiRead,
	const FD::Log::FileRead* const fileRead,
	std::shared_ptr<Add::SharedInfo>& sharedInfo
) :
	projectRead(projectRead),
	sharedInfo(sharedInfo),
	recentButtons(initRecentFileTypes(fileRead->recent())),
	images(Internal::makeImages()),
	emptyFiles({
			ButtonInfo{images.at(0), "_Empty", text.empty.operator const std::string & (), text.empty_description, "",FD::Log::File::Type::Empty },
			ButtonInfo{images.at(1), "_ELua", text.emptyLua.operator const std::string & (), text.emptyLua_description, ".lua",FD::Log::File::Type::Empty_Lua},
			ButtonInfo{images.at(2), "_EPy", text.emptyPython.operator const std::string & (), text.emptyPython_description, ".py",FD::Log::File::Type::Empty_Python},
			ButtonInfo{images.at(3), "_ECpp", text.emptyCpp.operator const std::string & (), text.emptyCpp_description, ".cpp",FD::Log::File::Type::Empty_Cpp}
		})
{
	FluidumScene_Log_Constructor("Coding::New");

	style.windowPos = guiRead->centerPos() - (guiRead->windowSize() / 3.0f);
	style.windowSize = guiRead->windowSize() - (style.windowPos * 2.0f);

	style.buttonSize = ImVec2(style.windowSize.x / 2.1f, style.windowSize.y * 0.09f);

	assert(sharedInfo);
	str.directoryPath = sharedInfo->path;

	//capacity
	str.directoryPath.reserve(200);
	str.fileName.reserve(200);


	FluidumScene_Log_RequestTryAddScene("PopupBackWindow");
	this->only = Scene::tryAddScene<PopupBackWindow>();
}

FS::Project::Add::NewFile::~NewFile() {
	try {
		if (this->only) {
			FluidumScene_Log_RequestDeleteScene("PopupBackWindow");
			Scene::deleteScene<PopupBackWindow>();
		}

		FluidumScene_Log_Destructor("Project::Add::NewFile");
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

void FS::Project::Add::NewFile::call() {

	ImGui::SetNextWindowFocus();
	ImGui::SetNextWindowPos(style.windowPos);
	ImGui::SetNextWindowSize(style.windowSize);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.5f);
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);

	constexpr auto flag =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove;


	//animation
	if (!only)
		ImAnime::PushStyleVar(anime.counter, 0.5f, 0.0f, 1.0f, ImAnimeType::LINEAR, ImGuiStyleVar_Alpha);

	ImGui::Begin("Coding::New", nullptr, flag);

	if (only)
		ImAnime::PushStyleVar(anime.counter, 0.5f, 0.0f, 1.0f, ImAnimeType::LINEAR, ImGuiStyleVar_Alpha);


	this->title();

	ImGui::Spacing(); ImGui::Spacing();

	this->recent();
	ImGui::SameLine();
	this->list();

	ImGui::Spacing();
	this->form();
	ImGui::Spacing();

	this->bottom();

	ImAnime::PopStyleVar();

	ImGui::End();

	ImGui::PopStyleVar(3);
}

void FS::Project::Add::NewFile::title() {
	ImGui::BeginChild("NPtitle", ImVec2(style.windowSize.x / 2.0f, style.windowSize.y * 0.07f));
	ImGui::SetWindowFontScale(1.7f);

	ImGui::Text(text.title);

	ImGui::EndChild(); ImGui::SameLine();
	ImGui::BeginChild("Search", ImVec2(style.windowSize.x / 3.0f, style.windowSize.y * 0.07f));
	ImGui::Spacing();

	//search
	if (ImGui::InputText(text.search, str.searchStr.data(), str.searchStr.capacity()))
		this->search();

	ImGui::EndChild();
}

void FS::Project::Add::NewFile::search() {

}

void FS::Project::Add::NewFile::recent() {
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());

	ImGui::BeginChild("Recent", ImVec2(style.windowSize.x / 2.35f, style.windowSize.y * 0.65f), false, ImGuiWindowFlags_NoScrollbar);

	ImGui::BeginChild("RecentTitle", ImVec2(0.0f, style.windowSize.y * 0.08f));
	ImGui::SetWindowFontScale(1.2f);
	ImGui::Spacing(); ImGui::Spacing();
	ImGui::Text(text.recent);

	ImGui::EndChild();

	ImGui::BeginChild("Recent_", ImVec2(style.windowSize.x / 2.35f, 0.0f), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);

	if (recentButtons.empty())
		ImGui::TextDisabled(text.recentEmpty);
	else {
		const float width = style.windowSize.x / 2.5f;
		for (auto& x : recentButtons) {
			const auto [left, right] = this->button(
				&x,
				width
			);

			if (left) {
				select.ptr = &x;
				str.extension = x.extension;
				str.extension.reserve(100);
			}
		}
	}

	ImGui::EndChild();
	ImGui::EndChild();

	ImGui::PopStyleVar(2);
}

void FS::Project::Add::NewFile::list() {
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());

	//slect空間全体
	ImGui::BeginChild("Select", ImVec2(style.windowSize.x / 2.0f, style.windowSize.y * 0.65f), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	//dummy空間
	ImGui::BeginChild("SelectU", ImVec2(0.0f, style.windowSize.y * 0.08f), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
	this->filter();

	ImGui::EndChild();

	ImGui::BeginChild("SelectB", ImVec2(style.windowSize.x / 2.0f, 0.0f), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);


	//buttons
	this->buttons();

	ImGui::EndChild();//bottom

	ImGui::EndChild();//全体

	ImGui::PopStyleVar(2);
}

void FS::Project::Add::NewFile::filter() {
	int32_t layoutIndex = 0;

	constexpr const char* icons[] = { ICON_MD_INBOX" ",ICON_FA_DNA"  " };
	std::string empty = icons[0];
	empty += text.empty;
	std::string algo = icons[1];
	//algo += text.algorithm;

	const char* filter1[3] = { text.all, empty.data() , algo.data() };

	ImGui::AlignTextToFramePadding();
	ImGui::Text(ICON_FA_FILTER); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetWindowSize().x * 0.3f);


	if (ImGui::BeginCombo("##filter", filter1[layoutIndex], ImGuiComboFlags_HeightLargest)) {
		for (int n = 0; n < IM_ARRAYSIZE(filter1); n++) {
			const bool is_selected = (layoutIndex == n);
			if (ImGui::Selectable(filter1[n], is_selected))
				layoutIndex = n;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}

void FS::Project::Add::NewFile::buttons() {

	ImGui::BulletText(text.empty);
	for (auto& x : emptyFiles) {
		if (x.hide)
			continue;

		const auto [left, right] = this->button(&x);

		if (left) {
			select.ptr = &x;
			str.extension = x.extension;
			str.extension.reserve(100);
		}
	}

}

void FS::Project::Add::NewFile::form() {
	if (!select.ptr) {
		ImGui::Dummy({ 0.0f,ImGui::GetWindowHeight() * 0.16f });
		return;
	}

	//animation
	ImAnime::PushStyleVar(anime.form, 0.3f, 0.0f, 1.0f, ImAnimeType::LINEAR, ImGuiStyleVar_Alpha);

	ImGui::BeginChild("Form", { 0.0f,ImGui::GetWindowHeight() * 0.16f });

	ImGui::Spacing();

	this->directoryPath();

	ImGui::Spacing();

	this->fileName();

	ImGui::EndChild();

	ImAnime::PopStyleVar();
}

void FS::Project::Add::NewFile::directoryPath() {
	ImGui::BulletText(text.folderPath); ImGui::Spacing(); ImGui::SameLine();

	if (sharedInfo->project)
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);

	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.95f);
	ImGui::InputText("##FolderPath", str.directoryPath.data(), str.directoryPath.capacity(), sharedInfo->project ? ImGuiInputTextFlags_ReadOnly : 0);
	pos.directoryPath = ImGui::GetItemRectMin();

	if (sharedInfo->project)
		ImGui::PopStyleVar();
}

void FS::Project::Add::NewFile::fileName() {
	ImGui::BulletText(text.fileName); ImGui::Spacing(); ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.6f);
	ImGui::InputText("##FileName", str.fileName.data(), str.fileName.capacity());
	pos.fileName = ImGui::GetItemRectMin();

	if (!select.ptr || select.ptr->type == FD::Log::File::Type::Empty)
		return;

	ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.17f);
	ImGui::InputText("##Extension", str.extension.data(), str.extension.capacity());
}

void FS::Project::Add::NewFile::bottom() {

	const ImVec2 buttonSize = ImVec2(ImGui::GetWindowSize().x * 0.2f, 0.0f);

	//back
	if (!this->only) {
		if (ImGui::Button(text.back, buttonSize)) {
			this->deleteThisScene();
		}

		ImGui::SameLine();
	}

	//cancel
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.05f, 0.05f, 0.5f));
	if (ImGui::Button(text.cancel, buttonSize)) {
		this->deleteThisScene();
		if (!this->only) {
			FluidumScene_Log_RequestDeleteScene("Project::Add::Select");
			Scene::deleteScene<Select>();
		}
	}
	ImGui::PopStyleColor();

	if (!select.ptr)
		return;

	ImGui::SameLine();

	//create
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.02f, 0.35f, 0.02f, 1.0f));
	if (ImGui::Button(text.create, buttonSize)) {
		pos.create = ImGui::GetItemRectMin();
		this->tryCreate();
	}
	ImGui::PopStyleColor();
}

void FS::Project::Add::NewFile::tryCreate() {

	if (!this->check())
		return;//failed

	if (!this->create())
		return;//failed

	//shared
	if (sharedInfo) {
		sharedInfo->path = std::move(this->str.fullPath);
		sharedInfo->create = true;
		sharedInfo->type = SharedInfo::Type::File;
	}

	this->deleteThisScene();
	if (!this->only) {
		FluidumScene_Log_RequestDeleteScene("Project::Add::Select");
		Scene::deleteScene<Select>();
	}
}

bool FS::Project::Add::NewFile::create() {
	using enum FD::Log::File::Type;
	assert(!str.fullPath.empty());

	std::ofstream ofs(this->str.fullPath, std::ios::out);

	if (!ofs || !std::filesystem::exists(this->str.fullPath)) {
		GLog.add<FD::Log::Type::None>("Error Failed to create file.");
		Scene::addScene<Utils::Message>(text.error_unexpected, pos.create);
		return false;
	}


	if (select.ptr->type == Empty)
		;
	else if (select.ptr->type == Empty_Lua)
		ofs << FD::File::Template::EmptyLua << std::endl;
	else if (select.ptr->type == Empty_Python)
		ofs << FD::File::Template::EmptyPy << std::endl;
	else if (select.ptr->type == Empty_Cpp)
		ofs << FD::File::Template::EmptyCpp << std::endl;
	else {
		FluidumScene_Log_SeriousError_ThrowException();
	}


	return true;
}

bool FS::Project::Add::NewFile::check() {
	std::string directoryPath = this->str.directoryPath.data();
	std::string fileName = this->str.fileName.data();
	std::string extension = this->str.extension.data();

	const std::string name = fileName + extension;

	CheckPath::Info info{
		.project = sharedInfo->project,
		.directory = false,
		.parent = directoryPath,
		.name = name,
		.pos_name = pos.fileName,
		.pos_path = pos.directoryPath,
		.pos_create = pos.create,
	};

	FluidumScene_Log_CallSceneConstructor("Project::Add::CheckPath");
	Scene::callConstructor<CheckPath>(info);

	if (info.noerror) {
		str.fullPath = std::move(info.fullPath);
		return true;
	}

	//error
	return false;
}

std::pair<bool, bool> FS::Project::Add::NewFile::button(
	const ButtonInfo* const info,
	const std::optional<float> width
) {
	const ImVec2 size = width.has_value() ? ImVec2{ width.value(), style.buttonSize.y } : style.buttonSize;


	ImGui::BeginChild(info->label, size, true);

	//cliked
	if (info == select.ptr) {
		const ImVec2 windowPos = ImGui::GetWindowPos();
		constexpr ImU32 col = FU::ImGui::convertImVec4ToImU32(0.1f, 0.4f, 0.9f, 0.2f);
		ImGui::GetWindowDrawList()->AddRectFilled(windowPos, windowPos + ImGui::GetWindowSize(), col);
	}

	const ImVec2 dummy = { 0.0f, size.y * 0.12f };
	ImGui::BeginChild("Left", { size.x * 0.17f,size.y }, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImGui::Dummy(dummy); ImGui::Spacing(); ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();
	ImGui::Image(info->image, ImVec2(size.y * 0.7f, size.y * 0.7f));
	ImGui::EndChild();

	ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();

	ImGui::BeginChild("Right");
	ImGui::SetWindowFontScale(1.35f);

	ImGui::Spacing();
	ImGui::Text(info->title.c_str());

	ImGui::PushFont(FDR::getDefaultFontMiniSize());
	ImGui::PushStyleColor(ImGuiCol_TextDisabled, { 0.6f,0.6f,0.6f,1.0f });
	ImGui::TextDisabled(info->description);
	ImGui::PopStyleColor();
	ImGui::PopFont();

	ImGui::EndChild();

	std::pair<bool, bool> click = { false ,false };
	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows)) {
		ImU32 col = ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
		ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetWindowPos(), ImGui::GetWindowPos() + ImGui::GetWindowSize(), col);
		click.first = ImGui::IsMouseClicked(ImGuiMouseButton_Left);
		click.second = ImGui::IsMouseClicked(ImGuiMouseButton_Right);
	}

	ImGui::EndChild();

	return click;
}

std::vector<FS::Project::Add::NewFile::ButtonInfo> FS::Project::Add::NewFile::initRecentFileTypes(const std::vector<FD::Log::File::Type>& types) {
	using enum FD::Log::File::Type;

	std::vector<ButtonInfo> result{};
	for (const auto x : types) {
		if (x == Empty)
			result.emplace_back(ButtonInfo{ images.at(0), "_Empty", text.empty.operator const std::string & (), text.empty_description, "",FD::Log::File::Type::Empty });
		else if (x == Empty_Lua)
			result.emplace_back(ButtonInfo{ images.at(1), "_ELua", text.emptyLua.operator const std::string & (), text.emptyLua_description,".lua",FD::Log::File::Type::Empty_Lua });
		else if (x == Empty_Python)
			result.emplace_back(ButtonInfo{ images.at(2), "_EPy", text.emptyPython.operator const std::string & (), text.emptyPython_description ,".py",FD::Log::File::Type::Empty_Python });
		else if (x == Empty_Cpp)
			result.emplace_back(ButtonInfo{ images.at(3), "_ECpp", text.emptyCpp.operator const std::string & (), text.emptyCpp_description ,".cpp",FD::Log::File::Type::Empty_Cpp });

		else {
			FluidumScene_Log_Abort();
			abort();
		}
	}
	return result;
}

void FS::Project::Add::NewFile::deleteThisScene() {
	FluidumScene_Log_RequestDeleteScene("Project::Add::NewFile");
	Scene::deleteScene<NewFile>();
}