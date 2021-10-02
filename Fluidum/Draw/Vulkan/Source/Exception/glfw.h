#pragma once

#include <iostream>

namespace FVK::Internal::Exception {

	void glfwErrorCallback(int code, const char* message);

	std::string getGlfwErrorMessage() noexcept;
}