#include "info.h"
#include <imgui_internal.h>

FS::Bar::ProjectInfo::ProjectInfo(
	FD::ImGuiWindowWrite* const imguiWindowWrite,
	const FD::Style::ColorRead* const colorRead,
	FD::ProjectWrite* const projectWrite,
	const FD::ProjectRead* const projectRead,
	FD::Project::PropertyWrite* const propertyWrite,
	const FD::Project::PropertyRead* const propertyRead,
	FD::Project::PropertyLuaWrite* const propertyLuaWrite,
	const FD::Project::PropertyLuaRead* const propertyLuaRead,
	FD::ToolBarWrite* const toolBarWrite,
	const FD::Style::VarRead* const varRead
) :
	imguiWindowWrite(imguiWindowWrite),
	colorRead(colorRead),
	projectWrite(projectWrite),
	projectRead(projectRead),
	propertyWrite(propertyWrite),
	propertyRead(propertyRead),
	propertyLuaWrite(propertyLuaWrite),
	propertyLuaRead(propertyLuaRead),
	toolBarWrite(toolBarWrite),
	varRead(varRead)
{
	FluidumScene_Log_Constructor(::FS::Bar::ProjectInfo);

	//toolbar
	toolBarWrite->add(&ProjectInfo::toolBar, this, text.explorer.operator const char* ());
}

FS::Bar::ProjectInfo::~ProjectInfo() noexcept {
	toolBarWrite->remove<ProjectInfo>();
	FluidumScene_Log_Destructor(::FS::Bar::ProjectInfo);
}

void FS::Bar::ProjectInfo::call() {

	this->catchWindowEvents();
}

void FS::Bar::ProjectInfo::catchWindowEvents() {
	if (this->flag.window)
		return;

	FluidumScene_Log_RequestDeleteScene(::FS::Bar::ProjectInfo);
	Scene::deleteScene<ProjectInfo>();
}

void FS::Bar::ProjectInfo::toolBar() {

}

void FS::Bar::ProjectInfo::setImGuiWindow() {
	imguiWindowWrite->set(FU::Class::ClassCode::GetClassCode<ProjectInfo>(), ImGui::GetCurrentWindow());
}

void FS::Bar::ProjectInfo::left() {

}

//void FS::Bar::ProjectInfo::projectInfo() {
//	ImGui::BulletText("Project");
//
//	if (!ImGui::BeginTable("##ProjectInfo", 2, ImGuiTableFlags_Borders))
//		return;
//
//	//project name
//	{
//		ImGui::TableNextColumn();
//		ImGui::Text("Name");
//		ImGui::TableNextColumn();
//		ImGui::Text(projectRead->projectName().c_str());
//	}
//
//	//project type
//	{
//		ImGui::TableNextColumn();
//		ImGui::Text("Type");
//		const char* name = "Error";
//		const auto projectType = propertyRead->currentProjectType();
//		name = FD::Project::Property::projectTypeToCChar(projectType);
//		ImGui::TableNextColumn();
//		ImGui::Text(name);
//	}
//
//	ImGui::EndTable();
//}
//
//void FS::Bar::ProjectInfo::projectInfo_Lua() {
//	const bool exists = propertyLuaRead->entryFileExists();
//
//	if (!ImGui::BeginTable("##MainFileInfo", 2, ImGuiTableFlags_Borders))
//		return;
//
//	const std::string path = propertyLuaRead->entryFilePath();
//
//	//entry filepath
//	{
//		ImGui::TableNextColumn();
//		ImGui::Text(text.path);
//
//		ImGui::TableNextColumn();
//		if (!exists)
//			ImGui::Text(text.mainFileDoesNotExist);
//		else {
//			ImGui::Text(path.c_str());
//		}
//	}
//
//	//entry file name
//	{
//		const std::string name = FU::File::fileName(path);
//
//		ImGui::TableNextColumn();
//		ImGui::Text(text.name);
//
//		ImGui::TableNextColumn();
//		ImGui::Text(name.c_str());
//	}
//
//	ImGui::EndTable();
//}
//
//void FS::Bar::ProjectInfo::standardFluidumLibrary() {
//
//	ImGui::BulletText(text.standardFluidumLibrary);
//
//	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4());
//	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2());
//	const float buttonWidth = ImGui::GetWindowSize().x;
//
//	constexpr ImGuiTreeNodeFlags nodeFlag =
//		ImGuiTreeNodeFlags_Selected|
//		ImGuiTreeNodeFlags_FramePadding |
//		ImGuiTreeNodeFlags_OpenOnDoubleClick |
//		ImGuiTreeNodeFlags_SpanAvailWidth;
//
//	//fluidum.system
//	if (ImGui::TreeNodeEx("fluidum.system", nodeFlag)) {
//		
//	}
//
//	//fluidum.array
//	if (ImGui::TreeNodeEx("fluidum.array", nodeFlag)) {
//
//	}
//
//	//fluidum.plot
//	if (ImGui::TreeNodeEx("fluidum.plot", nodeFlag)) {
//
//	}
//
//	//fluidum.genome
//	if (ImGui::TreeNodeEx("fluidum.genome", nodeFlag)) {
//
//	}
//
//	ImGui::PopStyleColor();
//	ImGui::PopStyleVar();
//
//}
//
