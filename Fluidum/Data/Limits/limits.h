#pragma	once

#include "../Common/common.h"

namespace FD::Project::Limits {

	struct UserFiles final {
		FluidumUtils_Class_Delete_ConDestructor(UserFiles)
	public:
		//num of directories(virtual folder)
		static constexpr uint16_t DirectorySizeMax = 500;
	};

	struct ProjectFiles final {
		FluidumUtils_Class_Delete_ConDestructor(ProjectFiles)
	public:
		//num of directories
		static constexpr uint16_t DirectorySizeMax = 500;
	};


}