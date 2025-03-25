#pragma once

#include "../../Common/common.h"

namespace FS::Analysis {

	class Plot final : public Scene {
	public:
		explicit Plot(
			const FD::ImPlotRead_Lock* const implotRead,
			FD::ImPlotWrite* const implotWrite
		);
		void Constructor(
			FD::ImPlotRead_Lock,
			FD::ImPlotWrite
		);

		~Plot() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Plot);

	public:
		virtual void call() override;

	private:
		const FD::ImPlotRead_Lock* const implotRead;
		FD::ImPlotWrite* const implotWrite;

	private:
		ImGuiID dockSpaceID{};

	private:
		void dockSpace();
		void empty();
		void plot();

	};

}