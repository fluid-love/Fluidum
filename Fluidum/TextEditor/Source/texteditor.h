#pragma once

#include "../../../External/ImGuiColorTextEdit/TextEditor.h"

namespace FTE {

	//Fluidum�ɓ��������\������
	::TextEditor::LanguageDefinition getLuaLanguageDefinition();

	//Dark
	::TextEditor::Palette getDarkPalette();

	class TextEditor final : public ::TextEditor {



	};

}