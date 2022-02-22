#pragma once

#include "../../Common/common.h"

namespace FS::Genome {

	class Generation final : public Scene {
	public:
		explicit Generation();
		void Constructor();

		~Generation() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Generation);

	public:
		virtual void call() override;

	private:

	private:
		void table();


	};
}