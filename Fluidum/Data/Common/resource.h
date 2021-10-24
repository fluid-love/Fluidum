#pragma once

#include "../../Utils/include.h"

namespace FD::Internal::Resource {

#ifdef Fluidum_Path
	constexpr inline const char* GuiStyleFilePath = Fluidum_Path "/Resources/Gui/.style";
	constexpr inline const char* GuiTextFilePath = Fluidum_Path "/Resources/Gui/.lang";
#else
	constexpr inline const char* GuiStyleFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Gui/.style")>();
	constexpr inline const char* GuiTextFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Gui/.lang")>();
#endif	

}

namespace FD::Internal::Resource {

#ifdef Fluidum_Path
	constexpr inline const char* JapaneseGuiTextFolderPath = Fluidum_Path "/Resources/Gui/Text/Japanese/";
	constexpr inline const char* EnglishGuiTextFolderPath = Fluidum_Path "/Resources/Gui/Text/English/";
	constexpr inline const char* ChineseGuiTextFolderPath = Fluidum_Path "/Resources/Gui/Text/Chinese/";

#else
	constexpr inline const char* JapaneseGuiTextFolderPath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Gui/Text/Japanese/")>();
	constexpr inline const char* EnglishGuiTextFolderPath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Gui/Text/English/")>();
	constexpr inline const char* ChineseGuiTextFolderPath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Gui/Text/Chinese/")>();

#endif	

}

namespace FD::Internal::Resource {

#ifdef Fluidum_Path
	constexpr inline const char* TempProjectFolderPath = Fluidum_Path "/Resources/Project/TempProject/";
	constexpr inline const char* RecentProjectHistoryFilePath = Fluidum_Path "/Resources/Project/RecentProject.history";

#else
	constexpr inline const char* TempProjectFolderPath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Project/TempProject/")>();
	constexpr inline const char* RecentProjectHistoryFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Project/RecentProject.history")>();

#endif	

}

namespace FD::Internal::Resource {

#ifdef Fluidum_Path
	constexpr inline const char* RecentProjectTemplateHistoryFilePath = Fluidum_Path "/Resources/Log/Recent/RecentTemplateProject.history";
	constexpr inline const char* RecentFileHistoryFilePath = Fluidum_Path "/Resources/Log/Recent/RecentFile.history";

#else
	constexpr inline const char* RecentProjectTemplateHistoryFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Log/Recent/RecentTemplateProject.history")>();
	constexpr inline const char* RecentFileHistoryFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Log/Recent/RecentFile.history")>();

#endif	

}