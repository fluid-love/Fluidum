#pragma once

#include "../../Common/common.h"

namespace FS {

	class Animation final : public Scene {
	public:
		explicit Animation();
		void Constructor();

		~Animation() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Animation);

	public:
		virtual void call() override;

	private:

	};

}