#include "menubar.h"

//new project
#include "Project/newproject.h"

//popup back window
#include "../../Utils/Popup/backwindow.h"

using namespace FU::ImGui::Operators;

FS::MenuBar::MenuBar(
	FD::ProjectWrite* const projectWrite,
	FD::WindowWrite* const windowWrite,
	const FD::GuiRead* const guiRead,
	FD::GuiWrite* const guiWrite
) : projectWrite(projectWrite), windowWrite(windowWrite), guiRead(guiRead), guiWrite(guiWrite)
{
	GLog.add<FD::Log::Type::None>("Construct MenuBarScene.");

	const auto size = ImGui::GetFontSize() * 0.45f;
	style.offset = { size ,size };

	guiWrite->menuBarHeight(ImGui::GetFontSize() + size * 2.0f);

}

FS::MenuBar::~MenuBar() {
	try {
		GLog.add<FD::Log::Type::None>("Destruct MenuBarScene.");
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

void FS::MenuBar::call() {
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, style.offset);//厚みをもたせる

	//MenuBarのサイズはImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2

	ImGui::SetNextWindowPos(ImVec2(), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(guiRead->windowSize().x, ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2), ImGuiCond_Once);

	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.017f, 0.017f, 0.017f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.087f, 0.087f, 0.087f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.087f, 0.087f, 0.087f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.07f, 0.07f, 0.14f, 0.9f));

	constexpr ImGuiWindowFlags flag =
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_MenuBar |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove;

	ImGui::Begin("MenuBar", nullptr, flag);
	if (ImGui::BeginMenuBar()) {
		//dummy for icon
		if (ImGui::BeginMenu("   "))
			ImGui::EndMenu();


		this->fileGui();
		this->editGui();
		this->calc();
		this->viewGui();
		this->helpGui();

		ImGui::EndMenuBar();
	}
	ImGui::End();

	ImGui::PopStyleColor(4);
	ImGui::PopStyleVar();

}

void FS::MenuBar::fileGui() {

	if (ImGui::BeginMenu(text.file)) {

		this->itemCreateNewProject();
		ImGui::Spacing();
		this->itemOpen();
		ImGui::Spacing();
		this->itemSave();
		ImGui::Spacing();
		this->itemSaveAs();

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		this->itemTerminate();

		ImGui::EndMenu();
	}
}

void FS::MenuBar::itemCreateNewProject() {
	if (!ImGui::MenuItem(text.create))
		return;

	GLog.add<FD::Log::Type::None>("Request add NewProjectScene.");
	Scene::addScene<Bar::NewProject>();
}

void FS::MenuBar::itemOpen() {
	if (!ImGui::MenuItem(text.open))
		return;
	
}

void FS::MenuBar::itemSave() {
	if (!ImGui::MenuItem(text.save))
		return;

	GLog.add<FD::Log::Type::None>("Request save project.");
	try {
		projectWrite->save();
	}
	catch (const FD::Project::ExceptionType type) {
		using enum FD::Project::ExceptionType;
		GLog.add<FD::Log::Type::None>("Failed to save project.");

		//前回の読み込み(セーブ)からプロジェクトファイルが移動削除された場合
		if (type == NotFoundProjectFolder) {
			//表示して新しく作成させる//実装予定
		}
		else {
			GLog.add<FD::Log::Type::Error>("abort() has been called. File {}.", __FILE__);
			abort();
		}
		abort();
	}
	//書き込みに失敗
	catch (const std::exception&) {
		//表示して新しく保存させる
		abort();
	}

}

void FS::MenuBar::itemSaveAs() {
	if (!ImGui::MenuItem(text.saveFileAs))
		return;
}

void FS::MenuBar::itemTerminate() {
	//閉じるは赤くする
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.7f, 0.05f, 0.05f, 0.5f));
	if (ImGui::MenuItem(text.terminate)) {
		GLog.add<FD::Log::Type::None>("Request Terminate.");
		*windowWrite->getCloseFlag() = true;
	}
	ImGui::PopStyleColor();
}

void FS::MenuBar::editGui() {
	if (ImGui::BeginMenu(text.edit)) {

		ImGui::EndMenu();
	}
}

void FS::MenuBar::calc() {
	if (ImGui::BeginMenu(text.calc)) {

		ImGui::Spacing();
		if (ImGui::MenuItem(text.run)) {

		}
		ImGui::Spacing();
		if (ImGui::MenuItem(text.runIgnoreStep)) {

		}
		ImGui::Spacing();
		if (ImGui::MenuItem(text.calcInfo)) {

		}

		ImGui::EndMenu();
	}

}

void FS::MenuBar::viewGui() {
	if (ImGui::BeginMenu(text.view)) {

		ImGui::EndMenu();
	}
}

void FS::MenuBar::helpGui() {
	if (ImGui::BeginMenu(text.help)) {

		if (ImGui::MenuItem(text.credit)) {
			//Scene::addScene<FS::BarPart::Credit>();
		}


		ImGui::EndMenu();
	}
}






















