#pragma once

#include "../type.h"

namespace FS::Lua::Table::Exception {
	
	enum class Type : uint8_t {
		NotTable
	};

	struct Info final {
		Info(const Type type) : type(type) {}

		const Type type;
		
	};

}

namespace FS::Lua::Table {

	/*
	Exception
		table elemnts -> !(a number(include integer) or a string convertible to a number)
		== lua_isnumber -> false 

		lua_istable(L, tableIndex) != true
	*/
	std::vector<Val> getArrayOfNumbers(State L,const int32_t tableIndex);

}