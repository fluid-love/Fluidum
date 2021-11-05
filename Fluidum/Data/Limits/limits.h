#pragma	once

#include "../Common/common.h"

namespace FD::Project::Limits {

	struct UserFiles final {
		FluidumUtils_Class_Delete_ConDestructor(UserFiles)
	public:
		static constexpr uint16_t DirectorySizeMax = 500;
	};

}