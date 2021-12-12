#include "newproject.h"
#include "../../../Utils/Popup/backwindow.h"
#include "../../../Utils/Popup/message.h"

using namespace FU::ImGui::Operators;

namespace FS::Bar::Internal {
	std::vector<FDR::ImGuiImage> makeImages() {
		std::vector<FDR::ImGuiImage> result;
		for (uint16_t i = 0; i < 4; i++) {
			std::string path = Resource::NewProjectIconsFolderPath;
			path += std::to_string(i + 1) += ".png";
			result.emplace_back(FDR::createImGuiImage(path.c_str()));
		}
		return result;
	}
}

FS::Bar::NewProject::NewProject(
	const FD::SceneRead* const sceneRead,
	FD::WindowWrite* const windowWrite,
	const FD::GuiRead* const guiRead,
	FD::GuiWrite* const guiWrite,
	const FD::Log::ProjectRead* const projectLogRead,
	FD::Log::ProjectWrite* const projectLogWrite
) :
	sceneRead(sceneRead),
	windowWrite(windowWrite),
	guiRead(guiRead),
	guiWrite(guiWrite),
	projectLogWrite(projectLogWrite),
	images(Internal::makeImages()),
	emptyTemplates({
			ButtonInfo{FD::Log::Project::Type::Empty, images.at(0), "_Empty", text.empty.operator const std::string & (), text.emptyDescription},
			ButtonInfo{FD::Log::Project::Type::Empty_Lua, images.at(1), "_ELua", text.emptyLua.operator const std::string & (), text.emptyLuaDescription},
			ButtonInfo{FD::Log::Project::Type::Empty_Python, images.at(2), "_EPy", text.emptyPython.operator const std::string & (), text.emptyPythonDescription},
			ButtonInfo{FD::Log::Project::Type::Empty_Cpp, images.at(3), "_ECpp", text.emptyCpp.operator const std::string & (), text.emptyCppDescription}
		}),
	algorithmTemplates({
		}),
		recentTemplates(this->initRecentTempates(projectLogRead->recent()))

{
	FluidumScene_Log_Constructor("NewProject");

	style.windowPos = guiRead->centerPos() - (guiRead->windowSize() / 3.0f);
	style.windowSize = guiRead->windowSize() - (style.windowPos * 2.0f);

	style.buttonSize = ImVec2(style.windowSize.x / 2.1f, style.windowSize.y * 0.09f);

	FluidumScene_Log_RequestAddScene("PopupBackWindow");
	Scene::addScene<PopupBackWindow>();

	this->searchStr.str.reserve(100);
}

FS::Bar::NewProject::~NewProject() noexcept {
	try {
		FluidumScene_Log_RequestDeleteScene("PopupBackWindow");
		Scene::deleteScene<PopupBackWindow>();

		FluidumScene_Log_Destructor("NewProject");
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

void FS::Bar::NewProject::call() {

	if (!sceneRead->isExist<ProjectForm>() && !ImGui::IsPopupOpen("RecentPopup"))
		ImGui::SetNextWindowFocus();
	ImGui::SetNextWindowPos(style.windowPos);
	ImGui::SetNextWindowSize(style.windowSize);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.1f);
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);

	constexpr auto flag =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove;

	ImGui::Begin("NewProject", nullptr, flag);

	//animation
	ImAnime::PushStyleVar(anime.counter, 0.5f, 0.0f, 1.0f, ImAnimeType::LINEAR, ImGuiStyleVar_Alpha);

	ImGui::BeginChild("##NPMain", { 0.0f,style.windowSize.y * 0.92f });

	this->title();

	ImGui::Spacing(); ImGui::Separator();

	ImGui::Spacing(); ImGui::SameLine();
	this->recent();
	ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();
	ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();
	this->select();

	ImGui::EndChild();//NPMain

	ImGui::Spacing(); ImGui::SameLine();
	this->bottom();

	ImAnime::PopStyleVar();

	ImGui::End();

	ImGui::PopStyleVar(3);

	this->recentPopup();
	this->checkForm();
}

void FS::Bar::NewProject::checkForm() {
	if (!formInfo)
		return;

	if (formInfo.use_count() != 1)
		return;

	if (formInfo->create) {
		projectLogWrite->update(formInfo->type);
		FluidumScene_Log_RequestDeleteScene("NewProject");
		Scene::deleteScene<Bar::NewProject>();
	}

	formInfo.reset();
}

