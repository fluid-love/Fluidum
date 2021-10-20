#include "texteditor.h"

::TextEditor::LanguageDefinition FTE::getLuaLanguageDefinition() {
	::TextEditor::LanguageDefinition lua = ::TextEditor::LanguageDefinition::Lua();

	lua.mTokenRegexStrings.insert(lua.mTokenRegexStrings.begin() ,std::make_pair<std::string, ::TextEditor::PaletteIndex>("..t", ::TextEditor::PaletteIndex::ErrorMarker));

	return lua;
}
