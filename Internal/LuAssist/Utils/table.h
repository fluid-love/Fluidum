#pragma once

#include "../Common/type.h"

namespace LuAssist::Utils::Table::Exception {

	struct NotTable final {};

	struct NotNumber final {
		//index: 1,2,3...
		const std::size_t index;

		const std::string input;
	};

	struct NotString final {
		//index: 1,2,3...
		const std::size_t index;

		const std::string input;
	};

	struct NotBoolean final {
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

	/*
	Exception
		NotTable
		NotString
	*/
	std::vector<std::string> convertTableToArrayOfStrings(State L, const int32_t tableIndex);

	/*
	Exception
		NotTable
		NotBoolean
	*/
	std::vector<bool> convertTableToArrayOfBooleans(State L, const int32_t tableIndex);


}