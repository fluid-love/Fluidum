#include "window.h"

void FVK::Internal::Window::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	auto app = reinterpret_cast<FVK::Internal::Window*>(glfwGetWindowUserPointer(window));
	app->info.framebufferResized = true;
}

void FVK::Internal::Window::create(const Data::WindowData& data, const NormalParameter& parameter) {
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	const GLFWvidmode* mode = this->getVideoMode();

	this->info.window = glfwCreateWindow(mode->width, mode->height, parameter.title, nullptr, nullptr);

	this->checkGlfwCreateWindow();

	//set pos
	glfwSetWindowPos(this->info.window, parameter.posX, parameter.posY);

	try {
		this->setResizedCallback();
	}
	catch (const ::FVK::Exception::Error&) {
		this->destroy();
		Exception::throwFailedToCreate();
	}
}

void FVK::Internal::Window::create(const Data::WindowData& data, const FullScreenParameter& parameter) {
	assert(!this->info.window);

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

	const GLFWvidmode* mode = this->getVideoMode();

	this->info.window = glfwCreateWindow(mode->width, mode->height, parameter.title, nullptr, nullptr);

	checkGlfwCreateWindow();

	try {
		this->setResizedCallback();
	}
	catch (const ::FVK::Exception::Error&) {
		this->destroy();
		Exception::throwFailedToCreate();
	}
}

void FVK::Internal::Window::create(const Data::WindowData& data, const MaximizedParameter& parameter) {
	assert(!this->info.window);

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

	const GLFWvidmode* mode = this->getVideoMode();

	this->info.window = glfwCreateWindow(mode->width, mode->height, parameter.title, nullptr, nullptr);
	checkGlfwCreateWindow();

	try {
		this->setResizedCallback();
	}
	catch (const ::FVK::Exception::Error&) {
		this->destroy();
		Exception::throwFailedToCreate();
	}
}

void FVK::Internal::Window::checkGlfwCreateWindow() const {
	if (this->info.window)
		return;

	//error

	std::string message{};
	try {
		message = Exception::getGlfwErrorMessage();
	}
	catch (const std::exception&) {
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to create window.");
		Exception::throwFailedToCreate();
	}

	GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to create window.");
	Exception::throwFailedToCreate();
}

void FVK::Internal::Window::setResizedCallback() {
	glfwSetWindowUserPointer(this->info.window, this);

	auto result = glfwSetFramebufferSizeCallback(this->info.window, framebufferResizeCallback);
	if (result == NULL) {
		glfwSetWindowUserPointer(NULL, NULL);
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to set callback.");
		Exception::throwFailedToCreate();
	}

}

const FVK::Internal::Data::WindowInfo& FVK::Internal::Window::get() const noexcept {
	assert(this->info.window);
	return this->info;
}

void FVK::Internal::Window::destroy() noexcept {
	assert(this->info.window);
	glfwDestroyWindow(this->info.window);
}

const GLFWvidmode* FVK::Internal::Window::getVideoMode() const {

	//https://www.glfw.org/docs/3.3/group__monitor.html#gac3adb24947eb709e1874028272e5dfc5
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	if (monitor == NULL) {
		auto lock = GMessenger.getLock();
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "There is no monitor.");
		GMessenger.add<FU::Log::Type::Info>(__FILE__, __LINE__, "Make sure that the monitor is properly connected.");
		Exception::throwFailedToCreate();
	}

	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	if (mode == NULL) { //error
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "A platform-specific error occurred.");
		Exception::throwFailedToCreate();
	}

	return mode;

}

std::pair<FVK::I32, FVK::I32> FVK::Internal::Window::windowSize() const noexcept {
	int width{}, height{};
	glfwGetWindowSize(this->info.window, &width, &height);
	assert(width > 0 && height > 0);
	return { static_cast<I32>(width), static_cast<I32>(height) };
}