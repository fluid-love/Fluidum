#include "window.h"

//using GlfwSetWindowSizeLimitsCallback = void(*)(GLFWwindow* window, std::optional<int> minwidth, std::optional<int> minheight, std::optional<int> maxwidth, std::optional<int> maxheight);
//GlfwSetWindowSizeLimitsCallback glfwSetWindowSizeLimitsCallback = GlfwSetWindowSizeLimitsCallback{};
//
//void FVK::Internal::Window::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
//	//auto win = reinterpret_cast<FVK::Internal::Window*>(glfwGetWindowUserPointer(window));
//	//win->info.framebufferResized = true;
//
//	//int posX, posY;
//	//glfwGetWindowPos(win->info.window, &posX, &posY);
//
//	//const auto [l, r] = fullscreenPos();
//
//	//if (posX != l || posY != r) {
//	//	win->info.maximized = false;
//	//	return;
//	//}
//
//	//auto [w, h] = fullscreenSize();
//	//h -= 1;
//	//if (width == w && h == height)
//	//	win->info.maximized = true;
//	//else
//	//	win->info.maximized = false;
//}
//
//void FVK::Internal::Window::setPosCallback(GLFWwindow* window, int posX, int posY) {
//	//auto win = reinterpret_cast<FVK::Internal::Window*>(glfwGetWindowUserPointer(window));
//
//	//const auto [l, r] = fullscreenPos();
//
//	//if (posX != l || posY != r) {
//	//	win->info.maximized = false;
//	//	return;
//	//}
//
//	//int width, height;
//	//glfwGetWindowSize(win->info.window, &width, &height);
//	//auto [w, h] = fullscreenSize();
//	//h -= 1;
//	//if (width == w && h == height)
//	//	win->info.maximized = true;
//	//else
//	//	win->info.maximized = false;
//
//}
//
//void FVK::Internal::Window::focusedCallback(GLFWwindow* window, int focus) {
//	//auto win = reinterpret_cast<FVK::Internal::Window*>(glfwGetWindowUserPointer(window));
//
//	//if (focus == GLFW_TRUE)
//	//	win->info.focused = true;
//	//else
//	//	win->info.focused = false;
//}
//
//void FVK::Internal::Window::sizeLimitsCallback(GLFWwindow* window, std::optional<int> minwidth, std::optional<int> minheight, std::optional<int> maxwidth, std::optional<int> maxheight) {
//	//auto win = reinterpret_cast<FVK::Internal::Window*>(glfwGetWindowUserPointer(window));
//	//if (minwidth)
//	//	win->info.sizeMinLimits.first = *minwidth;
//	//if (minheight)
//	//	win->info.sizeMinLimits.second = *minheight;
//	//if (maxwidth)
//	//	win->info.sizeMaxLimits.first = *maxwidth;
//	//if (maxheight)
//	//	win->info.sizeMaxLimits.second = *maxheight;
//}

#ifdef FluidumUtils_Type_OS_Windows
LRESULT CALLBACK FVK::Internal::Window::windowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {

	if (msg == WM_SETFOCUS) {
		auto ptr = GetWindowLongPtr(hwnd, DWLP_USER);
		if (ptr == NULL)
			return DefWindowProc(hwnd, msg, wp, lp);

		Window* window = reinterpret_cast<Window*>(ptr);
		window->info.focused = true;
	}

	else if (msg == WM_KILLFOCUS) {
		auto ptr = GetWindowLongPtr(hwnd, DWLP_USER);
		if (ptr == NULL)
			return DefWindowProc(hwnd, msg, wp, lp);

		Window* window = reinterpret_cast<Window*>(ptr);
		window->info.focused = false;
	}

	return DefWindowProc(hwnd, msg, wp, lp);
}
#endif

void FVK::Internal::Window::create(const Data::WindowData& data, const NormalParameter& parameter) {
	assert(false);
}

