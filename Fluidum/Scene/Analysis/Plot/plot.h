//#pragma once
//
//#include "../../common/FSusing.h"
//
//#include <implot/implot.h>
//#include <imgui_internal.h>
//
//namespace FS {
//
//	class Plot final :public Scene {
//	public:
//		explicit Plot(Manager* manager,FD::ImPlotRead* plot);
//		void Constructor(FD::ImPlotRead);
//
//		~Plot();
//
//	public:
//		virtual void update() override;
//		virtual void draw() override;
//
//	private://data
//		FD::ImPlotRead* plot = nullptr;
//
//	private://data
//		ImGuiID dockID = {};
//		std::vector<ImGuiWindow*> plotWindows = {};
//
//	private:
//		void dockSpaceGui();
//		void plotGui();
//
//	};
//}

#pragma once

#include "../../Common/common.h"

namespace FS::Analysis {

	class Plot final : public Scene {
	public:
		explicit Plot();

		void Constructor();

		~Plot() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Plot)

	public:
		virtual void call() override;



	};
}