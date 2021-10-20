#include "texteditor.h"

FS::TextEditor::TextEditor(
	FD::Coding::TabWrite* const tabWrite,
	const FD::Coding::TabRead* const tabRead,
	const FD::GuiRead* const guiRead,
	FD::ProjectWrite* const projectWrite,
	const FD::ProjectRead* const projectRead,
	const std::string& path
)
	: tabWrite(tabWrite), tabRead(tabRead), guiRead(guiRead), projectWrite(projectWrite), projectRead(projectRead)
{
	GLog.add<FD::Log::Type::None>("Construct TextEditorScene.");


	auto paths = tabRead->getDisplayFilePaths();

	for (auto& x : paths) {
		std::ifstream ifs(path);
		std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

		auto info_ = info.emplace_back(Info{ tabWrite->getEditor(x) ,path });
		info_.editor->SetLanguageDefinition(FTE::getLuaLanguageDefinition());
		info_.editor->SetText(str);
	}

	if (!path.empty())
		return;

	if (projectRead->getCurrentMainCodeType() == FD::Project::CodeType::Empty)
		projectWrite->setMainCodePath(tabRead->getDisplayFilePaths().at(0).c_str());
}

FS::TextEditor::~TextEditor() noexcept {
	try {
		GLog.add<FD::Log::Type::None>("Destruct TextEditorScene.");
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

	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.07f, 0.07f, 0.07f, 0.65f));

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());

	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(1024, 768), ImGuiCond_FirstUseEver);

	for (uint16_t i = 0; auto & x : this->info) {
		this->editor = x.editor;
		std::string label = text.editor.operator const char* ();
		(label += "##") += std::to_string(i++);
		ImGui::Begin(label.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar);

		this->textEditorMenu();
		this->textEditor();
		this->textEditorInfo();

		ImGui::End();
	}

	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor();

	this->textChange();
	this->update();
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

	if (ImGui::MenuItem(text.save)) {
		this->saveText();
	}
	if (ImGui::MenuItem(text.saveAs)) {

	}
	ImGui::Separator();

	if (ImGui::MenuItem(text.create)) {

	}
	if (ImGui::MenuItem(text.load)) {

	}
	ImGui::Spacing();

	ImGui::Separator();

	if (ImGui::MenuItem(text.quit))
		;

	ImGui::EndMenu();
}

void FS::TextEditor::editMenu() {
	if (!ImGui::BeginMenu(text.edit))
		return;

	bool ro = editor->IsReadOnly();
	if (ImGui::MenuItem(text.readOnly, nullptr, &ro))
		editor->SetReadOnly(ro);

	ImGui::Separator();

	if (ImGui::MenuItem(text.undo, "ALT-Backspace", nullptr, !ro && editor->CanUndo()))
		editor->Undo();
	if (ImGui::MenuItem(text.redo, "Ctrl-Y", nullptr, !ro && editor->CanRedo()))
		editor->Redo();

	ImGui::Separator();

	if (ImGui::MenuItem(text.copy, "Ctrl-C", nullptr, editor->HasSelection()))
		editor->Copy();
	if (ImGui::MenuItem(text.cut, "Ctrl-X", nullptr, !ro && editor->HasSelection()))
		editor->Cut();
	if (ImGui::MenuItem(text.del, "Del", nullptr, !ro && editor->HasSelection()))
		editor->Delete();
	if (ImGui::MenuItem(text.paste, "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
		editor->Paste();

	ImGui::Separator();

	if (ImGui::MenuItem(text.selectAll, nullptr, nullptr))
		editor->SetSelection(FTE::TextEditor::Coordinates(), FTE::TextEditor::Coordinates(editor->GetTotalLines(), 0));

	ImGui::EndMenu();

}

void FS::TextEditor::themeMenu() {
	if (!ImGui::BeginMenu(text.theme))
		return;

	auto palette = editor->GetPalette();

	if (ImGui::MenuItem(text.dark)) {
		palette.at(static_cast<std::size_t>(FTE::TextEditor::PaletteIndex::Background)) = ImGui::ColorConvertFloat4ToU32({ 0.05f,0.05f,0.05f,1.0f });
		palette.at(static_cast<std::size_t>(FTE::TextEditor::PaletteIndex::String)) = ImGui::ColorConvertFloat4ToU32({ 1.0f,1.0f,1.0f,1.0f });

		editor->SetPalette(palette);
	}
	if (ImGui::MenuItem(text.light))
		editor->SetPalette(FTE::TextEditor::GetLightPalette());
	if (ImGui::MenuItem(text.blue))
		editor->SetPalette(FTE::TextEditor::GetRetroBluePalette());
	ImGui::EndMenu();
}

void FS::TextEditor::textEditor() {
	editor->Render("TextEditor", { ImGui::GetWindowWidth(),ImGui::GetWindowHeight() * 0.93f });
}

void FS::TextEditor::textEditorInfo() {
	auto cpos = editor->GetCursorPosition();
	ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor->GetTotalLines(),
		editor->IsOverwrite() ? "Ovr" : "Ins",
		editor->CanUndo() ? "*" : " ",
		editor->GetLanguageDefinition().mName.c_str(), "test");
}

void FS::TextEditor::saveText() {
	auto textToSave = editor->GetText();
	const std::string path = getCurrentEditorPath();
	std::ofstream ofs(path, std::ios::trunc);
	ofs << textToSave;

	tabWrite->setIsTextChanged(path, false);
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
	if (!editor->IsTextChanged())
		return;

	std::cout << "a";
}

std::string FS::TextEditor::getCurrentEditorPath() {
	const auto itr = std::find_if(info.cbegin(), info.cend(), [&](auto& x) {return x.editor == this->editor; });
	return itr->path;
}

