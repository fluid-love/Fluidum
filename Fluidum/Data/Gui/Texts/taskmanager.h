#pragma once

#include "../text.h"

namespace FD::Text::Internal {

	struct TaskManager final {
		TaskManager();
		~TaskManager() noexcept = default;

		GuiText taskManager;
		GuiText cpuLoad;
		GuiText numOfPhysicalProcessor;
		GuiText numOfLogicalProcessor;
		GuiText cpuArchitecture;

	};

}

namespace FD::Text {

	using ::FD::Text::Internal::TaskManager;

}
