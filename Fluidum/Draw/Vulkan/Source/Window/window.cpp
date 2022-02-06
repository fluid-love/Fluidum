#include "window.h"

#ifdef BOOST_OS_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
#include <GLFW/glfw3native.h>

using GlfwSetWindowSizeLimitsCallback = void(*)(GLFWwindow* window, std::optional<int> minwidth, std::optional<int> minheight, std::optional<int> maxwidth, std::optional<int> maxheight);
GlfwSetWindowSizeLimitsCallback glfwSetWindowSizeLimitsCallback = GlfwSetWindowSizeLimitsCallback{};

void FVK::Internal::Window::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	auto win = reinterpret_cast<FVK::Internal::Window*>(glfwGetWindowUserPointer(window));
	win->info.framebufferResized = true;

	int posX, posY;
	glfwGetWindowPos(win->info.window, &posX, &posY);

	const auto [l, r] = fullscreenPos();

	if (posX != l || posY != r) {
		win->info.maximized = false;
		return;
	}

	auto [w, h] = fullscreenSize();
	h -= 1;
	if (width == w && h == height)
		win->info.maximized = true;
	else
		win->info.maximized = false;
}

void FVK::Internal::Window::setPosCallback(GLFWwindow* window, int posX, int posY) {
	auto win = reinterpret_cast<FVK::Internal::Window*>(glfwGetWindowUserPointer(window));

	const auto [l, r] = fullscreenPos();

	if (posX != l || posY != r) {
		win->info.maximized = false;
		return;
	}

	int width, height;
	glfwGetWindowSize(win->info.window, &width, &height);
	auto [w, h] = fullscreenSize();
	h -= 1;
	if (width == w && h == height)
		win->info.maximized = true;
	else
		win->info.maximized = false;

}

void FVK::Internal::Window::focusedCallback(GLFWwindow* window, int focus) {
	auto win = reinterpret_cast<FVK::Internal::Window*>(glfwGetWindowUserPointer(window));

	if (focus == GLFW_TRUE)
		win->info.focused = true;
	else
		win->info.focused = false;
}

void FVK::Internal::Window::sizeLimitsCallback(GLFWwindow* window, std::optional<int> minwidth, std::optional<int> minheight, std::optional<int> maxwidth, std::optional<int> maxheight) {
	auto win = reinterpret_cast<FVK::Internal::Window*>(glfwGetWindowUserPointer(window));
	if (minwidth)
		win->info.sizeMinLimits.first = *minwidth;
	if (minheight)
		win->info.sizeMinLimits.second = *minheight;
	if (maxwidth)
		win->info.sizeMaxLimits.first = *maxwidth;
	if (maxheight)
		win->info.sizeMaxLimits.second = *maxheight;
}

void FVK::Internal::Window::create(const Data::WindowData& data, const NormalParameter& parameter) {
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	const GLFWvidmode* mode = this->getVideoMode();

	this->info.window = glfwCreateWindow(mode->width, mode->height, parameter.title, nullptr, nullptr);

	this->checkGlfwCreateWindow();

	//set pos
	glfwSetWindowPos(this->info.window, parameter.posX, parameter.posY);

	this->setCallbacks();
}

void FVK::Internal::Window::create(const Data::WindowData& data, const FullScreenParameter& parameter) {
	assert(!this->info.window);

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);


	const auto [width, height] = fullscreenSize();
	this->info.window = glfwCreateWindow(width, height - 1, parameter.title, nullptr, nullptr);
	checkGlfwCreateWindow();

	this->setCallbacks();

	//limits
	{
		const int minW = static_cast<int>(width * 0.2f);
		const int minH = static_cast<int>(height * 0.2f);
		glfwSetWindowSizeLimits(this->info.window, minW, minH, width, height - 1);
		info.sizeMinLimits = { minW, minH };
		info.sizeMaxLimits = { width, height - 1 };
	}


	info.maximized = true;
	info.focused = true;
}

void FVK::Internal::Window::create(const Data::WindowData& data, const MaximizedParameter& parameter) {
	assert(!this->info.window);

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

	const GLFWvidmode* mode = this->getVideoMode();

	this->info.window = glfwCreateWindow(mode->width, mode->height, parameter.title, nullptr, nullptr);
	checkGlfwCreateWindow();

	this->setCallbacks();

	info.maximized = true;
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

void FVK::Internal::Window::setCallbacks() noexcept {

	glfwSetWindowUserPointer(this->info.window, this);

	glfwSetFramebufferSizeCallback(this->info.window, framebufferResizeCallback);
	glfwSetWindowFocusCallback(this->info.window, focusedCallback);

	glfwSetWindowPosCallback(this->info.window, setPosCallback);

	glfwSetWindowSizeLimitsCallback = sizeLimitsCallback;
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

void FVK::Internal::Window::resizeWindow(const IF32 x, const IF32 y, const IF32 width, const IF32 height) const noexcept {
#ifdef BOOST_OS_WINDOWS
	HWND hwnd = glfwGetWin32Window(this->info.window);

	MoveWindow(
		hwnd,
		static_cast<int>(x),
		static_cast<int>(y),
		static_cast<int>(width),
		static_cast<int>(height),
		TRUE
	);
#else
#error NotSupported
#endif 
	setPosCallback(info.window, x, y);
	framebufferResizeCallback(info.window, width, height);
}

std::pair<FVK::IF32, FVK::IF32> FVK::Internal::Window::fullscreenSize() {
	int l, r, w, h;
	glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &l, &r, &w, &h);
	return { static_cast<IF32>(w), static_cast<IF32>(h) };
}

std::pair<FVK::IF32, FVK::IF32> FVK::Internal::Window::fullscreenPos() {
	int l, r, w, h;
	glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &l, &r, &w, &h);
	return { static_cast<IF32>(l), static_cast<IF32>(r) };
}

void FVK::Internal::Window::fullscreen() const {
	const auto [width, height] = fullscreenSize();
	const auto [posX, posY] = fullscreenPos();
	glfwSetWindowPos(info.window, posX, posY);
	glfwSetWindowSize(info.window, static_cast<int>(width), static_cast<int>(height - 1));
}


void glfwSetWindowSizeLimits_(GLFWwindow* window, int minwidth, int minheight, int maxwidth, int maxheight) {
	glfwSetWindowSizeLimits(window, minwidth, minheight, maxwidth, maxheight);

	std::optional<int> minW = (minwidth == GLFW_DONT_CARE) ? std::nullopt : std::optional<int>(minwidth);
	std::optional<int> minH = (minheight == GLFW_DONT_CARE) ? std::nullopt : std::optional<int>(minheight);
	std::optional<int> maxW = (maxwidth == GLFW_DONT_CARE) ? std::nullopt : std::optional<int>(maxwidth);
	std::optional<int> maxH = (maxheight == GLFW_DONT_CARE) ? std::nullopt : std::optional<int>(maxheight);

	glfwSetWindowSizeLimitsCallback(window, minW, minH, maxW, maxH);
}
