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
	FD::Coding::DisplayWrite* const displayWrite,
	const FD::Coding::DisplayRead* const displayRead,
	const FD::ProjectRead* const projectRead
) :
	tabWrite(tabWrite),
	tabRead(tabRead),
	displayWrite(displayWrite),
	displayRead(displayRead),
	projectRead(projectRead)
{
	FluidumScene_Log_Constructor(::FS::Coding::Tab);

	this->updateInfo();
}

FS::Coding::Tab::~Tab() noexcept {
	FluidumScene_Log_Destructor(::FS::Coding::Tab);
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

	FluidumScene_Log_RequestDeleteScene(::FS::Coding::Tab);
	Scene::deleteScene<Tab>();
}

void FS::Coding::Tab::update() {
	if (!tabRead->update())
		return;
	this->updateInfo();
}

void FS::Coding::Tab::updateInfo() {
	std::vector<std::string> paths = tabRead->paths();

	info.files.resize(paths.size());
	for (Size i = 0, size = paths.size(); i < size; i++) {
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

	for (UIF16 i = 0; auto & x : info.files) {
		if (i == select.index) {
			ImGui::PushStyleColor(ImGuiCol_Button, { 0.2f,0.3f,0.9f,0.5f });

			if (ImGui::Button(x.name.c_str(), size)) {
				pos.clicked = FU::ImGui::messagePos();
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
	const std::string& path = info.files.at(select.index).path;

	//Not exists
	{
		const bool exist = this->checkExistsFile(path);
		if (!exist) {
			FU::MB::error(text.error_notExist);
			return;
		}
	}

	//None of the editors are displayed.
	if (displayRead->empty()) {
		const bool success = this->addDisplayPath(path);
		if (!success) {
			FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
			Scene::addScene<Utils::Message>(text.error_unexpected, pos.clicked);
		}
		tabWrite->save();
		return;
	}

	try {
		//The currently displayed editor is the same as the selected editor.
		const bool focused = displayRead->isEditorFocused(path);
		if (focused)
			return;
	}
	catch (...) {
		FluidumScene_Log_UnexpectedExceptionWarning();
		FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
		Scene::addScene<Utils::Message>(text.error_unexpected, pos.clicked);
		return;
	}

	std::string current{};
	try {
		current = displayRead->focusedFilePath();
	}
	catch (...) {
		FluidumScene_Log_UnexpectedExceptionWarning();
		FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
		Scene::addScene<Utils::Message>(text.error_unexpected, pos.clicked);
		return;
	}

	//None of the editors are focused.
	if (current.empty()) {
		std::vector<std::string> paths{};
		try {
			paths = displayRead->paths();
		}
		catch (...) {
			FluidumScene_Log_UnexpectedExceptionWarning();
			FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
			Scene::addScene<Utils::Message>(text.error_unexpected, pos.clicked);
			return;
		}

		if (paths.empty()) {
			const bool success = this->addDisplayPath(path);
			if (!success) {
				FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
				Scene::addScene<Utils::Message>(text.error_unexpected, pos.clicked);
			}
			tabWrite->save();
			return;
		}
		else {
			{
				const bool success = this->addDisplayPath(path);
				if (!success) {
					FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
					Scene::addScene<Utils::Message>(text.error_unexpected, pos.clicked);
					return;
				}
			}
			//remove front
			{
				const bool success = this->removeDisplayPath(paths[0]);
				if (!success) {
					try { displayWrite->remove(path); }
					catch (...) {
						FluidumScene_Log_SeriousError();
						std::terminate();
					}
					FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
					Scene::addScene<Utils::Message>(text.error_unexpected, pos.clicked);
					return;
				}
				tabWrite->save();
			}
		}
	}
	else {
		{
			const bool success = this->addDisplayPath(path);
			if (!success) {
				FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
				Scene::addScene<Utils::Message>(text.error_unexpected, pos.clicked);
				return;
			}
		}

		//remove focused editor
		{
			const bool success = this->removeDisplayPath(current);
			if (!success) {
				try { displayWrite->remove(path); }
				catch (...) {
					FluidumScene_Log_SeriousError();
					std::terminate();
				}
				FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
				Scene::addScene<Utils::Message>(text.error_unexpected, pos.clicked);
				return;
			}
		}
		tabWrite->save();
	}

}

bool FS::Coding::Tab::addDisplayPath(const std::string& path) noexcept {
	GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Add display file({}).", path);

	try {
		displayWrite->add(path);
	}
	catch (const FD::Coding::DisplayWrite::Exception val) {
		if (val == FD::Coding::DisplayWrite::Exception::NotFound) {
			;
		}
		else {
			FluidumScene_Log_InternalWarning_Enum(val);
		}
		FluidumScene_Log_InternalWarning();
		return false;
	}
	catch (...) {
		FluidumScene_Log_UnexpectedExceptionWarning();
		return false;
	}
	return true;
}

bool FS::Coding::Tab::removeDisplayPath(const std::string& path) noexcept {
	GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Remove display file({}).", path);

	try {
		displayWrite->remove(path);
	}
	catch (const FD::Coding::DisplayWrite::Exception val) {
		if (val == FD::Coding::DisplayWrite::Exception::NotFound) {
			;
		}
		else {
			FluidumScene_Log_InternalWarning_Enum(val);
		}
		FluidumScene_Log_InternalWarning();
		return false;
	}
	catch (...) {
		FluidumScene_Log_UnexpectedExceptionWarning();
		return false;
	}
	return true;
}

bool FS::Coding::Tab::checkExistsFile(const std::string& path) noexcept {
	try {
		return std::filesystem::exists(path);
	}
	catch (const std::filesystem::filesystem_error& error) {
		GLog.add<FU::Log::Type::Warning>(__FILE__, __LINE__, "std::filesystem::filesystem_error was thrown({}).", error.what());
	}

	return false;
}

void FS::Coding::Tab::closeButton() {
	using namespace FU::ImGui::Operators;

	if (!ImGui::IsItemHovered())
		return;

	ImGui::SameLine();

	const auto min = ImGui::GetItemRectMin();
	const auto max = ImGui::GetItemRectMax();

	constexpr ImU32 hoveredCol = FU::ImGui::ConvertImVec4ToImU32(0.8f, 0.2f, 0.2f, 0.6f);
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
		tabWrite->remove(current.path);
		tabWrite->save();
		return;
	}

	const auto button = FU::MB::button_button_cancel(FU::MB::Icon::Warning, text.popup_save, text.popup_saveAndClose, text.popup_withoutSaving, text.popup_cancel);
	if (button == 0) {//save close
		tabWrite->saveText(current.path);
		tabWrite->remove(current.path);
	}
	else if (button == 1) {//without saving
		tabWrite->remove(current.path);
	}
	else if (button == 2)//cancel
		return;
	else {//unexpected
		FluidumScene_Log_InternalWarning();
		return;
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



