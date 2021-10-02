#include "FVKwindow.h"

void FVK::Internal::Window::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	auto app = reinterpret_cast<FVK::Internal::Window*>(glfwGetWindowUserPointer(window));
	app->info.framebufferResized = true;
}

void FVK::Internal::Window::create(const Data::WindowData& data, const NormalParameter& parameter) {
	//二重は禁止
	assert(!this->info.window);

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	//window作成
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	this->info.window = glfwCreateWindow(mode->width, mode->height, parameter.title, nullptr, nullptr);
	checkGlfwCreateWindow();

	//windowのポジション
	glfwSetWindowPos(this->info.window, parameter.posX, parameter.posY);

	glfwSetWindowUserPointer(this->info.window, this);
	glfwSetFramebufferSizeCallback(this->info.window, framebufferResizeCallback);

	setGlfw();
}

void FVK::Internal::Window::create(const Data::WindowData& data, const FullScreenParameter& parameter) {
	//二重は禁止
	assert(!this->info.window);

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	
	//window作成
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	this->info.window = glfwCreateWindow(mode->width, mode->height, parameter.title, nullptr, nullptr);
	checkGlfwCreateWindow();
	setGlfw();
}

void FVK::Internal::Window::create(const Data::WindowData& data, const MaximizedParameter& parameter) {
	assert(!this->info.window);

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

	//window作成
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	this->info.window = glfwCreateWindow(mode->width, mode->height, parameter.title, nullptr, nullptr);
	checkGlfwCreateWindow();

	glfwSetWindowUserPointer(this->info.window, this);
	glfwSetFramebufferSizeCallback(this->info.window, framebufferResizeCallback);

	setGlfw();
}

void FVK::Internal::Window::checkGlfwCreateWindow() const {
	if (this->info.window)
		return;

	//windowが作成できていない場合
	std::string message = Exception::getGlfwErrorMessage();
	Exception::throwFailedToCreate(message.c_str());
}

void FVK::Internal::Window::setGlfw() {
	glfwSetWindowUserPointer(this->info.window, this);
	glfwSetFramebufferSizeCallback(this->info.window, framebufferResizeCallback);
}

const FVK::Internal::Data::WindowInfo& FVK::Internal::Window::get() const noexcept {
	assert(this->info.window);
	return this->info;
}

void FVK::Internal::Window::destroy() {
	assert(this->info.window);
	glfwDestroyWindow(this->info.window);
}

std::pair<int32_t, int32_t> FVK::Internal::Window::getWindowSize() const {
	static_assert(std::same_as<int32_t, int>);
	int width, height;
	glfwGetWindowSize(this->info.window, &width, &height);
	assert(width > 0 && height > 0);
	vk::Extent2D a;
	return { width,height };
}