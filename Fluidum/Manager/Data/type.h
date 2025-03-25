#pragma once

#include "../../Utils/include.h"

//forward declaration
namespace FD::Internal {
	class ManagerPassKey;
}

namespace FD {

	//ManagerPassKey
	template<typename T>
	concept IsConstructibleFromManagerPassKey = std::constructible_from<T, const Internal::ManagerPassKey&>;

	//delete copy and move
	template<typename T>
	concept IsNotCopyMoveAble = !std::copyable<T> && !std::movable<T>;

	//!default
	template<typename T>
	concept IsNotDefaultConstructible = !std::is_default_constructible_v<T>;

	//not CVR
	template<typename T>
	concept IsNotCVR =
		!std::is_const_v<T> &&
		!std::is_volatile_v<T> &&
		!std::is_reference_v<T>;

	//class
	template<typename T>
	concept IsClass = FU::Concept::IsClass<T>;

}

namespace FD {

	template<typename T>
	concept IsDataAble =
		(std::is_pointer_v<T> && IsClass<std::remove_pointer_t<T>>) || (
			IsConstructibleFromManagerPassKey<T> &&
			IsNotCopyMoveAble<T> &&
			IsNotDefaultConstructible<T> &&
			IsNotCVR<T> &&
			IsClass<T>);

}