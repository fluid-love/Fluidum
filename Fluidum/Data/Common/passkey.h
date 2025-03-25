#pragma once

#include "resource.h"

namespace FD {

	class ProjectWrite;

}

namespace FD::Internal::Project {

	class PassKey final {
	public:
		FluidumUtils_Class_Default_ConDestructor_Noexcept(PassKey);
		FluidumUtils_Class_Delete_CopyMove(PassKey);

	private:
		friend class ProjectWrite;

	};

}
