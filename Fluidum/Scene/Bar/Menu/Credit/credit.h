#pragma once

#include "../../../Common/common.h"

namespace FS::Bar {

	class Credit final : public Scene {
	public:
		explicit Credit();
		void Constructor();

		~Credit() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Credit)

	public:
		virtual void call() override;

	};

}