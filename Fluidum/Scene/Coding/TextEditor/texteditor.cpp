#include "texteditor.h"
#include "../../Calc/Lua/lua.h"
#include <nfd.h>

FS::TextEditor::TextEditor(
	const FD::Style::VarRead* const varRead,
	FD::Coding::TabWrite* const tabWrite,
	const FD::Coding::TabRead* const tabRead,
	FD::Coding::DisplayWrite* const displayWrite,
	const FD::Coding::DisplayRead* const displayRead,
	const FD::GuiRead* const guiRead,
	FD::ProjectWrite* const projectWrite,
	const FD::ProjectRead* const projectRead,
	const FD::ProjectFilesRead* const projectFilesRead,
	FD::ToolBarWrite* const toolBarWrite,
	const FD::Style::ColorRead* const colorRead
) :
	varRead(varRead),
	tabWrite(tabWrite),
	tabRead(tabRead),
	displayWrite(displayWrite),
	displayRead(displayRead),
	guiRead(guiRead),
	projectWrite(projectWrite),
	projectRead(projectRead),
	toolBarWrite(toolBarWrite),
	colorRead(colorRead)
{
	FluidumScene_Log_Constructor(::FS::TextEditor);

	this->setInfo();

	this->checkSyntaxTimePoint = std::chrono::system_clock::now() + std::chrono::seconds(3);

	toolBarWrite->add(&TextEditor::toolBar, this, text.editor.operator const std::string & ());

	this->luaState = luaL_newstate();

	style.infoWindowHeight = ImGui::GetFontSize() * 5.0f;

	zoom.input.reserve(4);
}

FS::TextEditor::~TextEditor() noexcept {
	lua_close(luaState);

	toolBarWrite->remove<TextEditor>();

	FluidumScene_Log_Destructor(::FS::TextEditor);
}

void FS::TextEditor::call() {

	if (info.empty()) {
		this->windowEmpty();
		if (!displayRead->empty())
			this->setInfo();
		return;
	}

	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.07f, 0.07f, 0.07f, 0.65f));

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());


	for (Size i = 0; auto & x : this->info) {
		current = &info[i];

		std::string label = text.editor.operator const char* ();
		(label += "##") += std::to_string(i++);
		ImGui::Begin(label.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar);

		this->isWindowFocused();

		this->textEditorMenu();
		this->textEditor();
		this->breakPoint();

		this->textEditorInfo();

		this->checkSyntaxError();

		ImGui::End();
	}

	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor();

	this->textChange();
	this->update();
	this->shortcut();
}

void FS::TextEditor::toolBar() {
	if (info.empty())
		return;

	//save
	if (ImGui::Button(ICON_FA_SAVE)) {
		FU::ImGui::tooltip(anime.tool_save, text.save);
		this->saveText(this->selected);
	}

	this->tool_separator();
	ImGui::SameLine();

	if (ImGui::Button(ICON_FA_UNDO)) {
		FU::ImGui::tooltip(anime.tool_undo, text.undo);
		this->saveText(this->selected);
	}

	ImGui::SameLine();

	if (ImGui::Button(ICON_FA_REDO)) {
		FU::ImGui::tooltip(anime.tool_redo, text.redo);
		this->saveText(this->selected);
	}
}

void FS::TextEditor::tool_separator() {
	const ImVec2 pos1 = { ImGui::GetItemRectMax().x , ImGui::GetItemRectMin().y };
	const ImVec2 pos2 = ImGui::GetItemRectMax();

	ImGui::GetWindowDrawList()->AddLine(pos1, pos2, colorRead->toolBarVerticalSeparator());

}

void FS::TextEditor::isWindowFocused() {
	if (!ImGui::IsWindowFocused())
		return;

	this->selected = current;

	if (displayRead->isEditorFocused(current->editor))
		return;

	displayWrite->focusedEditor(current->info.path);
}

void FS::TextEditor::windowEmpty() {
	std::string label = text.editor.operator const char* ();
	label += "##0";
	ImGui::Begin(label.c_str(), nullptr);

	ImGui::End();
}

void FS::TextEditor::setInfo() {
	auto displayInfo = displayRead->info();

	for (auto& x : displayInfo) {
		std::ifstream ifs(x.path);
		std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

		auto info_ = this->info.emplace_back(Info{ tabWrite->getEditor(x.path) ,x , FD::Project::File::getSupportedFileType(x.path) });
		info_.editor->SetLanguageDefinition(FTE::getLuaLanguageDefinition());
		info_.editor->SetText(str);
		info_.editor->SetPalette(FTE::getDarkPalette());
	}
}

void FS::TextEditor::textEditorMenu() {

	if (!ImGui::BeginMenuBar())
		return;

	this->fileMenu();
	this->editMenu();
	this->themeMenu();

	ImGui::EndMenuBar();
}

