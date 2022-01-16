#pragma once

#include "../../Manager/include.h"
#include "../../Data/include.h"
#include "../../Draw/include.h"

#include "../../Internal/ImGuiAnimation/ImGuiAnimation.h"

namespace FS {

	using MainFluidumScene = ::FS::FluidumScene <
		FD::GuiWrite,
		FD::GuiRead,
		FD::SceneRead,
		FD::WindowWrite,
		FD::WindowRead,
		FD::ExitWrite,
		FD::ExitRead,
		FD::GuiTextRead,
		FD::Style::ThemeWrite,
		FD::Style::ThemeRead,
		FD::Style::ColorWrite,
		FD::Style::ColorRead,
		FD::Style::VarWrite,
		FD::Style::VarRead,
		FD::LayoutWrite,
		FD::LayoutRead,
		FD::ProjectWrite,
		FD::ProjectRead,
		FD::ProjectFilesWrite_Lock,
		FD::ProjectFilesRead_Lock,
		FD::ProjectFilesRead,
		FD::UserFilesWrite_Lock,
		FD::UserFilesRead_Lock,
		FD::UserFilesRead,
		FD::Coding::TabWrite,
		FD::Coding::TabRead,
		FD::Coding::DisplayWrite,
		FD::Coding::DisplayRead,
		FD::ConsoleWrite,
		FD::ConsoleRead,
		FD::FunctionWrite<FD::Calc::Language::Lua>,
		FD::FunctionRead<FD::Calc::Language::Lua>,
		FD::ImPlotWrite,
		FD::ImPlotRead,
		FD::ImPlotRead_Lock,
		FD::CalcWrite,
		FD::CalcRead,
		FD::Calc::ArrayWrite,
		FD::Calc::ArrayRead,
		FD::ToolBarWrite,
		FD::ToolBarRead,
		FD::ImGuiWindowWrite,
		FD::ImGuiWindowRead,
		FD::TaskWrite,
		FD::TaskRead,
		FD::History::FileWrite,
		FD::History::FileRead,
		FD::History::ProjectWrite,
		FD::History::ProjectRead,
		FD::Project::PropertyWrite,
		FD::Project::PropertyRead,
		FD::Project::PropertyLuaWrite,
		FD::Project::PropertyLuaRead
	> ;

	//public inheritance in each scene.
	using Scene = MainFluidumScene::SceneBaseType;

}
