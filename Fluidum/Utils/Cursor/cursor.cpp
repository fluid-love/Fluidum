#include "cursor.h"
#include <boost/predef.h>

#ifdef BOOST_OS_WINDOWS
#include <Windows.h>
#endif

void FU::Cursor::setCursorType(const Type type) {
#ifdef BOOST_OS_WINDOWS
	HCURSOR hCursor = GetCursor();

	if (type == Type::Wait)
		hCursor = LoadCursor(NULL, IDC_WAIT);
	else if (type == Type::Arrow)
		hCursor = LoadCursor(NULL, IDC_ARROW);
	else if (type == Type::Hand)
		hCursor = LoadCursor(NULL, IDC_HAND);
	else if (type == Type::SizeWE)
		hCursor = LoadCursor(NULL, IDC_SIZEWE);
	else if (type == Type::SizeNS)
		hCursor = LoadCursor(NULL, IDC_SIZENS);
	else if (type == Type::SizeNESW)
		hCursor = LoadCursor(NULL, IDC_SIZENESW);
	else if (type == Type::SizeNWSE)
		hCursor = LoadCursor(NULL, IDC_SIZENWSE);
	else if (type == Type::SizeAll)
		hCursor = LoadCursor(NULL, IDC_SIZEALL);

	SetCursor(hCursor);
#else
#error Not Supported
#endif

}