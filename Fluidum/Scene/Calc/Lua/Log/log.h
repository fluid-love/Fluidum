#pragma once

#include "../../../Common/common.h"

namespace FS::Lua {
	enum class LogType : uint8_t {
		NumOfArgs_Min,
		NumOfArgs_Max,
		Type,

		Plot_Figure_Max,
		Plot_Plot_Max,
		Plot_MarkerStr,
	};

	using Message = FU::File::Text<::FS::Resource::LuaMessagePath,LogType>;

}
