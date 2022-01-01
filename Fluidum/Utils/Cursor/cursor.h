#pragma once

#include "../Type/type.h"

namespace FU::Cursor {

	enum class Type : UIF8 {
		Wait,
		Arrow
	};

	void setCursorType(const Type type);

}