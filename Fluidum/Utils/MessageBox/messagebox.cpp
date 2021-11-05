#include "messagebox.h"
#include "../Text/guitext.h"

#ifdef BOOST_OS_WINDOWS

long FU::MB::Internal::iconToVal(const Icon type) {
	if (type == Icon::Warning)
		return MB_ICONWARNING;
	else if (type == Icon::Error)
		return MB_ICONERROR;
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

int32_t FU::MB::button_button_cancel(
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

void FU::MB::ok_cancel(const char* message) {
#ifdef BOOST_OS_WINDOWS
	using namespace Internal;

	//arg -> utf-8 | windows -> wchar_t
	std::wstring  wMessage = ::FU::Text::utf8ToUtf16(message);

	long icon = MB_ICONWARNING;

	MessageBox(NULL, wMessage.data(), Title, icon | MB_OKCANCEL | MB_TASKMODAL);

#else
#error Not Supported
#endif

}