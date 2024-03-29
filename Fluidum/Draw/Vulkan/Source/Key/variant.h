#pragma once

#include "key.h"

namespace FVK::Internal::Key {

	//Parameterに使う
	template<template<typename>typename T>
	using VariantKey = std::variant<T<CharKeyType>, T<StringKeyType>, T<IndexKeyType>, T<SelectKeyType>>;

	//vector<variant>ではなくvariant<vector>
	template<template<typename>typename T>
	using VariantVectorKey =
		std::variant<
		std::vector<T<CharKeyType>>,
		std::vector<T<StringKeyType>>,
		std::vector<T<IndexKeyType>>,
		std::vector<T<SelectKeyType>>
		>;

	template<typename T>//もうちょい考える
	concept IsVariantKey = FU::Concept::IsInstanceOf<T, std::variant>;

}

