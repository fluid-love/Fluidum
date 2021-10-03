#include "glfw.h"

std::string glfwErrorMessage{};

void FVK::Internal::Exception::glfwErrorCallback(int code, const char* message) {

#ifndef NDEBUG
	std::cerr << "GLFW ERROR: ErrorCode: " << code << " ErrorMessage: " << message << "." << std::endl;
#endif

	glfwErrorMessage = message;
}

std::string FVK::Internal::Exception::getGlfwErrorMessage() noexcept {
	return glfwErrorMessage;
}
