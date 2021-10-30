#pragma once

#include "using.h"

//相対パスにする時はResources/までの相対パスを指定する

namespace FS::Resource {

#ifdef Fluidum_Path
	constexpr inline const char* TitleImageFilePath = Fluidum_Path "/Resources/Images/Title/title.jpg";
#else
	constexpr inline const char* TitleImageFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Images/Title/title.jpg")>();
#endif	

}

//Bar
namespace FS::Resource {

#ifdef FluidumScene_Path
	constexpr inline const char* LeftBarIconsFilePath = Fluidum_Path "/Resources/Images/Bar/Icons/";
	constexpr inline const char* TitleBarIconFilePath = Fluidum_Path "/Resources/Images/Bar/Icon/Fluidum.png";
	constexpr inline const char* NewProjectIconsFolderPath = Fluidum_Path "/Resources/Images/Bar/NewProjectIcons/";

#else
	constexpr inline const char* LeftBarIconsFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Images/Bar/Icons/")>();
	constexpr inline const char* TitleBarIconFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Images/Bar/Icon/Fluidum.png")>();
	constexpr inline const char* NewProjectIconsFolderPath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Images/Bar/NewProjectIcons/")>();

#endif	

}

//Font
namespace FS::Resource {

#ifdef Fluidum_Path
	constexpr inline const char* MPlusFontFilePath = Fluidum_Path "/Resources/Fonts/mplus/mplus-1c-medium.ttf";
#else
	constexpr inline const char* MPlusFontFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Fonts/mplus/mplus-1c-medium.ttf")>();
#endif	

}

//Coding
namespace FS::Resource {

#ifdef Fluidum_Path
	constexpr inline const char* CodingNewFilePath = Fluidum_Path "/Resources/Images/Coding/Icon/open.png";
	constexpr inline const char* CodingOpenFilePath = Fluidum_Path "/Resources/Images/Coding/Icon/new.png";
	constexpr inline const char* CodingNewIconsFolderPath = Fluidum_Path "/Resources/Images/Coding/Icons/";

#else
	constexpr inline const char* CodingOpenFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Images/Coding/Icon/open.png")>();
	constexpr inline const char* CodingNewFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Images/Coding/Icon/new.png")>();
	constexpr inline const char* CodingNewIconsFolderPath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Images/Coding/Icons/")>();

#endif	

}

//Utils
namespace FS::Resource {

#ifdef Fluidum_Path
	constexpr inline const char* UtilsIconFolderPath = Fluidum_Path "/Resources/Utils/icon/";
#else
	constexpr inline const char* UtilsIconFolderPath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/FluidumSceneUtils/icon/")>();
#endif	

}

//Calc
namespace FS::Resource {

#ifdef Fluidum_Path
	constexpr inline const char* LuaMessagePath = Fluidum_Path "/Resources/Log/LuaMessage.jpn";
	constexpr inline const char* LuaFolderPath = Fluidum_Path "/Resources/Source/Lua/";
	constexpr inline const char* LuaSLLPlotPath = Fluidum_Path  "/Resources/Source/Lua/plot.lua";
	constexpr inline const char* LuaSLLGenomePath = Fluidum_Path  "/Resources/Source/Lua/genome.lua";
	constexpr inline const char* LuaSLLPianoPath = Fluidum_Path  "/Resources/Source/Lua/piano.lua";
	constexpr inline const char* LuaSLLMathPath = Fluidum_Path  "/Resources/Source/Lua/math.lua";

#else
	constexpr inline const char* LuaMessagePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Log/LuaMessage.jpn")>();
	constexpr inline const char* LuaFolderPath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Source/Lua/")>();
	constexpr inline const char* LuaSLLPlotPath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Source/Lua/plot.lua")>();
	constexpr inline const char* LuaSLLGenomePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Source/Lua/genome.lua")>();
	constexpr inline const char* LuaSLLPianoPath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Source/Lua/piano.lua")>();
	constexpr inline const char* LuaSLLMathPath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Source/Lua/math.lua")>();

#endif	

}