#pragma once

#include "../../../libraries/lithash/lithash.h"

namespace Fluidum::Utils::Class {

	class ClassCode final {
	public:
		using CodeType = uint32_t;//hash

		template<typename T>
		_NODISCARD static consteval CodeType GetClassCode() requires std::is_class_v<T> {
			return createHash(__FUNCSIG__);
		}
	};

}