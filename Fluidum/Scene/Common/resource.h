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

//Project
namespace FS::Resource {

#ifdef Fluidum_Path
	constexpr inline const char* ProjectNewFilePath = Fluidum_Path "/Resources/Images/Project/Icon/open.png";
	constexpr inline const char* ProjectOpenFilePath = Fluidum_Path "/Resources/Images/Project/Icon/new.png";
	constexpr inline const char* ProjectDirectoryPath = Fluidum_Path "/Resources/Images/Project/Icon/dir.png";

	constexpr inline const char* ProjectNewIconsFolderPath = Fluidum_Path "/Resources/Images/Project/Icons/";

#else
	constexpr inline const char* ProjectOpenFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Images/Project/Icon/open.png")>();
	constexpr inline const char* ProjectNewFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Images/Project/Icon/new.png")>();
	constexpr inline const char* ProjectDirectoryPath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Images/Project/Icon/dir.png")>();
								 
	constexpr inline const char* ProjectNewIconsFolderPath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Images/Project/Icons/")>();
								 
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
	constexpr inline const char* LuaMessagePath = Fluidum_Path "/Resources/Log/Lua/jpn";
	constexpr inline const char* LuaFluidumStandardLibraryFolderPath = Fluidum_Path "/Fluidum/Language/Lua/FluidumStandardLibrary/";
	constexpr inline const char* LuaInitFilePath = Fluidum_Path "/Fluidum/Language/Lua/init.lua";

#else
	constexpr inline const char* LuaMessagePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Log/Lua/jpn")>();
	constexpr inline const char* LuaFluidumStandardLibraryFolderPath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Fluidum/Languages/Lua/FluidumStandardLibrary/")>();
	constexpr inline const char* LuaInitFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Fluidum/Languages/Lua/init.lua")>();

#endif	

}