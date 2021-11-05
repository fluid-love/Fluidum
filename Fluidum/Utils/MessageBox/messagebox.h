#pragma once

#include <boost/predef.h>
#include <GLFW/glfw3native.h>
#include "../Concept/concept.h"
#include <string>
#include <array>

#ifdef BOOST_OS_WINDOWS
#include <Windows.h>
#undef max
#undef min
#else
#error Not Supported
#endif

namespace FU::MB {
	enum class Icon : uint8_t {
		Warning,
		Error
	};
}

namespace FU::MB::Internal {
	inline HHOOK MsgBoxHook{};
	inline std::wstring Button1{};
	inline std::wstring Button2{};
	inline std::wstring Button3{};
	constexpr inline wchar_t Title[] = TEXT("Fluidum");


	inline LRESULT CALLBACK Proc3(int code, WPARAM wParam, LPARAM lParam) {
		HWND hwnd;

		if (code < 0)
			return CallNextHookEx(MsgBoxHook, code, wParam, lParam);

		switch (code)
		{
		case HCBT_ACTIVATE:

			hwnd = (HWND)wParam;

			//SetWindowText(hwnd, Title);

			SetWindowText(GetDlgItem(hwnd, IDYES), Button1.data());
			SetWindowText(GetDlgItem(hwnd, IDNO), Button2.data());
			if (!Button3.empty())
				SetWindowText(GetDlgItem(hwnd, IDCANCEL), Button3.data());

			return 0;

		}

		return CallNextHookEx(MsgBoxHook, code, wParam, lParam);
	}

	int MsgBox3(const wchar_t* caption, UINT uType);

	long iconToVal(const Icon type);


	void reset();
}

namespace FU::MB {

	int32_t button_button_cancel(
		const Icon iconType,
		const char* message,
		const char* button1,
		const char* button2,
		const char* cancelButton = nullptr
	);

	//Icon::Error message [IDOK]
	void error(const char* message);

	//Icon::Warning message [IDOK] [IDCANCEL]
	void ok_cancel(const char* message);

}