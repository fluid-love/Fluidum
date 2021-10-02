#include "FVKglfw.h"
#include "../Exception/FVKglfw.h"

FVK::Internal::Glfw::Glfw(ManagerPassKey) {
	this->init();
}

void FVK::Internal::Glfw::init() {
	auto result = glfwInit();

	if (result != GLFW_TRUE)
		Exception::throwFailedToInitialize("Failed to initialize GLFW.");

	//https://www.glfw.org/docs/3.3/group__vulkan.html#ga1abcbe61033958f22f63ef82008874b1
	if (glfwVulkanSupported() != GLFW_TRUE) {
		terminate();
		Exception::throwNotSupported("Vulkan is not available.");
	}

	glfwSetErrorCallback(Internal::Exception::glfwErrorCallback);
}

void FVK::Internal::Glfw::destroy() {
	glfwTerminate();
}
