#pragma once

#include "../../Manager/include.h"
#include "../../Data/include.h"
#include "../../Draw/include.h"

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
		FD::Coding::TabRead
	>;

	//�e�V�[����public�p������ upcast����̂�public
	using Scene = MainFluidumScene::SceneBaseType;

}
