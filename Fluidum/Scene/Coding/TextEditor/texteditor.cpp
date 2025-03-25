#include "texteditor.h"
#include "../../Calc/Lua/lua.h"
#include "../../Utils/Popup/message.h"
#include <nfd.h>
#include <imgui_internal.h>

FS::TextEditor::TextEditor(
	FD::ImGuiWindowWrite* const imguiWindowWrite,
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
	imguiWindowWrite(imguiWindowWrite),
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
	//It catches events of addition and deletion.
	this->update();

	if (info.empty()) {
		this->windowEmpty();
		if (!displayRead->empty())
			this->setInfo();
		return;
	}

	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.07f, 0.07f, 0.07f, 0.65f));

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);


	for (Size i = 0; auto & x : this->info) {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());

		ImGui::SetNextWindowSizeConstraints(varRead->viewWindowSizeConstraints(), FU::ImGui::vec2Max());

		zoom.input = std::to_string(static_cast<UIF32>(std::round(x.info.zoomRatio * 100.0f)));

		current = &info[i];

		std::string label = text.editor.operator const char* ();
		(label += "##") += std::to_string(i++);
		ImGui::Begin(label.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar);

		ImGui::PopStyleVar();

		this->isWindowFocused();

		this->textEditorMenu();
		this->textEditor();
		this->breakPoint();

		this->textEditorInfo();

		this->checkSyntaxError();

		ImGui::End();
	}

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();

	this->textChange();
	this->shortcut();
	this->catchZoom();
}

void FS::TextEditor::setImGuiWindow() {
	imguiWindowWrite->set(FU::Class::ClassCode::GetClassCode<TextEditor>(), ImGui::GetCurrentWindow());
}

void FS::TextEditor::toolBar() {
	if (info.empty()) {
		this->toolBar_dummy();
		return;
	}

	//undo
	this->toolBar_undo();

	ImGui::SameLine();

	//redo
	this->toolBar_redo();

}

void FS::TextEditor::toolBar_dummy() {

	//undo
	ImGui::AlignTextToFramePadding();
	ImGui::TextDisabled(ICON_FA_UNDO);
	FU::ImGui::tooltip(anime.tool_undo, text.undo);

	ImGui::SameLine();

	//redo
	ImGui::AlignTextToFramePadding();
	ImGui::TextDisabled(ICON_FA_REDO);
	FU::ImGui::tooltip(anime.tool_redo, text.redo);

}

void FS::TextEditor::toolBar_undo() {
	if (!current->editor->CanUndo()) {
		//dummy
		ImGui::AlignTextToFramePadding();
		ImGui::TextDisabled(ICON_FA_UNDO);
		FU::ImGui::tooltip(anime.tool_undo, text.undo);
		return;
	}

	//canundo

	if (!ImGui::Button(ICON_FA_UNDO)) {
		FU::ImGui::tooltip(anime.tool_undo, text.undo);
		return;
	}

	current->editor->Undo();

}

void FS::TextEditor::toolBar_redo() {
	if (!current->editor->CanRedo()) {
		//dummy
		ImGui::AlignTextToFramePadding();
		ImGui::TextDisabled(ICON_FA_REDO);
		FU::ImGui::tooltip(anime.tool_redo, text.redo);
		return;
	}

	//canredo

	current->editor->Undo();
	if (ImGui::Button(ICON_FA_REDO)) {
		FU::ImGui::tooltip(anime.tool_redo, text.redo);
		current->editor->Redo();
	}

}

void FS::TextEditor::tool_separator() {
	const ImVec2 pos1 = { ImGui::GetItemRectMax().x , ImGui::GetItemRectMin().y };
	const ImVec2 pos2 = ImGui::GetItemRectMax();

	ImGui::GetWindowDrawList()->AddLine(pos1, pos2, colorRead->toolBarVerticalSeparator());

}

void FS::TextEditor::isWindowFocused() {
	if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
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
	this->info.clear();

	for (auto& x : displayInfo) {
		std::ifstream ifs(x.path);
		std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

		auto& info_ = this->info.emplace_back(Info{ tabWrite->getEditor(x.path) ,x , FD::Project::File::getSupportedFileType(x.path) });
		info_.name = FU::File::fileName(x.path);
		info_.nameWidth = ImGui::CalcTextSize(info_.name.c_str()).x;
		info_.editor->SetLanguageDefinition(FTE::getLuaLanguageDefinition());
		info_.editor->SetText(str);

		//theme
		{
			using enum FD::Coding::DisplayInfo::Theme;
			if (x.theme == Default)
				info_.editor->SetPalette(FTE::TextEditor::GetDarkPalette());
			else if (x.theme == Dark)
				info_.editor->SetPalette(FTE::getDarkPalette());
			else if (x.theme == Light)
				info_.editor->SetPalette(FTE::TextEditor::GetLightPalette());
			else
				info_.editor->SetPalette(FTE::TextEditor::GetRetroBluePalette());
		}
	}
}

void FS::TextEditor::textEditorMenu() {

	if (!ImGui::BeginMenuBar())
		return;

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { ImGui::GetStyle().ItemSpacing.x, ImGui::GetStyle().ItemSpacing.y * 1.85f });
	this->fileMenu();
	this->editMenu();
	this->themeMenu();
	ImGui::PopStyleVar();

	ImGui::EndMenuBar();

}