void FS::TextEditor::fileMenu() {
	if (!ImGui::BeginMenu(text.file))
		return;

	if (ImGui::MenuItem(text.save_icon))
		this->saveText(current);

	varRead->iconDummy();
	if (ImGui::MenuItem(text.saveAs))
		this->saveAs(current);

	ImGui::EndMenu();
}

void FS::TextEditor::editMenu() {
	if (!ImGui::BeginMenu(text.edit))
		return;

	bool ro = current->editor->IsReadOnly();
	if (ImGui::MenuItem(text.readOnly, nullptr, &ro))
		current->editor->SetReadOnly(ro);

	ImGui::Separator();

	if (ImGui::MenuItem(text.undo_icon, "ALT-Backspace", nullptr, !ro && current->editor->CanUndo()))
		current->editor->Undo();
	if (ImGui::MenuItem(text.redo_icon, "Ctrl-Y", nullptr, !ro && current->editor->CanRedo()))
		current->editor->Redo();

	ImGui::Separator();

	if (ImGui::MenuItem(text.copy, "Ctrl-C", nullptr, current->editor->HasSelection()))
		current->editor->Copy();
	if (ImGui::MenuItem(text.cut, "Ctrl-X", nullptr, !ro && current->editor->HasSelection()))
		current->editor->Cut();
	if (ImGui::MenuItem(text.del, "Del", nullptr, !ro && current->editor->HasSelection()))
		current->editor->Delete();
	if (ImGui::MenuItem(text.paste, "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
		current->editor->Paste();

	ImGui::Separator();

	if (ImGui::MenuItem(text.selectAll, nullptr, nullptr))
		current->editor->SetSelection(FTE::TextEditor::Coordinates(), FTE::TextEditor::Coordinates(current->editor->GetTotalLines(), 0));

	ImGui::EndMenu();

}

void FS::TextEditor::themeMenu() {
	if (!ImGui::BeginMenu(text.theme))
		return;

	//TextEditor::GetDarkPalette == defaut , FTE::getDarkPalette == dark
	if (ImGui::MenuItem(text.default_)) {
		current->editor->SetPalette(FTE::TextEditor::GetDarkPalette());
	}
	if (ImGui::MenuItem(text.dark)) {
		current->editor->SetPalette(FTE::getDarkPalette());
	}
	if (ImGui::MenuItem(text.light))
		current->editor->SetPalette(FTE::TextEditor::GetLightPalette());
	if (ImGui::MenuItem(text.blue))
		current->editor->SetPalette(FTE::TextEditor::GetRetroBluePalette());
	ImGui::EndMenu();
}

void FS::TextEditor::textEditor() {
	ImGui::BeginChild("##Editor", { 0.0f, ImGui::GetWindowHeight() - style.infoWindowHeight });
	ImGui::SetWindowFontScale(current->info.zoomRatio);
	current->editor->Render(current->info.path.c_str());
	ImGui::EndChild();

	ImGui::Spacing();
}

void FS::TextEditor::textEditorInfo() {
	ImGui::BeginChild("TextEditorInfo");
	auto cpos = current->editor->GetCursorPosition();

	//zoom percentage
	this->editorInfo_zoom();

	ImGui::SameLine(); ImGui::Separator(); ImGui::SameLine();

	//line
	ImGui::Text(text.line); ImGui::SameLine();
	ImGui::Text(":%d  ", cpos.mLine + 1);

	ImGui::SameLine();

	//column
	ImGui::Text(text.column); ImGui::SameLine();
	ImGui::Text(":%d  ", cpos.mColumn + 1);

	ImGui::SameLine();

	//Ovr or Ins
	ImGui::Text(current->editor->IsOverwrite() ? "Ovr  " : "Ins  ");

	ImGui::SameLine();

	//Undo
	if (current->editor->CanUndo())
		ImGui::Text(ICON_FA_UNDO);
	else
		ImGui::TextDisabled(ICON_FA_UNDO);

	ImGui::SameLine();

	//language
	{
		using enum FD::Project::File::SupportedFileType;
		if (current->language == Lua) {
			ImGui::Text("Lua");
		}
		else if (current->language == Python) {
			ImGui::Text("Python");
		}
		else if (current->language == Cpp) {
			ImGui::Text("C++");
		}
	}

	ImGui::SameLine();


	ImGui::EndChild();
}

void FS::TextEditor::editorInfo_zoom() {
	constexpr const char* percent[] = {
		" 30 % ", " 50 % ", " 70 % ", " 100 % ", " 120 % ", " 150 % ", " 200 % ", " 300 % "
	};

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2());
	ImGui::PopStyleVar();
	const std::string preview = std::to_string(std::roundf(current->info.zoomRatio * 100.0f)) + " %";

	ImGui::InputText("##ZoomInput", zoom.input.data(), zoom.input.capacity(), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsDecimal);

	ImGui::SameLine();

	//combo
	if (!ImGui::BeginCombo("##ZoomCombo", "", ImGuiComboFlags_NoPreview))
		return;

	for (auto x : percent) {
		ImGui::Selectable(x);
	}

	ImGui::EndCombo();
}

void FS::TextEditor::saveText(const Info* const info) {
	GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Save text({}).", current->info.path);
	tabWrite->saveText(current->info.path);
}

void FS::TextEditor::saveAs(const Info* const info) {
	std::unique_ptr<nfdchar_t*> outPath = std::make_unique<nfdchar_t*>();
	GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Save dialog.");
	const nfdresult_t result = NFD_SaveDialog(nullptr, nullptr, outPath.get());
	if (result == NFD_OKAY) {
		GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Save file {}.", *outPath.get());
	}
	else if (result == NFD_CANCEL) {
		GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Cancel save dialog.");
		return;
	}
	else {//NFD_ERROR
		GLog.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Error file dialog.");
		FluidumScene_Log_InternalWarning();
		return;
	}
}

void FS::TextEditor::update() {
	if (!displayRead->changed())
		return;

	auto paths = displayRead->paths();

	for (auto& x : paths) {
		//not new file
		auto itr = std::find_if(info.begin(), info.end(), [&](auto& y) {return y.info.path == x; });
		if (itr != info.end()) {
			continue;
		}

		std::ifstream ifs(x);
		std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
		auto info_ = info.emplace_back(Info{ tabWrite->getEditor(x) ,x });
		info_.editor->SetLanguageDefinition(FTE::getLuaLanguageDefinition());
		info_.editor->SetText(str);
	}

	for (const auto& x : paths) {
		auto itr = std::erase_if(info, [&](auto& y)
			{
				auto itr = std::find_if(info.begin(), info.end(), [&](auto&) {return y.info.path == x; });
				return itr == info.end();
			});
	}
}

void FS::TextEditor::textChange() {
	if (!current->editor->IsTextChanged())
		return;

	tabWrite->setIsTextSaved(current->info.path, false);
}

std::string FS::TextEditor::getCurrentEditorPath() {
	const auto itr = std::find_if(info.cbegin(), info.cend(), [&](auto& x) {return x.editor == current->editor; });
	return itr->info.path;
}

void FS::TextEditor::breakPoint() {
	if (!ImGui::IsWindowFocused())
		return;
	if (!ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		return;

	const ImVec2 pos = ImGui::GetWindowPos();
	const ImVec2 size = ImGui::GetWindowSize();

	if (ImGui::IsMouseHoveringRect(pos, { pos.x + size.x,pos.y + size.y })) {

		//FTE::TextEditor::Breakpoints points = {1};
		//current->editor->SetBreakpoints(points);
	}

}

void FS::TextEditor::checkSyntaxError() {
	if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
		return;
	if (checkSyntaxTimePoint > std::chrono::system_clock::now())
		return;

	{
		using enum FD::Project::File::SupportedFileType;
		if (current->language == Lua)
			this->checkLua();
		else if (current->language == Python)
			this->checkPython();
		else if (current->language == Cpp)
			this->checkAngelScript();
	}


	//every 4 seconds
	this->checkSyntaxTimePoint = std::chrono::system_clock::now() + std::chrono::seconds(4);
}

void FS::TextEditor::checkLua() {
	const std::string code = current->editor->GetText();
	const int err = luaL_loadstring(luaState, code.c_str());
	FTE::TextEditor::ErrorMarkers markers{};

	if (err == LUA_OK)
		;
	else if (err == LUA_ERRSYNTAX) {
		const char* message = lua_tostring(luaState, -1);

		std::cmatch match{};
		const bool m = std::regex_search(message, match, std::regex(":[0123456789]:"));
		if (m) {
			std::string str = match.str();
			str = str.at(1);
			int line = std::stoi(str);
			//over -> eof error
			line > current->editor->GetTotalLines() ? line-- : line;

			//message
			std::string errMesssage = message;
			auto itr = std::find(errMesssage.begin(), errMesssage.end(), str[0]);
			errMesssage.erase(errMesssage.begin(), itr);
			markers.insert(std::pair(line, errMesssage));
		}
	}

	current->editor->SetErrorMarkers(markers);
	LuAssist::Utils::popAll(luaState);
}

void FS::TextEditor::checkPython() {

}

void FS::TextEditor::checkAngelScript() {

}

void FS::TextEditor::shortcut() {
	this->shortcut_zoom();
}

void FS::TextEditor::shortcut_zoom() {
	const auto& io = ImGui::GetIO();

	if (!io.KeyCtrl || (io.MouseWheel == 0.0f))
		return;

	if (io.MouseWheel > 0.0f) {
		selected->info.zoomRatio += 0.1f;
	}
	else {
		selected->info.zoomRatio -= 0.1f;
	}

}


































