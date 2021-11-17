#pragma once

#include "../Common/type.h"

namespace LuAssist::Utils::Table::Exception {

	struct NotTable final {};

	struct NotNumber final {
		//index: 1,2,3...
		const std::size_t index;

		const std::string input;
	};


}

namespace LuAssist::Utils::Table {

	/*
	Exception
		NotTable
		NotNumber
	*/
	std::vector<Num> convertTableToArrayOfNumbers(State L, const int32_t tableIndex);

}