void FS::Bar::NewProject::title() {
	ImGui::BeginChild("NPtitle", ImVec2(style.windowSize.x / 2.0f, style.windowSize.y * 0.07f));
	ImGui::SetWindowFontScale(1.7f);

	ImGui::Text(text.newProject);

	ImGui::EndChild(); ImGui::SameLine();
	ImGui::BeginChild("Search", ImVec2(style.windowSize.x / 3.0f, style.windowSize.y * 0.07f));
	ImGui::Spacing();

	//search
	if (ImGui::InputText(text.search, searchStr.str.data(), searchStr.str.capacity()))
		this->search();

	ImGui::EndChild();
}

void FS::Bar::NewProject::recent() {
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());

	ImGui::BeginChild("Recent", ImVec2(style.windowSize.x / 2.35f, style.windowSize.y * 0.8f), false, ImGuiWindowFlags_NoScrollbar);

	ImGui::BeginChild("RecentTitle", ImVec2(0.0f, style.windowSize.y * 0.08f));
	ImGui::SetWindowFontScale(1.2f);
	ImGui::Spacing(); ImGui::Spacing();
	ImGui::Text(text.recent);

	ImGui::EndChild();

	ImGui::BeginChild("Recent_", ImVec2(style.windowSize.x / 2.35f, 0.0f), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);

	if (recentTemplates.empty())
		ImGui::TextDisabled(text.NotApplicable);
	else {
		const float width = style.windowSize.x / 2.5f;
		for (auto& x : recentTemplates) {
			const auto [left, right] = this->button(
				x.image,
				x.label,
				x.title.c_str(),
				x.description,
				width
			);
			this->formScene(left, x);

			if (right)
				recentPopupInfo.flag = true;
		}
	}

	ImGui::EndChild();
	ImGui::EndChild();

	ImGui::PopStyleVar(2);
}

void FS::Bar::NewProject::select() {
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());

	//slect空間全体
	ImGui::BeginChild("Select", ImVec2(style.windowSize.x / 2.0f, style.windowSize.y * 0.8f), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	//dummy
	ImGui::BeginChild("SelectU", ImVec2(0.0f, style.windowSize.y * 0.07f), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::Spacing(); ImGui::Spacing();
	this->filter();

	ImGui::EndChild();

	ImGui::BeginChild("SelectB", ImVec2(style.windowSize.x / 2.0f, 0.0f), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);


	//buttons
	this->buttons();

	ImGui::EndChild();//bottom

	ImGui::EndChild();//全体

	ImGui::PopStyleVar(2);
}

void FS::Bar::NewProject::filter() {
	int32_t layoutIndex = 0;

	constexpr const char* icons[] = { ICON_MD_INBOX" ",ICON_FA_DNA"  " };
	std::string empty = icons[0];
	empty += text.empty;
	std::string algo = icons[1];
	algo += text.algorithm;

	const char* filter1[3] = { text.all, empty.data() , algo.data() };

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

void FS::Bar::NewProject::buttons() {

	ImGui::BulletText(text.empty);
	for (auto& x : emptyTemplates) {
		if (x.hide)
			continue;

		bool click = this->button(x.image, x.label, x.title.c_str(), x.description).first;
		this->formScene(click, x);
	}

	ImGui::BulletText(text.algorithm);
	for (auto& x : algorithmTemplates) {
		if (x.hide)
			continue;

		bool click = this->button(x.image, x.label, x.title.c_str(), x.description).first;
		this->formScene(click, x);
	}

}

void FS::Bar::NewProject::formScene(const bool isButtonClicked, const ButtonInfo& button) {
	if (!isButtonClicked)
		return;

	ProjectForm::Info info{
		.type = button.type,
		.name = button.title
	};

	this->formInfo = std::make_shared<ProjectForm::Info>(std::move(info));

	FluidumScene_Log_RequestAddScene("ProjectFrom");
	Scene::addScene<ProjectForm>(formInfo);
}

void FS::Bar::NewProject::bottom() {

	const ImVec2 buttonSize = ImVec2(ImGui::GetWindowSize().x * 0.2f, 0.0f);

	//cancel
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.05f, 0.05f, 0.5f));
	if (ImGui::Button(text.cancel, buttonSize)) {
		ImGui::CloseCurrentPopup();//popupを終了	

		//PopupBackWindow -> destructor
		FluidumScene_Log_RequestDeleteScene("NewProject");
		Scene::deleteScene<NewProject>();

	}
	ImGui::PopStyleColor();

}

#include <imgui_internal.h>

