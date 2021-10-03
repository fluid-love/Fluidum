#pragma once

#include "../../Utils/include.h"

namespace FD::Internal {
	class ManagerPassKey;
}

//次の制約をデータに課す
namespace FD {

	//ManagerPassKeyがコンストラクタの引数であること
	template<typename T>
	concept IsConstructibleFromManagerPassKey = std::constructible_from<T,const Internal::ManagerPassKey&>;

	//CopyもMoveも禁止されていること
	template<typename T>
	concept IsNotCopyMoveAble = !std::copyable<T> && !std::movable<T>;

	//default構築不可
	template<typename T>
	concept IsNotDefaultConstructible = !std::is_default_constructible_v<T>;

	//余計な修飾型でない
	template<typename T>
	concept IsNotCVR =
		!std::is_const_v<T> &&
		!std::is_volatile_v<T> &&
		!std::is_reference_v<T>;

	//クラスであること
	template<typename T>
	concept IsClass = FU::Concept::IsClass<T>;

	//ポインタを除き 全てを満たすものだけが受け入れられる
	template<typename T>
	concept IsDataAble =
		(std::is_pointer_v<T> && IsClass<std::remove_pointer_t<T>>) || (
			IsConstructibleFromManagerPassKey<T> &&
			IsNotCopyMoveAble<T> &&
			IsNotDefaultConstructible<T> &&
			IsNotCVR<T> &&
			IsClass<T>);

}