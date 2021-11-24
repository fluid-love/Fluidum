#pragma once

#include "../../Common/common.h"
#include <LuAssist/type.h>

namespace FD::Calc::Array {

	using Index = uint32_t;
	using Key = uint32_t;

	enum class ValueType : uint8_t {
		Number,
		String,
		Bit
	};

	template<ValueType T>
	using ElmType =
		std::conditional_t<T == ValueType::Number, LuAssist::Num,
		std::conditional_t<T == ValueType::String, std::string,
		std::conditional_t<T == ValueType::Bit, bool,
		void
		>>>;

}