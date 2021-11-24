#pragma once

#include "../../../Common/common.h"

namespace FS::Lua {
	enum class LogType : uint16_t {
		NumOfArgs_Min,
		NumOfArgs_Max,
		Type,
		TableType,
		MaxSize,
		NotMatch,

		Plot_Figure_Max,
		Plot_Plot_Max,
		Plot_MarkerStr,
		Plot_SameSize,


	};

	using Message = FU::File::Text<::FS::Resource::LuaMessagePath,LogType>;

}