void FVK::Internal::Window::create(const Data::WindowData& data, const FullScreenParameter& parameter) {
	assert(!this->info.window);

	const auto [x, y, width, height] = fullscreenPosSize();

#ifdef FluidumUtils_Type_OS_Windows
	HINSTANCE instance = GetModuleHandle(NULL);

	{
		WNDCLASSEX  windowClass{};

		const auto iconPath = FU::Text::utf8ToUtf16(parameter.iconFilePath);
		HANDLE icon = LoadImage(instance, iconPath.c_str(), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_DEFAULTCOLOR | LR_LOADFROMFILE);

		windowClass.cbSize = sizeof(windowClass);
		windowClass.style = CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc = windowProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = instance;
		windowClass.hIcon = static_cast<HICON>(icon);
		windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		windowClass.hbrBackground = reinterpret_cast<HBRUSH>((COLOR_BACKGROUND + 1));
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = TEXT("FluidumClass");
		windowClass.hIconSm = NULL;

		RegisterClassEx(&windowClass);
	}


	const auto titleUtf16 = FU::Text::utf8ToUtf16(parameter.title);
	info.window = CreateWindow(
		TEXT("FluidumClass"),                              //class name
		titleUtf16.c_str(),                                //tile
		WS_POPUP ^ WS_CAPTION ^ WS_BORDER ^ WS_MAXIMIZE ^ WS_THICKFRAME,							           //style
		static_cast<int>(x), static_cast<int>(y),          //pos
		static_cast<int>(width), static_cast<int>(height - 1), //size
		NULL,									           //parent
		NULL,									           //menu
		GetModuleHandle(NULL),					           //instance
		NULL
	);


	//set userdata
	SetWindowLongPtr(info.window, DWLP_USER, reinterpret_cast<LONG_PTR>(this));

#endif

	this->checkWindow();

	//this->setCallbacks();

	////limits
	//{
	//	const int minW = static_cast<int>(width * 0.2f);
	//	const int minH = static_cast<int>(height * 0.2f);
	//	glfwSetWindowSizeLimits(this->info.window, minW, minH, width, height - 1);
	//	info.sizeMinLimits = { minW, minH };
	//	info.sizeMaxLimits = { width, height - 1 };
	//}

#ifdef FluidumUtils_Type_OS_Windows
	ShowWindow(info.window, SW_SHOWNORMAL);
#endif

	info.fullscreen = true;
	info.focused = true;
}

void FVK::Internal::Window::create(const Data::WindowData& data, const MaximizedParameter& parameter) {
	assert(false);
}

void FVK::Internal::Window::checkWindow() {
	if (info.window) //noerror
		return;

	//Failed to create window.

#ifdef FluidumUtils_Type_OS_Windows
	GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__,
		"Failed to create window. ErrorCode is {}. See https://docs.microsoft.com/en-us/windows/win32/debug/system-error-codes",
		GetLastError()
		);
#endif
}

void FVK::Internal::Window::setCallbacks() noexcept {

	//glfwSetWindowUserPointer(this->info.window, this);

	//glfwSetFramebufferSizeCallback(this->info.window, framebufferResizeCallback);
	//glfwSetWindowFocusCallback(this->info.window, focusedCallback);

	//glfwSetWindowPosCallback(this->info.window, setPosCallback);

	//glfwSetWindowSizeLimitsCallback = sizeLimitsCallback;
}

const FVK::Internal::Data::WindowInfo& FVK::Internal::Window::get() const noexcept {
	assert(this->info.window);
	return this->info;
}

void FVK::Internal::Window::destroy() noexcept {
	assert(this->info.window);
#ifdef FluidumUtils_Type_OS_Windows
	const BOOL result = DestroyWindow(this->info.window);
	if (result == FALSE) {
		std::terminate();
	}
#endif
}

std::pair<FVK::IF32, FVK::IF32> FVK::Internal::Window::getPos() const {
#ifdef FluidumUtils_Type_OS_Windows
	RECT rect{};
	const BOOL result = GetWindowRect(
		info.window,
		&rect
	);
	if (result == FALSE) {
		Exception::throwUnexpected();
	}
#endif
	return { static_cast<IF32>(rect.left), static_cast<IF32>(rect.top) };
}

std::pair<FVK::IF32, FVK::IF32> FVK::Internal::Window::getSize() const {
#ifdef FluidumUtils_Type_OS_Windows
	RECT rect{};
	const BOOL result = GetWindowRect(
		info.window,
		&rect
	);
	if (result == FALSE) {
		Exception::throwUnexpected();
	}
#endif
	return { static_cast<IF32>(rect.right - rect.left), static_cast<IF32>(rect.bottom - rect.top) };
}

