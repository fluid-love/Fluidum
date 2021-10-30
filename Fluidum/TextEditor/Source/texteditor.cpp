#include "texteditor.h"
#include "../../Utils/include.h"

::TextEditor::LanguageDefinition FTE::getLuaLanguageDefinition() {
	::TextEditor::LanguageDefinition lua = ::TextEditor::LanguageDefinition::Lua();

	//lua.mTokenRegexStrings.insert(lua.mTokenRegexStrings.begin() ,std::make_pair<std::string, ::TextEditor::PaletteIndex>("..t", ::TextEditor::PaletteIndex::ErrorMarker));

	return lua;
}

::TextEditor::Palette FTE::getDarkPalette() {
	constexpr ::TextEditor::Palette p = { {
			0xff7f7f7f,	// Default
			0xffd69c56,	// Keyword	
			0xff00ff00,	// Number
			FU::ImGui::convertImVec4ToImU32(1.0f,1.0f,1.0f,1.0f),	// String
			0xff70a0e0, // Char literal
			0xffffffff, // Punctuation
			0xff408080,	// Preprocessor
			0xffaaaaaa, // Identifier
			0xff9bc64d, // Known identifier
			0xffc040a0, // Preproc identifier
			0xff206020, // Comment (single line)
			0xff406020, // Comment (multi line)
			FU::ImGui::convertImVec4ToImU32(0.02f,0.02f,0.02f,1.0f), // Background
			0xffe0e0e0, // Cursor
			0x80a06020, // Selection
			0x800020ff, // ErrorMarker
			0x40f08000, // Breakpoint
			0xff707000, // Line number
			0x40000000, // Current line fill
			0x40808080, // Current line fill (inactive)
			0x40a0a0a0, // Current line edge
		} };
	return p;
}
