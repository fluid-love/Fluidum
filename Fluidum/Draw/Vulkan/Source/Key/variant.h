#pragma once

#include "key.h"

namespace FVK::Internal::Key {

	//Parameter‚Ég‚¤
	template<template<typename>typename T>
	using VariantKey = std::variant<T<CharKeyType>, T<StringKeyType>, T<IndexKeyType>, T<SelectKeyType>>;

	//vector<variant>‚Å‚Í‚È‚­variant<vector>
	template<template<typename>typename T>
	using VariantVectorKey =
		std::variant<
		std::vector<T<CharKeyType>>,
		std::vector<T<StringKeyType>>,
		std::vector<T<IndexKeyType>>,
		std::vector<T<SelectKeyType>>
		>;

	template<typename T>//‚à‚¤‚¿‚å‚¢l‚¦‚é
	concept IsVariantKey = FU::Concept::IsInstanceOf<T, std::variant>;

}

