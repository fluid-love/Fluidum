#pragma once

#include <iostream>
#include <algorithm>
#include <vector>

#include "../Concept/concept.h"

namespace FU::Container {

	//sort ascending order -> erase duplicate elements
	template<Concept::IsStdVector T>
	constexpr void sortAscendingOrder_EraseDuplicateElms(T &con) {
		std::sort(con.begin(), con.end());
		con.erase(std::unique(con.begin(), con.end()), con.end());
	}

}
