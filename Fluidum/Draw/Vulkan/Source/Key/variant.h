#pragma once

#include "key.h"

namespace FVK::Internal::Key {

	//use in parameter
	template<template<typename>typename T>
	using VariantKey = std::variant<T<CharKey>, T<StrKey>, T<IndexKey>, T<SelectKey>>;

	//variant<vector>
	template<template<typename>typename T>
	using VariantVectorKey =
		std::variant<
		std::vector<T<CharKey>>,
		std::vector<T<StrKey>>,
		std::vector<T<IndexKey>>,
		std::vector<T<SelectKey>>
		>;

	template<typename T>//Ç‡Ç§ÇøÇÂÇ¢çlÇ¶ÇÈ
	concept IsVariantKey = FU::Concept::IsInstanceOf<T, std::variant>;

}

