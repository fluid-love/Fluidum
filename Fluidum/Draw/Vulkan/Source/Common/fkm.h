#pragma once

#include <FKMinclude.h>
#include "../Common/FVKtype.h"

namespace FVK::Internal {
	//Global FluidumKeyManager
	inline FKM::KeyManager<FvkType> GKeyManager{};
}

namespace FVK {

	using FKM::OrderKey;
	using FKM::IndexKey;
	using FKM::SelectKey;

}




