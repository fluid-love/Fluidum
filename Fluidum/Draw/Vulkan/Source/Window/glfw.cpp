#include "glfw.h"
#include "../Exception/glfw.h"

FVK::Internal::Glfw::Glfw(ManagerPassKey) {
	this->init();
}

void FVK::Internal::Glfw::init() const {
	{
		auto result = glfwInit();

		if (result != GLFW_TRUE) {
			GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to initialize glfw.");
			Exception::throwFailedToCreate();
		}
	}

	//https://www.glfw.org/docs/3.3/group__vulkan.html#ga1abcbe61033958f22f63ef82008874b1
	if (glfwVulkanSupported() != GLFW_TRUE) {
		this->destroy();
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Vulkan is not supported.");
		Exception::throwNotSupported();
	}

	{
		auto result = glfwSetErrorCallback(Internal::Exception::glfwErrorCallback);
		if (result == NULL) {
			this->destroy();
			GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to set callback.");
			Exception::throwFailedToCreate();
		}
	}

}

void FVK::Internal::Glfw::destroy() const noexcept {
	//https://www.glfw.org/docs/3.1/group__init.html#gaaae48c0a18607ea4a4ba951d939f0901
	glfwTerminate();
}
