#pragma once

#include "../Common/common.h"

namespace FD {

	//���݂̃v���W�F�N�g
	struct SelectProject final {
	public:
		explicit SelectProject(Internal::PassKey) noexcept {};
		~SelectProject() = default;
		FluidumUtils_Class_Delete_CopyMove(SelectProject)

#ifdef FluidumUtils_DebugMode
			SelectProject() = default;
#endif



	};

}