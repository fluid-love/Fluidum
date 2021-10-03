#pragma once

#include "../Common/include.h"

namespace FVK::Internal {

	class Glfw final {
	public:
		Glfw(ManagerPassKey);
		~Glfw() = default;
		FluidumUtils_Class_Default_CopyMove(Glfw)

	public:
		void destroy();

	private:
		void init();
	};


}