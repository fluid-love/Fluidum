#include "texteditor.h"

FS::TextEditor::TextEditor(
	const FD::GuiRead* const guiRead,
	const std::string& path
)
	: guiRead(guiRead)
{
	GLog.add<FD::Log::Type::None>("Construct TextEditorScene.");

	editor.SetLanguageDefinition(FTE::getLuaLanguageDefinition());
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


	ImGui::Begin("TextEditor", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar);


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

	if (ImGui::BeginMenu("File")) {
		if (ImGui::MenuItem("Save")) {

			auto textToSave = editor.GetText();
			/// save text....
		}
		if (ImGui::MenuItem("Quit"))
			;
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Edit")) {
		bool ro = editor.IsReadOnly();
		if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
			editor.SetReadOnly(ro);
		ImGui::Separator();

		if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && editor.CanUndo()))
			editor.Undo();
		if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && editor.CanRedo()))
			editor.Redo();

		ImGui::Separator();

		if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, editor.HasSelection()))
			editor.Copy();
		if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && editor.HasSelection()))
			editor.Cut();
		if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor.HasSelection()))
			editor.Delete();
		if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
			editor.Paste();

		ImGui::Separator();

		if (ImGui::MenuItem("Select all", nullptr, nullptr))
			editor.SetSelection(FTE::TextEditor::Coordinates(), FTE::TextEditor::Coordinates(editor.GetTotalLines(), 0));

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("View")) {
		if (ImGui::MenuItem("Dark palette"))
			editor.SetPalette(FTE::TextEditor::GetDarkPalette());
		if (ImGui::MenuItem("Light palette"))
			editor.SetPalette(FTE::TextEditor::GetLightPalette());
		if (ImGui::MenuItem("Retro blue palette"))
			editor.SetPalette(FTE::TextEditor::GetRetroBluePalette());
		ImGui::EndMenu();
	}
	ImGui::EndMenuBar();
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
