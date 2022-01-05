#pragma once

#include "../Base/base.h"

namespace FDR::Internal::Initialization {

	struct FullScreenWithBarInfo final {
		const char* title = "";
	};

	//initialize
	void initialize(const FullScreenWithBarInfo& info);

}