#pragma once

#include "../../Common/common.h"

namespace FS::Genome {

	class Overview final : public Scene {
	public:
		explicit Overview();
		void Constructor();

		~Overview() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Overview);

	public:
		virtual void call() override;

	private://data


	private:

	
	private:

	};
}