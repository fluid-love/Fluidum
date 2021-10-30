#pragma once

#include "../../../Libraries/ImGuiColorTextEdit/TextEditor.h"

namespace FTE {

	//FluidumÇ…ì¡âªÇµÇΩç\ï∂ã≠í≤
	::TextEditor::LanguageDefinition getLuaLanguageDefinition();

	//Dark
	::TextEditor::Palette getDarkPalette();

	class TextEditor final : public ::TextEditor {



	};

}