void FS::TextEditor::fileMenu() {
	if (!ImGui::BeginMenu(text.file))
		return;

	if (ImGui::MenuItem(text.save_icon))
		this->saveText(current);

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
		displayWrite->theme(current->info.path, FD::Coding::DisplayInfo::Theme::Default);
		tabWrite->save();
	}
	if (ImGui::MenuItem(text.dark)) {
		current->editor->SetPalette(FTE::getDarkPalette());
		displayWrite->theme(current->info.path, FD::Coding::DisplayInfo::Theme::Dark);
		tabWrite->save();
	}
	if (ImGui::MenuItem(text.light)) {
		current->editor->SetPalette(FTE::TextEditor::GetLightPalette());
		displayWrite->theme(current->info.path, FD::Coding::DisplayInfo::Theme::Light);
		tabWrite->save();
	}
	if (ImGui::MenuItem(text.blue)) {
		current->editor->SetPalette(FTE::TextEditor::GetRetroBluePalette());
		displayWrite->theme(current->info.path, FD::Coding::DisplayInfo::Theme::Light);
		tabWrite->save();
	}
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
	this->textEditorInfo_left();
	ImGui::SameLine();
	this->textEditorInfo_right();
}

void FS::TextEditor::textEditorInfo_left() {
	const float windowWidth = ImGui::GetWindowWidth() - current->nameWidth - (ImGui::GetStyle().WindowPadding.x * 2.0f);

	ImGui::BeginChild("TextEditorInfoLeft", { windowWidth, 0.0f });
	auto cpos = current->editor->GetCursorPosition();

	ImGui::Spacing(); ImGui::SameLine();

	//zoom percentage
	this->editorInfo_zoom();

	ImGui::SameLine(); ImGui::Spacing();
	ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();

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
	ImGui::AlignTextToFramePadding();
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


	ImGui::EndChild();
}

void FS::TextEditor::textEditorInfo_right() {

	ImGui::BeginChild("TextEditorInfoRight");

	ImGui::Text(current->name.c_str());

	ImGui::EndChild();
}

void FS::TextEditor::editorInfo_zoom() {
	constexpr UI16 percent[] = {
		30, 50, 70, 100, 120, 150, 200, 300
	};

	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 3.2f);

	const bool focus = ImGui::InputText("##ZoomInput", zoom.input.data(), zoom.input.capacity(), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsDecimal);
	if (focus) {
		pos.zoom = ImGui::GetItemRectMin();
		flag.inputZoom = true;
	}
	flag.zoomFocused = focus;

	ImGui::SameLine();

	//combo
	if (!ImGui::BeginCombo("##ZoomCombo", "", ImGuiComboFlags_NoPreview))
		return;

	for (auto x : percent) {
		const std::string label = std::to_string(x) + '%';
		if (ImGui::Selectable(label.c_str())) {
			current->info.zoomRatio = x / 100.0f;
		}
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
		auto itr = std::find_if(info.begin(), info.end(), [&](auto& y) {return y.info.path == x; });
		if (itr != info.end()) {
			//Not a new file
			continue;
		}

		std::ifstream ifs(x);
		std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
		auto& info_ = info.emplace_back(Info{ tabWrite->getEditor(x) ,x });
		info_.name = FU::File::fileName(x);
		info_.nameWidth = ImGui::CalcTextSize(info_.name.c_str()).x;
		info_.editor->SetLanguageDefinition(FTE::getLuaLanguageDefinition());
		info_.editor->SetText(str);
	}

	auto itr = std::erase_if(info, [&](auto& x)
		{
			const auto find = std::find(paths.cbegin(), paths.cend(), x.info.path);
			return find == paths.cend();
		}
	);

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
	if (!selected)
		return;

	this->shortcut_zoom();
}

void FS::TextEditor::shortcut_zoom() {
	const auto& io = ImGui::GetIO();

	if (!io.KeyCtrl || (io.MouseWheel == 0.0f))
		return;

	const float val = current->info.zoomRatio * 100.0f;

	if (io.MouseWheel > 0.0f) {
		//limit
		if ((val + 0.1f) >= FD::Coding::TextEditor::Limits::ZoomMax)
			return;
		selected->info.zoomRatio += 0.1f;
	}
	else {
		//limit
		if ((val - 0.1f) <= FD::Coding::TextEditor::Limits::ZoomMin)
			return;
		selected->info.zoomRatio -= 0.1f;
	}

	displayWrite->zoomRatio(selected->info.path, selected->info.zoomRatio);
	tabWrite->save();
}

void FS::TextEditor::catchZoom() {
	if (!flag.inputZoom)
		return;

	if (flag.zoomFocused)
		return;

	flag.inputZoom = false;

	const float val = std::stof(std::string(zoom.input.c_str()));
	const UIF16 roundedVal = static_cast<UIF16>(std::round(val));

	//check
	//is number
	const bool isNumber = std::all_of(zoom.input.cbegin(), zoom.input.cend(), [](char c) { return std::isdigit(c) != 0; });
	if (!isNumber) {
		FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
		Scene::addScene<Utils::Message>(text.error_forbiddenCharactor, pos.zoom);
	}

	//limits
	if (roundedVal >= FD::Coding::TextEditor::Limits::ZoomMax)
		return;
	if (roundedVal <= FD::Coding::TextEditor::Limits::ZoomMin)
		return;

	selected->info.zoomRatio = val;

}


































