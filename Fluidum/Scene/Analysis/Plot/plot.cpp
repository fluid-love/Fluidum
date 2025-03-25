#include "plot.h"
#include "imgui_internal.h"

namespace FS::Analysis {
	//imgui_internal.h
	std::vector<ImGuiWindow*> GWindows{};
}

FS::Analysis::Plot::Plot(
	const FD::ImPlotRead_Lock* const implotRead,
	FD::ImPlotWrite* const implotWrite
) :
	implotRead(implotRead),
	implotWrite(implotWrite)
{
	FluidumScene_Log_Constructor(::FS::Analysis::Plot);

	//ImPlot
	ImPlot::CreateContext();
	ImPlot::StyleColorsDark();
}

FS::Analysis::Plot::~Plot() noexcept {
	FluidumScene_Log_Destructor(::FS::Analysis::Plot);

	ImPlot::DestroyContext();
}

void FS::Analysis::Plot::call() {
	//ImPlot::ShowDemoWindow();

	static float xs1[11], ys1[11];
	for (int i = 0; i < 11; ++i) {
		xs1[i] = i * 0.001f;
		ys1[i] = 0.5f + 0.5f * sinf(50 * (xs1[i] + (float)ImGui::GetTime() / 10));
	}
	static double xs2[11], ys2[11];
	for (int i = 0; i < 11; ++i) {
		xs2[i] = i * 0.1f;
		ys2[i] = xs2[i] * xs2[i];
	}

	ImGui::Begin("Genome");

	if (ImPlot::BeginPlot("Fitness")) {
		ImPlot::SetupAxes("generation", "fitness");
		ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
		ImPlot::PlotLine("chromesome-1", xs1, ys1, 11);
		ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
		ImPlot::PlotLine("chromesome-2", xs2, ys2, 11);
		ImPlot::EndPlot();
	}

	ImGui::End();

	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());

	////this->dockSpace();
	////this->plot();

	//ImGui::PopStyleVar();
}


void FS::Analysis::Plot::dockSpace() {
	ImGui::Begin("Plots", nullptr, ImGuiWindowFlags_NoSavedSettings);

	this->dockSpaceID = ImGui::GetID("Plots");

	const auto size = ImGui::GetWindowSize();
	ImGui::DockSpace(this->dockSpaceID, { size.x,size.y * 0.8f });

	//dock plot windows
	for (auto window : GWindows) {
		if (window) {
			ImGui::SetWindowDock(window, this->dockSpaceID, ImGuiCond_Always);
		}
	}

	ImGui::End();
}

void FS::Analysis::Plot::plot() {
	const auto lock = implotRead->getLock();

	const auto* data = implotRead->get();

	GWindows.resize(data->size());

//for (FD::Plot::FigureIndex i = 0; const auto & x : *data) {
//	ImGui::Begin(x.figure.title.c_str(), nullptr, ImGuiWindowFlags_NoSavedSettings);
//
//	if (ImPlot::BeginPlot(x.figure.title.c_str(), x.figure.axisLabelX.c_str(), x.figure.axisLabelY.c_str(), ImVec2(-1, -1), ImPlotFlags_NoMousePos, NULL)) {
//
//		for (const auto& y : x.plots) {
//			ImPlot::SetNextMarkerStyle(static_cast<ImPlotMarker>(y.marker));
//			if (y.imPlotType == FD::Plot::ImPlotType::Line)
//				ImPlot::PlotLine(x.figure.title.c_str(), y.values1.data(), y.values2.data(), static_cast<int>(y.values1.size()));
//			//	else if (data.type == ImPlotType::BAR)
//			//		ImPlot::PlotBars(x.figure.name.c_str(), data.values2.data(), 2);
//		}
//		ImPlot::EndPlot();
//	}
//
//	//for docking
//	GWindows[i++] = ImGui::GetCurrentWindow();
//
//	ImGui::End();
//}
}































