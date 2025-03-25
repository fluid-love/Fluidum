#pragma once

#include "../../../../KeyManager/include.h"
#include "../Common/type.h"

namespace FVK::Internal {
	//Global FluidumKeyManager
	inline FKM::KeyManager<FvkType> GKeyManager{};
}

namespace FVK {

	using FKM::OrderKey;
	using FKM::IndexKey;
	using FKM::SelectKey;
	using FKM::CharKey;
	using FKM::StrKey;

}




