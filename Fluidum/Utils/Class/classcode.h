#pragma once

#include "../../../External/lithash/lithash.h"
#include "../Type/type.h"

namespace FU::Class {

	class ClassCode final {
	public:
		using CodeType = UIF32;//hash

		template<typename T>
		[[nodiscard]] static consteval CodeType GetClassCode() requires std::is_class_v<T> {
			return createHash(__FUNCSIG__);
		}
	};

}