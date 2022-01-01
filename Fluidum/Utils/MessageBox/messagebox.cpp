#include "messagebox.h"
#include "../Text/guitext.h"

#ifdef BOOST_OS_WINDOWS

long FU::MB::Internal::iconToVal(const Icon type) {
	if (type == Icon::Warning)
		return MB_ICONWARNING;
	else if (type == Icon::Error)
		return MB_ICONERROR;
	else if (type == Icon::Information)
		return MB_ICONINFORMATION;
	else
		abort();
}

int FU::MB::Internal::MsgBox3(const wchar_t* caption, UINT uType) {
	int retval;

	MsgBoxHook = SetWindowsHookEx(
		WH_CBT,
		Proc3,
		NULL,
		GetCurrentThreadId()
	);

	retval = MessageBox(NULL, caption, Title, uType);

	UnhookWindowsHookEx(MsgBoxHook);
	return retval;
}

#endif

FU::I32 FU::MB::button_button_cancel(
	const Icon iconType,
	const char* message,
	const char* button1,
	const char* button2,
	const char* cancelButton
) {

#ifdef BOOST_OS_WINDOWS
	using namespace Internal;

	//arg -> utf-8 | windows -> wchar_t
	std::wstring  wMessage = ::FU::Text::utf8ToUtf16(message);

	long icon = iconToVal(iconType);

	Button1 = Text::utf8ToUtf16(button1);
	Button2 = Text::utf8ToUtf16(button2);
	if (cancelButton)
		Button3 = Text::utf8ToUtf16(cancelButton);

	auto result = MsgBox3(wMessage.data(), icon | MB_YESNOCANCEL | MB_TASKMODAL);

	reset();

	if (result == IDYES)
		return 0;
	else if (result == IDNO)
		return 1;

	return 2;
#else
#error Not Supported
#endif
}

void FU::MB::Internal::reset() {
	Button1.clear();
	Button1.shrink_to_fit();
	Button2.clear();
	Button2.shrink_to_fit();
	Button3.clear();
	Button3.shrink_to_fit();
}

void FU::MB::error(const char* message) {
#ifdef BOOST_OS_WINDOWS
	using namespace Internal;

	//arg -> utf-8 | windows -> wchar_t
	std::wstring  wMessage = ::FU::Text::utf8ToUtf16(message);

	long icon = MB_ICONERROR;
	
	MessageBox(NULL, wMessage.data(), Title, icon | MB_OK | MB_TASKMODAL);

#else
#error Not Supported
#endif
}

void FU::MB::information(const char* message) {
#ifdef BOOST_OS_WINDOWS
	using namespace Internal;

	//arg -> utf-8 | windows -> wchar_t
	std::wstring  wMessage = ::FU::Text::utf8ToUtf16(message);

	long icon = MB_ICONINFORMATION;
	
	MessageBox(NULL, wMessage.data(), Title, icon | MB_OK | MB_TASKMODAL);

#else
#error Not Supported
#endif
}

FU::I32 FU::MB::ok_cancel(const char* message) {
#ifdef BOOST_OS_WINDOWS
	using namespace Internal;

	//arg -> utf-8 | windows -> wchar_t
	std::wstring  wMessage = ::FU::Text::utf8ToUtf16(message);

	long icon = MB_ICONWARNING;

	int result = MessageBox(NULL, wMessage.data(), Title, icon | MB_OKCANCEL | MB_TASKMODAL);

	if (result == IDOK)
		return 0;
	else if (result == IDCANCEL)
		return 1;

#else
#error Not Supported
#endif
	assert(false);
	return 1;
}

FU::I32 FU::MB::yes_no(const char* message) {
#ifdef BOOST_OS_WINDOWS
	using namespace Internal;

	//arg -> utf-8 | windows -> wchar_t
	std::wstring  wMessage = ::FU::Text::utf8ToUtf16(message);

	long icon = MB_ICONWARNING;

	int result = MessageBox(NULL, wMessage.data(), Title, icon | MB_YESNO | MB_TASKMODAL);

	if (result == IDYES)
		return 0;
	else if (result == IDNO)
		return 1;

#else
#error Not Supported
#endif
	assert(false);
	return 1;
}

FU::I32 FU::MB::child_button_button_cancel(
	const Icon iconType,
	const char* message,
	const char* childMessage,
	const char* button1,
	const char* button2,
	const char* cancelButton
) {
#ifdef BOOST_OS_WINDOWS
/*
	const GLFWvidmode* screen = glfwGetVideoMode(glfwGetPrimaryMonitor());

	auto hwnd = CreateWindow(
		TEXT("STATIC"), TEXT("Fluidum"),
		WS_SYSMENU | WS_BORDER | WS_CAPTION & WS_MINIMIZEBOX,
		screen->width /3.0f, screen->height / 3.0f, screen->width / 3.0f, screen->height / 3.0f, NULL, NULL,
		GetModuleHandle(NULL), NULL
	);
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	ShowWindow(hwnd, SW_SHOW);


	auto message = CreateWindow(
		TEXT("STATIC"), TEXT("Kitty on your lap"),
		WS_BORDER,
		10, 10, 100, 100, hwnd, NULL,
		GetModuleHandle(NULL), NULL
	);

	CreateWindowEx(
		WS_EX_LEFT, TEXT("BUTTON"), TEXT("Stand by Ready!!"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		100, 100, 100, 40,
		hwnd, NULL, GetModuleHandle(NULL), NULL
	);

	MSG msg{};
	while (TRUE) {
		GetMessage(&msg, NULL, 0, 0);
		if (msg.message == WM_LBUTTONUP) break;
		DispatchMessage(&msg);
	}

	DestroyWindow(hwnd);
*/
#else
#error Not Supported
#endif

	return 0;
}