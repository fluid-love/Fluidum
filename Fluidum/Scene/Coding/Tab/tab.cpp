#include "tab.h"
#include <nfd.h>
#include "../../Utils/Popup/message.h"

namespace FS::Coding {

	constexpr const char* PopupLabel = "CodingTabPopup";
	constexpr const char* PopupLabel_Window = "CodingTabPopupWindow";

}

FS::Coding::Tab::Tab(
	FD::Coding::TabWrite* const tabWrite,
	const FD::Coding::TabRead* const tabRead,
	const FD::ProjectRead* const projectRead
) :
	tabWrite(tabWrite),
	tabRead(tabRead),
	projectRead(projectRead)
{
	FluidumScene_Log_Constructor("Coding::Tab");

	style.topBarSize = { 0.0f,ImGui::CalcTextSize(ICON_MD_FOLDER_OPEN).y * 1.7f };

	this->updateInfo();
}

FS::Coding::Tab::~Tab() noexcept {
	FluidumScene_Log_Destructor_("Coding::Tab");
}

void FS::Coding::Tab::call() {
	using namespace FU::ImGui::Operators;

	this->update();
	this->updateTextSaved();

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 0.0f));

	ImGui::Begin(text.tab, &windowCloseFlag);
	pos.center = ImGui::GetWindowPos() / 2.3f;

	this->fileList();

	this->popup();
	this->popup_window();
	ImGui::End();

	ImGui::PopStyleVar();


	this->checkWindowShouldClose();
}

void FS::Coding::Tab::checkWindowShouldClose() {
	if (this->windowCloseFlag)
		return;

	FluidumScene_Log_RequestDeleteScene("Coding::Tab");
	Scene::deleteScene<Tab>();
}

//void FS::Coding::Tab::include() {
//	std::unique_ptr<nfdchar_t*> outPath = std::make_unique<nfdchar_t*>();
//	GLog.add<FD::Log::Type::None>("Open file dialog.");
//	const nfdresult_t result = NFD_OpenDialog(".lua,.py,.as", NULL, outPath.get());
//	if (result == NFD_OKAY) {
//		GLog.add<FD::Log::Type::None>("Selected file path is {}.", *outPath.get());
//	}
//	else if (result == NFD_CANCEL) {
//		GLog.add<FD::Log::Type::None>("Cancel file dialog.");
//		return;
//	}
//	else {//NFD_ERROR
//		GLog.add<FD::Log::Type::Error>("Error file dialog.");
//		throw std::runtime_error("NFD_OpenDialog() return NFD_ERROR.");
//	}
//
//	using enum FD::Coding::TabWrite::Exception;
//	try {
//		tabWrite->addFile(*outPath.get());
//	}
//	catch (const FD::Coding::TabWrite::Exception type) {
//		if (type == AlreadyExist) {
//			GLog.add<FD::Log::Type::None>("Request add Utils::MessageScene.");
//			Scene::addScene<Utils::Message>(text.error_alreadyExist, pos.center);
//		}
//		else if (type == LimitFileSizeMax) {
//			GLog.add<FD::Log::Type::None>("Request add Utils::MessageScene.");
//			Scene::addScene<Utils::Message>(text.error_limitMaxSize, pos.center);
//		}
//		else {
//			GLog.add<FD::Log::Type::Error>("abort() has been called. File {}.", __FILE__);
//			abort();
//		}
//	}
//}

void FS::Coding::Tab::update() {
	if (!tabRead->update())
		return;
	this->updateInfo();
}

void FS::Coding::Tab::updateInfo() {
	std::vector<std::string> paths = tabRead->getFilePaths();

	info.files.resize(paths.size());
	for (std::size_t i = 0, size = paths.size(); i < size; i++) {
		std::string name = FU::File::fileName(paths[i]);
		info.files[i] = { std::move(paths[i]),  std::move(name) };
	}
}

void FS::Coding::Tab::updateTextSaved() {
	if (!tabRead->update_isTextSaved())
		return;

	for (auto& x : info.files) {
		if (tabRead->isTextSaved(x.path)) {
			if (x.asterisk)
				x.name.pop_back();
			x.asterisk = false;
		}
		else {
			if (!x.asterisk)
				x.name += '*';
			x.asterisk = true;
		}
	}
}

