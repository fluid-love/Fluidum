#pragma once

#include "../../../Libraries/ImGuiColorTextEdit/TextEditor.h"

namespace FTE {

	//Fluidum�ɓ��������\������
	::TextEditor::LanguageDefinition getLuaLanguageDefinition();

	//Dark
	::TextEditor::Palette getDarkPalette();

	class TextEditor final : public ::TextEditor {



	};

}