std::pair<bool, bool> FS::Bar::NewProject::button(
	const FDR::ImGuiImage& image,
	const char* label,
	const char* title,
	const char* description,
	const std::optional<float> width
) {
	const ImVec2 size = width.has_value() ? ImVec2{ width.value(), style.buttonSize.y } : style.buttonSize;

	ImGui::BeginChild(label, size, true);

	const ImVec2 dummy = { 0.0f, size.y * 0.12f };
	ImGui::BeginChild("Left", { size.x * 0.17f,size.y }, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImGui::Dummy(dummy); ImGui::Spacing(); ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();
	ImGui::Image(image, ImVec2(size.y * 0.7f, size.y * 0.7f));
	ImGui::EndChild();

	ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();

	ImGui::BeginChild("Right");
	ImGui::SetWindowFontScale(1.35f);

	ImGui::Spacing();
	ImGui::Text(title);

	ImGui::PushFont(FDR::getDefaultFontMiniSize());
	ImGui::PushStyleColor(ImGuiCol_TextDisabled, { 0.6f,0.6f,0.6f,1.0f });
	ImGui::TextDisabled(description);
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

void FS::Bar::NewProject::search() {
	const std::string data = searchStr.str.data();

	if (data.size() < searchStr.temp.size()) {
		std::for_each(emptyTemplates.begin(), emptyTemplates.end(), [&](auto& x) {x.hide = false; });
		std::for_each(algorithmTemplates.begin(), algorithmTemplates.end(), [&](auto& x) {x.hide = false; });

	}

	searchStr.temp = data;

	auto func = [&](ButtonInfo& x)->void {
		if (x.hide)
			return;

		const auto itr = std::search(x.title.begin(), x.title.end(), data.begin(), data.end());

		if (itr == x.title.end()) {
			x.hide = true;
		}
	};

	std::for_each(emptyTemplates.begin(), emptyTemplates.end(), func);
	std::for_each(algorithmTemplates.begin(), algorithmTemplates.end(), func);
}

std::vector<FS::Bar::NewProject::ButtonInfo> FS::Bar::NewProject::initRecentTempates(const std::vector<FD::Log::Project::Type>& types) {
	using enum FD::Log::Project::Type;

	std::vector<ButtonInfo> result{};
	for (const auto x : types) {
		if (x == Empty)
			result.emplace_back(ButtonInfo{ x,images.at(0), "_Empty", text.empty.operator const std::string & (), text.emptyDescription });
		else if (x == Empty_Lua)
			result.emplace_back(ButtonInfo{ x,images.at(1), "_ELua", text.emptyLua.operator const std::string & (), text.emptyLuaDescription });
		else if (x == Empty_Python)
			result.emplace_back(ButtonInfo{ x,images.at(2), "_EPy", text.emptyPython.operator const std::string & (), text.emptyPythonDescription });
		else if (x == Empty_Cpp)
			result.emplace_back(ButtonInfo{ x,images.at(3), "_ECpp", text.emptyCpp.operator const std::string & (), text.emptyCppDescription });

		else {
			FluidumScene_Log_Abort();
			abort();
		}
	}
	return result;
}

void FS::Bar::NewProject::recentPopup() {
	if (recentPopupInfo.flag) {
		ImGui::OpenPopup("RecentPopup");
		recentPopupInfo.flag = false;
	}
	if (!ImGui::BeginPopup("RecentPopup"))
		return;

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { ImGui::GetStyle().ItemSpacing.x,ImGui::GetStyle().ItemSpacing.y * 2.2f });

	if (ImGui::Selectable(text.recent_erase))
		this->recent_erase();

	if (ImGui::Selectable(text.recent_clear))
		this->recent_clear();

	ImGui::PopStyleVar();

	ImGui::EndPopup();
}

void FS::Bar::NewProject::recent_clear() {
	GLog.add<FD::Log::Type::None>("Clear RecentProject log.");
	projectLogWrite->clear();

	FluidumScene_Log_RequestAddScene("Utils::Message");
	Scene::addScene<Utils::Message>(text.recent_message, ImGui::GetItemRectMin());
}

void FS::Bar::NewProject::recent_erase() {
	GLog.add<FD::Log::Type::None>("Erase RecentProject log. Log index:{}.", recentPopupInfo.index);
	projectLogWrite->erase(recentPopupInfo.index);

	FluidumScene_Log_RequestAddScene("Utils::Message");
	Scene::addScene<Utils::Message>(text.recent_message, ImGui::GetItemRectMin());
}



