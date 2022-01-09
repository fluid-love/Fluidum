#pragma once

#include "../Common/include.h"

namespace FVK::Internal {

	class Glfw final {
	public:
		/*
		Exception:
			FailedToCreate
			NotSupported   -> Vulkan is not supported.
		*/
		//strong
		Glfw(ManagerPassKey);//initialize GLFW
		~Glfw() = default;
		FluidumUtils_Class_Default_CopyMove(Glfw);

	public:
		//no-throw
		void destroy() const noexcept;

	private:
		/*
		Exception:
			FailedToCreate
			NotSupported   -> Vulkan is not supported.
		*/
		//strong
		void init() const;
	};

}