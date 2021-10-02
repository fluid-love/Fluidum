#pragma once

#include "FVKtype.h"

//‘O•ûéŒ¾
namespace FVK::Internal::Manager {
	class Manager;
}

namespace FVK::Internal {

	class ManagerPassKey final {
	private:
		FluidumUtils_Class_Default_ConDestructor(ManagerPassKey)
		FluidumUtils_Class_Default_CopyMove(ManagerPassKey)
	private:
		friend ::FVK::Internal::Manager::Manager;
	};

}