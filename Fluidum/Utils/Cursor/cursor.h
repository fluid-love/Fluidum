#pragma once

#include <stdint.h>

namespace FU::Cursor {

	enum class Type : uint8_t {
		Wait,
		Arrow
	};

	void setCursorType(const Type type);

}