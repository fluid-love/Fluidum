#pragma once

#include "../Type/type.h"

namespace FU::Cursor {

	enum class Type : UIF8 {
		Wait,
		Arrow,
		Hand,
		SizeWE,
		SizeNS,
		SizeNESW,
		SizeNWSE,
		SizeAll,
	};

	void setCursorType(const Type type);

}