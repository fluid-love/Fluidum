#pragma once

#include "../Manager/manager.h"

namespace FVK::Internal::API {

	//if (GMnaager.operator bool() == false) throw NotCreated
	void checkManagerEmpty();


}