#include "messagebox.h"

long FU::MB::Internal::iconToVal(const Icon type) {
	if (type == Icon::Warning)
		return MB_ICONWARNING;
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
	std::wstring  wMessage = utf8ToUtf16(message);

	long icon = iconToVal(iconType);

	Button1 = utf8ToUtf16(button1);
	Button2 = utf8ToUtf16(button2);
	if (cancelButton)
		Button3 = utf8ToUtf16(cancelButton);

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

std::wstring FU::MB::Internal::utf8ToUtf16(const std::string& str) {
	if (str.empty())
		return std::wstring{};
	int size = MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), NULL, 0);
	std::wstring result(size, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), result.data(), size);
	return result;
}