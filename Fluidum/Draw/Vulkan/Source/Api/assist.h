#pragma once

#include "../Manager/FVKmanager.h"

namespace FVK::Internal::API {

	//GMnaagerが作成されていない場合は例外を投げるNotCreated
	void checkManagerEmpty();


}