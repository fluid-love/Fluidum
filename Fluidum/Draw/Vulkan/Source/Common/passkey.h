#pragma once

#include "type.h"

//forward declaration
namespace FVK::Internal::Manager {
	class Manager;
}

namespace FVK::Internal {

	class ManagerPassKey final {
	private:
		FluidumUtils_Class_Default_ConDestructor_Noexcept(ManagerPassKey)
		FluidumUtils_Class_Default_CopyMove_Noexcept(ManagerPassKey)
	private:
		friend ::FVK::Internal::Manager::Manager;
	};

}