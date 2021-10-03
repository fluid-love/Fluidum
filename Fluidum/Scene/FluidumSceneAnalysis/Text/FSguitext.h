#pragma once

#include "../../common/FSusing.h"
#include <FluidumGuiText/FluidumGuiText.h>

namespace FS::Text {

	using GuiText = Fluidum::Utils::GuiText::GuiText;

	struct Analysis final {
		Analysis();

		GuiText plot;
		GuiText function;

	};

}