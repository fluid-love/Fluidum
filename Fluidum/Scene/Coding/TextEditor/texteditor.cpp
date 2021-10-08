#include "texteditor.h"

FS::TextEditor::TextEditor(
	const FD::Coding::TabRead* const tabRead,
	const FD::GuiRead* const guiRead,
	FD::ProjectWrite* const projectWrite,
	const FD::ProjectRead* const projectRead,
	const std::string& path
)
	: tabRead(tabRead), guiRead(guiRead), projectWrite(projectWrite), projectRead(projectRead)
{
	GLog.add<FD::Log::Type::None>("Construct TextEditorScene.");

	editor.SetLanguageDefinition(FTE::getLuaLanguageDefinition());

	std::ifstream ifs(tabRead->getDisplayFilePath());
	std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	editor.SetText(str);

	if (projectRead->getCurrentMainCodeType() == FD::Project::CodeType::Empty)
		projectWrite->setMainCodePath(tabRead->getDisplayFilePath().c_str());

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


	ImGui::Begin(text.editor, nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar);


	this->textEditorMenu();
	this->textEditor();
	this->textEditorInfo();

	ImGui::End();

	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor();
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

	bool ro = editor.IsReadOnly();
	if (ImGui::MenuItem(text.readOnly, nullptr, &ro))
		editor.SetReadOnly(ro);

	ImGui::Separator();

	if (ImGui::MenuItem(text.undo, "ALT-Backspace", nullptr, !ro && editor.CanUndo()))
		editor.Undo();
	if (ImGui::MenuItem(text.redo, "Ctrl-Y", nullptr, !ro && editor.CanRedo()))
		editor.Redo();

	ImGui::Separator();

	if (ImGui::MenuItem(text.copy, "Ctrl-C", nullptr, editor.HasSelection()))
		editor.Copy();
	if (ImGui::MenuItem(text.cut, "Ctrl-X", nullptr, !ro && editor.HasSelection()))
		editor.Cut();
	if (ImGui::MenuItem(text.del, "Del", nullptr, !ro && editor.HasSelection()))
		editor.Delete();
	if (ImGui::MenuItem(text.paste, "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
		editor.Paste();

	ImGui::Separator();

	if (ImGui::MenuItem(text.selectAll, nullptr, nullptr))
		editor.SetSelection(FTE::TextEditor::Coordinates(), FTE::TextEditor::Coordinates(editor.GetTotalLines(), 0));

	ImGui::EndMenu();

}

void FS::TextEditor::themeMenu() {
	if (!ImGui::BeginMenu(text.theme))
		return;

	if (ImGui::MenuItem(text.dark))
		editor.SetPalette(FTE::TextEditor::GetDarkPalette());
	if (ImGui::MenuItem(text.light))
		editor.SetPalette(FTE::TextEditor::GetLightPalette());
	if (ImGui::MenuItem(text.blue))
		editor.SetPalette(FTE::TextEditor::GetRetroBluePalette());
	ImGui::EndMenu();
}

void FS::TextEditor::textEditor() {
	editor.Render("TextEditor");
}

void FS::TextEditor::textEditorInfo() {
	auto cpos = editor.GetCursorPosition();
	ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
		editor.IsOverwrite() ? "Ovr" : "Ins",
		editor.CanUndo() ? "*" : " ",
		editor.GetLanguageDefinition().mName.c_str(), "test");
}

void FS::TextEditor::saveText() {
	auto textToSave = editor.GetText();

	std::ofstream ofs(tabRead->getDisplayFilePath(), std::ios::trunc);

	ofs << textToSave;
}