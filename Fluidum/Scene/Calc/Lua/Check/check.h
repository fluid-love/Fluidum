#pragma once

#include "../type.h"
#include "../Utils/utils.h"
#include "../Log/log.h"

namespace FS::Lua::Internal {

	void checkNumOfArgs(State L, const std::size_t correctSize, const std::size_t min, const std::size_t max);

}