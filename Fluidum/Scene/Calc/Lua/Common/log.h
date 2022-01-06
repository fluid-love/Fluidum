#pragma once

#include "../../../Common/common.h"

namespace FS::Calc::Lua::Internal {

	enum class LogType : UIF16 {
		Initialize,
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

		OutOfRange

	};

	using Message = FU::File::Text<::FS::Resource::LuaMessagePath, LogType>;

}