void FS::Coding::Tab::fileList() {
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4());
	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2());
	const ImVec2 size = { ImGui::GetWindowSize().x - (2.0f * ImGui::GetStyle().WindowPadding.x) ,ImGui::GetFontSize() };

	ImGui::Spacing();

	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.3f,0.4f,0.9f,0.4f });

	for (uint16_t i = 0; auto & x : info.files) {
		if (i == select.index) {
			ImGui::PushStyleColor(ImGuiCol_Button, { 0.2f,0.3f,0.9f,0.5f });

			if (ImGui::Button(x.name.c_str(), size)) {
				select.index = i;
				this->display();
			}
			ImGui::PopStyleColor();
		}
		else {
			if (i == select.hovered)
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.2f,0.4f,0.8f,0.5f });

			if (ImGui::Button(x.name.c_str(), size)) {
				select.index = i;
				this->display();
			}

			if (i == select.hovered)
				ImGui::PopStyleColor();

		}

		if (ImGui::IsItemHovered())
			select.hovered = i;

		//close button
		this->closeButton();

		ImGui::Spacing();
		i++;
	}

	ImGui::PopStyleColor();//button hovered

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
}

void FS::Coding::Tab::display() {
	const std::string path = info.files.at(select.index).path;

	if (tabRead->getDisplayFilePaths().at(0) == path)
		return;

	GLog.add<FD::Log::Type::None>("Erase display file {}.", path);
	tabWrite->eraseDisplayFile(tabRead->getDisplayFilePaths().at(0));

	GLog.add<FD::Log::Type::None>("Add display file {}.", tabRead->getDisplayFilePaths().at(0));
	tabWrite->addDisplayFile(path);
}

void FS::Coding::Tab::closeButton() {
	using namespace FU::ImGui::Operators;

	if (!ImGui::IsItemHovered())
		return;

	ImGui::SameLine();

	const auto min = ImGui::GetItemRectMin();
	const auto max = ImGui::GetItemRectMax();

	constexpr ImU32 hoveredCol = FU::ImGui::convertImVec4ToImU32(0.8f, 0.2f, 0.2f, 0.6f);
	const ImVec2 pos = { max.x - ImGui::GetFontSize() - 3.0f,min.y };
	const ImVec2 pos2 = pos + ImGui::CalcTextSize(ICON_MD_CLOSE);

	if (ImGui::IsMouseHoveringRect(pos, pos2)) {
		ImGui::GetWindowDrawList()->AddText(pos, hoveredCol, ICON_MD_CLOSE);
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			this->close();
	}
	else
		ImGui::GetWindowDrawList()->AddText(pos, IM_COL32_WHITE, ICON_MD_CLOSE);

}

void FS::Coding::Tab::close() {
	auto& current = info.files.at(select.index);
	if (tabRead->isTextSaved(current.path)) {
		tabWrite->eraseFile(current.path);
		tabWrite->save();
		return;
	}

	GLog.add<FD::Log::Type::None>("Popup MessageBox.");
	int32_t button = FU::MB::button_button_cancel(FU::MB::Icon::Warning, text.popup_save, text.popup_saveAndClose, text.popup_withoutSaving, text.popup_cancel);
	if (button == 0) {//save close
		tabWrite->saveText(current.path);
		tabWrite->eraseFile(current.path);
	}
	else if (button == 1) {//without saving
		tabWrite->eraseFile(current.path);
	}
	else if (button == 2)//cancel
		return;
	else {//unexpected
		GLog.add<FD::Log::Type::Error>("abort() has been called. File {}.", __FILE__);
		abort();
	}

	tabWrite->save();
}

//if mouse right button clicked 
//popup menu
void FS::Coding::Tab::popup() {
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered() && ImGui::IsAnyItemHovered())
		ImGui::OpenPopup(PopupLabel);
	if (!ImGui::BeginPopup(PopupLabel))
		return;

	//ImGui::Selectable("test");

	ImGui::EndPopup();
}

void FS::Coding::Tab::popup_window() {
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
		ImGui::OpenPopup(PopupLabel_Window);
	if (!ImGui::BeginPopup(PopupLabel_Window))
		return;

	//ImGui::Selectable("test2");

	ImGui::EndPopup();
}



