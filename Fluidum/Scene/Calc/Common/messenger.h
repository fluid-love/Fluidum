#pragma once

#include "../../Common/common.h"

namespace FS::Calc::Internal {

	inline ::FU::Log::Messenger GMessenger{};

}

namespace FS::Calc::Lua::Internal {

	inline ::FU::Log::Messenger& GMessenger = ::FS::Calc::Internal::GMessenger;

}

namespace FS::Calc {

	inline void setMessengerCallback(FU::Log::MessengerCallbackType callback) {
		Internal::GMessenger.setCallback(callback);
	}

}