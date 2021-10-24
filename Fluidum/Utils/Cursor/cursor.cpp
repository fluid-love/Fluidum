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
	if (type == Type::Arrow)
		hCursor = LoadCursor(NULL, IDC_ARROW);

	SetCursor(hCursor);
#else
#error Not Supported
#endif

}