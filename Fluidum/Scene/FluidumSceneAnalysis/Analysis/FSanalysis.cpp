#include "FSanalysis.h"

FS::Analysis::Analysis(Manager* manager, FD::PlotRead* plot)
	:Scene(manager), plot(plot)
{
}

FS::Analysis::~Analysis() {

}

void FS::Analysis::update() {

}

void FS::Analysis::draw() {
	ImGui::Begin("Analysis");
	
	this->plotGui();
	ImGui::Separator();

	this->functionGui();
	ImGui::Separator();

	ImGui::End();
}

void FS::Analysis::plotGui() {
	ImGui::BulletText(ICON_MD_MULTILINE_CHART); ImGui::SameLine();
	ImGui::Text(text.plot);

}

void FS::Analysis::functionGui() {
	ImGui::BulletText(ICON_MD_FUNCTIONS); ImGui::SameLine();
	ImGui::Text(text.function);

}

