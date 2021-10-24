#pragma once

#include "../../Manager/include.h"
#include "../../Data/include.h"
#include "../../Draw/include.h"

#include "../../Libraries/ImGuiAnimation/ImGuiAnimation.h"

namespace FS {

	using MainFluidumScene = ::FS::FluidumScene <
		FD::GuiWrite,
		FD::GuiRead,
		FD::SceneRead,
		FD::WindowWrite,
		FD::WindowRead,
		FD::GuiTextRead,
		FD::StyleColorWrite,
		FD::StyleColorRead,
		FD::LayoutWrite,
		FD::LayoutRead,
		FD::ProjectWrite,
		FD::ProjectRead,
		FD::Coding::TabWrite,
		FD::Coding::TabRead,
		FD::ConsoleWrite,
		FD::FunctionWrite<FD::Calc::Language::Lua>,
		FD::FunctionRead<FD::Calc::Language::Lua>,
		FD::ImPlotWrite,
		FD::ImPlotRead,
		FD::TopBarWrite,
		FD::TopBarRead,
		FD::ImGuiWindowWrite,
		FD::ImGuiWindowRead,
		FD::TaskWrite,
		FD::TaskRead,
		FD::Log::FileWrite,
		FD::Log::FileRead,
		FD::Log::ProjectWrite,
		FD::Log::ProjectRead
	> ;

	//äeÉVÅ[ÉìÇ≈publicåpè≥Ç∑ÇÈ upcastÇ∑ÇÈÇÃÇ≈public
	using Scene = MainFluidumScene::SceneBaseType;

}
