#pragma once

#include "../../../Common/common.h"

namespace FS::Lua {
	enum class LogType : uint8_t {
		NumOfArgs_Min,
		NumOfArgs_Max,
		Type,

		Plot_Figure_Max,
		Plot_MarkerStr,
		Plot_PlotIndex_Min,
		Plot_PlotIndex_Max
	};

	using Message = FU::File::Text<::FS::Resource::LuaMessagePath,LogType>;

}
