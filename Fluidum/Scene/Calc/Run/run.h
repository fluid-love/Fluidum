#pragma once

#include "../../Common/common.h"

namespace FS::Calc {

	class Run final : public Scene {
	public:
		explicit Run(
			const FD::FluidumFilesRead* const fluidumFilesRead
		);
		void Constructor(
			FD::FluidumFilesRead
		);

		~Run() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Run)

	public:
		virtual void call() override;

	private:
		const FD::FluidumFilesRead* const fluidumFilesRead;

	private:
		void deleteThis();
	};
}