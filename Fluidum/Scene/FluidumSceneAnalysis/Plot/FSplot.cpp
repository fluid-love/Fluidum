#include "FSplot.h"

FS::Plot::Plot(Manager* manager, FD::PlotRead* plot)
	:Scene(manager), plot(plot)
{
	//ImPlot作成
	ImPlot::CreateContext();

	//theme
	ImPlot::StyleColorsDark();
}

FS::Plot::~Plot() {
	//ImPlot削除
	ImPlot::DestroyContext();
}

void FS::Plot::update() {

}

void FS::Plot::draw() {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());

	this->dockSpaceGui();
	this->plotGui();

	ImGui::PopStyleVar();
}

void FS::Plot::dockSpaceGui() {
	ImGui::Begin("Plot", nullptr, ImGuiWindowFlags_NoSavedSettings);
	this->dockID = ImGui::GetID("Plot");

	//dockのエリア
	ImGui::DockSpace(dockID, ImVec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y * 0.8f), ImGuiDockNodeFlags_PassthruCentralNode);

	//dock space に強制的に結びつける
	for (auto window : plotWindows) {
		if (window) {
			ImGui::SetWindowDock(window, dockID, ImGuiCond_Always);
		}
	}

	ImGui::End();
}

void FS::Plot::plotGui() {
	plot->lock();
	const auto data = plot->get();

	//Figureの数に変更があれば変更
	plotWindows.resize(data->size());

	for (FD::Plot::FigureIndex i = 0; const auto & x : *data) {
		ImGui::Begin(x.figure.title.c_str(), nullptr, ImGuiWindowFlags_NoSavedSettings);

		if (ImPlot::BeginPlot(x.figure.title.c_str(), x.figure.axisLabelX.c_str(), x.figure.axisLabelY.c_str(), ImVec2(-1, -1), ImPlotFlags_NoMousePos, NULL)) {

			for (const auto& y : x.data) {
				ImPlot::SetNextMarkerStyle(static_cast<ImPlotMarker>(y.marker));
				if (y.type == FD::Plot::PlotType::LINE)
					ImPlot::PlotLine(x.figure.title.c_str(), y.valuesX.data(), y.valuesY.data(), static_cast<int>(y.valuesX.size()));
				//	else if (data.type == PlotType::BAR)
				//		ImPlot::PlotBars(x.figure.name.c_str(), data.valuesY.data(), 2);
			}
			ImPlot::EndPlot();
		}

		//ドッキング用にwindowを保存
		plotWindows[i] = ImGui::GetCurrentWindow();
		i++;

		ImGui::End();
	}


	plot->unlock();
}

