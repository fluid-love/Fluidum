#pragma once

#include "../../Common/common.h"

namespace FS::Coding {

	class Debug final : public Scene {
	public:
		explicit Debug();
		void Constructor();

		~Debug() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Debug);

	public:
		virtual void call() override;

	private:



	};

}