void FVK::Internal::Window::setPos(const IF32 x, const IF32 y) const {
#ifdef FluidumUtils_Type_OS_Windows
	const BOOL result = SetWindowPos(
		info.window,
		NULL,
		static_cast<int>(x),
		static_cast<int>(y),
		NULL,
		NULL,
		SWP_NOSIZE
	);

	if (result == FALSE) {
		Exception::throwUnexpected();
	}
#endif
}

void FVK::Internal::Window::setSize(const IF32 width, const IF32 height) const {
#ifdef FluidumUtils_Type_OS_Windows
	const BOOL result = SetWindowPos(
		info.window,
		NULL,
		NULL,
		NULL,
		width,
		height,
		SWP_NOREPOSITION
	);

	if (result == FALSE) {
		Exception::throwUnexpected();
	}
#endif
}

void FVK::Internal::Window::resize(const IF32 x, const IF32 y, const IF32 width, const IF32 height) const {
#ifdef FluidumUtils_Type_OS_Windows
	const BOOL result = SetWindowPos(
		info.window,
		NULL,
		static_cast<int>(x),
		static_cast<int>(y),
		static_cast<int>(width),
		static_cast<int>(height),
		NULL
	);

	if (result == FALSE) {
		Exception::throwUnexpected();
	}
#else
#error NotSupported
#endif 
	//setPosCallback(info.window, x, y);
	//framebufferResizeCallback(info.window, width, height);
}

void FVK::Internal::Window::minimize() const {
#ifdef FluidumUtils_Type_OS_Windows
	const BOOL result = ShowWindow(this->info.window, SW_HIDE);
	if (result == FALSE) {
		Exception::throwUnexpected();
	}
#endif
}

std::tuple<FVK::IF32, FVK::IF32, FVK::IF32, FVK::IF32> FVK::Internal::Window::fullscreenPosSize() {
#ifdef FluidumUtils_Type_OS_Windows
	RECT rect{};
	BOOL result = SystemParametersInfo(SPI_GETWORKAREA, NULL, &rect, NULL);
	if (result == FALSE) {
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to get workarea rect.");
		Exception::throwUnexpected();
	}
#endif
	return {
		static_cast<IF32>(rect.left),
		static_cast<IF32>(rect.top),
		static_cast<IF32>(rect.right - rect.left),
		static_cast<IF32>(rect.bottom - rect.top)
	};
}

std::pair<FVK::IF32, FVK::IF32> FVK::Internal::Window::fullscreenPos() {
	const auto [l, r, w, h] = fullscreenPosSize();
	return { static_cast<IF32>(l), static_cast<IF32>(r) };
}

void FVK::Internal::Window::fullscreen() const {
	//const auto [width, height] = fullscreenSize();
	//const auto [posX, posY] = fullscreenPos();
	//glfwSetWindowPos(info.window, posX, posY);
	//glfwSetWindowSize(info.window, static_cast<int>(width), static_cast<int>(height - 1));
}

//void glfwSetWindowSizeLimits_(GLFWwindow* window, int minwidth, int minheight, int maxwidth, int maxheight) {
//	glfwSetWindowSizeLimits(window, minwidth, minheight, maxwidth, maxheight);
//
//	std::optional<int> minW = (minwidth == GLFW_DONT_CARE) ? std::nullopt : std::optional<int>(minwidth);
//	std::optional<int> minH = (minheight == GLFW_DONT_CARE) ? std::nullopt : std::optional<int>(minheight);
//	std::optional<int> maxW = (maxwidth == GLFW_DONT_CARE) ? std::nullopt : std::optional<int>(maxwidth);
//	std::optional<int> maxH = (maxheight == GLFW_DONT_CARE) ? std::nullopt : std::optional<int>(maxheight);
//
//	glfwSetWindowSizeLimitsCallback(window, minW, minH, maxW, maxH);
//}

void FVK::Internal::Window::setTransparent(const I32 r, const I32 g, const I32 b, const I32 alpha) const {
	auto hwnd = info.window;
	if ((GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_LAYERED) == WS_EX_LAYERED)
		return;

	assert(r >= 0 && r <= 255);
	assert(g >= 0 && g <= 255);
	assert(b >= 0 && b <= 255);
	assert(alpha >= 0 && alpha <= 255);
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hwnd, RGB(r, g, b), alpha, LWA_COLORKEY);

}

void FVK::Internal::Window::unsetTransparent() const {
	auto hwnd = info.window;
	if ((GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_LAYERED) == WS_EX_LAYERED) {
		SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) ^ WS_EX_LAYERED);
	}
}


