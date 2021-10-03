#pragma once

#include "../../common/FSusing.h"

#include <implot/implot.h>
#include <imgui_internal.h>

namespace FS {

	class Plot final :public Scene {
	public:
		explicit Plot(Manager* manager,FD::PlotRead* plot);
		void Constructor(FD::PlotRead);

		~Plot();

	public:
		virtual void update() override;
		virtual void draw() override;

	private://data
		FD::PlotRead* plot = nullptr;

	private://data
		ImGuiID dockID = {};
		std::vector<ImGuiWindow*> plotWindows = {};

	private:
		void dockSpaceGui();
		void plotGui();

	};
}