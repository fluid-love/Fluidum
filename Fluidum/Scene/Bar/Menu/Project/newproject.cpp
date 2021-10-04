#include "newproject.h"
#include "projectform.h"
#include "../../../Utils/Popup/backwindow.h"

using namespace FU::ImGui::Operators;

namespace FS::Bar::Internal {
	std::vector<FDR::ImGuiImage> makeImages() {
		std::vector<FDR::ImGuiImage> result;
		for (uint16_t i = 0; i < 5; i++) {
			std::string path = Resource::NewProjectIconFolderPath;
			path += std::to_string(i + 1) += ".png";
			result.emplace_back(FDR::createImGuiImage(path.c_str()));
		}
		assert(result.size() == 5);
		return result;
	}

}

FS::Bar::NewProject::NewProject(
	const FD::SceneRead* const sceneRead,
	FD::WindowWrite* const windowWrite,
	const FD::GuiRead* const guiRead,
	FD::GuiWrite* const guiWrite
) :
	sceneRead(sceneRead),
	windowWrite(windowWrite),
	guiRead(guiRead),
	guiWrite(guiWrite),
	images(Internal::makeImages()),
	emptyTemplates({
			ButtonInfo{images.at(0), "_Empty", text.empty, text.emptyDescription},
			ButtonInfo{images.at(1), "_ELua", text.emptyLua, text.emptyLuaDescription},
			ButtonInfo{images.at(2), "_EPy", text.emptyPython, text.emptyPythonDescription},
			ButtonInfo{images.at(3), "_EAS", text.emptyAngelScript, text.emptyAngelScriptDescription}
		}),
	algorithmTemplates({
		ButtonInfo{ images.at(4), "_IT", text.interactive, text.interactiveDescription }
		})

{
	GLog.add<FD::Log::Type::None>("Construct NewProjectScene.");


	style.windowPos = guiRead->centerPos() - (guiRead->windowSize() / 3.0f);
	style.windowSize = guiRead->windowSize() - (style.windowPos * 2.0f);

	style.buttonSize = ImVec2(style.windowSize.x / 2.2f, style.windowSize.y * 0.09f);

	GLog.add<FD::Log::Type::None>("Request PopupBackWindowScene.");
	Scene::addScene<PopupBackWindow>();
}

FS::Bar::NewProject::~NewProject() noexcept {
	try {
		GLog.add<FD::Log::Type::None>("Request delete PopupBackWindowScene.");
		Scene::deleteScene<PopupBackWindow>();

		GLog.add<FD::Log::Type::None>("Destruct NewProjectScene.");
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

	if (!sceneRead->isExist<ProjectForm>())
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


	ImGui::Begin("NewProject", nullptr, flag);

	this->title();

	ImGui::Spacing(); ImGui::Separator();

	this->recent();
	ImGui::SameLine();
	this->select();

	ImGui::Spacing(); ImGui::Spacing();
	this->bottom();

	ImGui::End();


	ImGui::PopStyleVar(3);
}

void FS::Bar::NewProject::title() {
	ImGui::BeginChild("NPtitle", ImVec2(style.windowSize.x / 2.0f, style.windowSize.y * 0.07f));
	ImGui::SetWindowFontScale(1.7f);

	ImGui::Text(text.newProject);

	ImGui::EndChild(); ImGui::SameLine();
	ImGui::BeginChild("Search", ImVec2(style.windowSize.x / 3.0f, style.windowSize.y * 0.07f));
	ImGui::Spacing();

	//検索
	std::string data;
	data.resize(100);
	ImGui::InputText(text.search, data.data(), 100);

	ImGui::EndChild();
}

void FS::Bar::NewProject::recent() {

	ImGui::BeginChild("Recent", ImVec2(style.windowSize.x / 2.5f, style.windowSize.y / 2.0f));

	ImGui::Text(text.recent);
	ImGui::SetWindowFontScale(1.35f);
	ImGui::Spacing();

	ImGui::BeginChild("RecentInner");

	ImGui::PushFont(FDR::getDefaultFontMiniSize());
	ImGui::TextDisabled(text.NotApplicable);
	ImGui::PopFont();

	ImGui::EndChild();

	ImGui::EndChild();
}

void FS::Bar::NewProject::select() {
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


	if (ImGui::BeginCombo("#filter", filter1[layoutIndex], ImGuiComboFlags_HeightLargest)) {
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
		bool click = this->button(x.image, x.label, x.title, x.description);
		this->formScene(click, x.title);
	}

	ImGui::BulletText(text.algorithm);
	for (auto& x : algorithmTemplates) {
		bool click = this->button(x.image, x.label, x.title, x.description);
		this->formScene(click, x.title);
	}

}

void FS::Bar::NewProject::formScene(const bool isButtonClicked, const char* type) {
	if (!isButtonClicked)
		return;

	GLog.add<FD::Log::Type::None>("Request add ProjectFromScene.");
	Scene::addScene<ProjectForm>(type);
}

void FS::Bar::NewProject::bottom() {

	const ImVec2 buttonSize = ImVec2(ImGui::GetWindowSize().x * 0.2f, 0.0f);

	//キャンセルボタン　赤字
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.05f, 0.05f, 0.5f));
	if (ImGui::Button(text.cancel, buttonSize)) {
		ImGui::CloseCurrentPopup();//popupを終了	

		//PopupBackWindowはデストラクタで削除
		GLog.add<FD::Log::Type::None>("Request delete NewProjectScene.");
		Scene::deleteScene<NewProject>();

	}
	ImGui::PopStyleColor();


}

#include <imgui_internal.h>
bool FS::Bar::NewProject::button(const FDR::ImGuiImage& image, const char* label, const char* title, const char* description) {
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



