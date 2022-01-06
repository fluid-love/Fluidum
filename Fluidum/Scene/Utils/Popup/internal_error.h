#pragma once

#include "../../Common/common.h"

namespace FS::Utils {

	class InternalError final : public Scene {
	public:
		explicit InternalError();
		void Constructor();

		~InternalError() noexcept;

		FluidumUtils_Class_Delete_CopyMove(InternalError)

	public:
		virtual void call() override;

	};

}