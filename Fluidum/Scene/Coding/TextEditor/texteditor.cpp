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
	FD::ToolBarWrite* const toolBarWrite
) :
	varRead(varRead),
	tabWrite(tabWrite),
	tabRead(tabRead),
	displayWrite(displayWrite),
	displayRead(displayRead),
	guiRead(guiRead),
	projectWrite(projectWrite),
	projectRead(projectRead),
	toolBarWrite(toolBarWrite)
{
	FluidumScene_Log_Constructor("TextEditor");

	this->setInfo();

	this->checkSyntaxTimePoint = std::chrono::system_clock::now() + std::chrono::seconds(3);

	toolBarWrite->add(&TextEditor::toolBar, this, text.editor.operator const std::string & ());

	this->luaState = luaL_newstate();
}

FS::TextEditor::~TextEditor() noexcept {
	try {
		lua_close(luaState);

		toolBarWrite->remove<TextEditor>();

		FluidumScene_Log_Destructor("TextEditor");
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

void FS::TextEditor::call() {

	if (info.empty()) {
		this->windowEmpty();
		if (!tabRead->empty())
			this->setInfo();
		return;
	}

	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.07f, 0.07f, 0.07f, 0.65f));

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());


	for (std::size_t i = 0; auto & x : this->info) {
		current = &info[i];

		std::string label = text.editor.operator const char* ();
		(label += "##") += std::to_string(i++);
		ImGui::Begin(label.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar);

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
}

void FS::TextEditor::toolBar() {

}

void FS::TextEditor::windowEmpty() {
	std::string label = text.editor.operator const char* ();
	label += "##0";
	ImGui::Begin(label.c_str());

	ImGui::End();
}

void FS::TextEditor::setInfo() {
	auto paths = tabRead->getDisplayFilePaths();

	for (auto& x : paths) {
		std::ifstream ifs(x);
		std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

		auto info_ = info.emplace_back(Info{ tabWrite->getEditor(x) ,x, FD::Project::File::getSupportedFileType(x) });
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

	if (ImGui::MenuItem(text.save))
		this->saveText();

	varRead->iconDummy();
	if (ImGui::MenuItem(text.saveAs))
		this->saveAs();

	ImGui::EndMenu();
}

void FS::TextEditor::editMenu() {
	if (!ImGui::BeginMenu(text.edit))
		return;

	bool ro = current->editor->IsReadOnly();
	if (ImGui::MenuItem(text.readOnly, nullptr, &ro))
		current->editor->SetReadOnly(ro);

	ImGui::Separator();

	if (ImGui::MenuItem(text.undo, "ALT-Backspace", nullptr, !ro && current->editor->CanUndo()))
		current->editor->Undo();
	if (ImGui::MenuItem(text.redo, "Ctrl-Y", nullptr, !ro && current->editor->CanRedo()))
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
	current->editor->Render("TextEditor", { ImGui::GetWindowWidth(),ImGui::GetWindowHeight() * 0.86f });
}

void FS::TextEditor::textEditorInfo() {
	ImGui::BeginChild("TextEditorInfo");
	auto cpos = current->editor->GetCursorPosition();

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

void FS::TextEditor::saveText() {
	GLog.add<FD::Log::Type::None>("Save text({}).", current->path);
	tabWrite->saveText(current->path);
}

void FS::TextEditor::saveAs() {
	std::unique_ptr<nfdchar_t*> outPath = std::make_unique<nfdchar_t*>();
	GLog.add<FD::Log::Type::None>("Save dialog.");
	const nfdresult_t result = NFD_SaveDialog(nullptr, nullptr, outPath.get());
	if (result == NFD_OKAY) {
		GLog.add<FD::Log::Type::None>("Save file {}.", *outPath.get());
	}
	else if (result == NFD_CANCEL) {
		GLog.add<FD::Log::Type::None>("Cancel save dialog.");
		return;
	}
	else {//NFD_ERROR
		GLog.add<FD::Log::Type::Error>("Error file dialog.");
		throw std::runtime_error("NFD_OpenDialog() return NFD_ERROR.");
	}
}

void FS::TextEditor::update() {
	if (!tabRead->isDisplayFileChanged())
		return;

	auto paths = tabRead->getDisplayFilePaths();

	for (auto& x : paths) {
		//not new file
		auto itr = std::find_if(info.begin(), info.end(), [&](auto& y) {return y.path == x; });
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
				auto itr = std::find_if(info.begin(), info.end(), [&](auto&) {return y.path == x; });
				return itr == info.end();
			});
	}
}

void FS::TextEditor::textChange() {
	if (!current->editor->IsTextChanged())
		return;

	tabWrite->setIsTextSaved(current->path, false);
}

std::string FS::TextEditor::getCurrentEditorPath() {
	const auto itr = std::find_if(info.cbegin(), info.cend(), [&](auto& x) {return x.editor == current->editor; });
	return itr->path;
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
