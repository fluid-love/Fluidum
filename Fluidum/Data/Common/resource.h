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


#ifdef Fluidum_Path
	constexpr inline const char* TitleGuiTextFilePath = Fluidum_Path "/Resources/Gui/Text/Title.jpn";
	constexpr inline const char* MenuBarGuiTextFilePath = Fluidum_Path "/Resources/Gui/Text/MenuBar.jpn";
	constexpr inline const char* StatusBarGuiTextFilePath = Fluidum_Path "/Resources/Gui/Text/StatusBar.jpn";
	constexpr inline const char* TopBarGuiTextFilePath = Fluidum_Path "/Resources/Gui/Text/TopBar.jpn";
	constexpr inline const char* CodingSelectGuiTextFilePath = Fluidum_Path "/Resources/Gui/Text/CodingSelect.jpn";
	constexpr inline const char* NewProjectGuiTextJpnFilePath = Fluidum_Path "/Resources/Gui/Text/NewProject.jpn";
	constexpr inline const char* ProjectFormGuiTextJpnFilePath = Fluidum_Path "/Resources/Gui/Text/ProjectForm.jpn";
	constexpr inline const char* ProjectFormGuiTextJpnFilePath = Fluidum_Path "/Resources/Gui/Text/ProjectSaveAs.jpn";
	constexpr inline const char* PopupSelectGuiTextJpnFilePath = Fluidum_Path "/Resources/Gui/Text/PopupSelect.jpn";
	constexpr inline const char* TextEditorGuiTextJpnFilePath = Fluidum_Path "/Resources/Gui/Text/TextEditor.jpn";
	constexpr inline const char* CodingTabGuiTextJpnFilePath = Fluidum_Path "/Resources/Gui/Text/CodingTab.jpn";
	constexpr inline const char* ProjectGuiTextJpnFilePath = Fluidum_Path "/Resources/Gui/Text/Project.jpn";

#else
	constexpr inline const char* TitleGuiTextJpnFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Gui/Text/Title.jpn")>();
	constexpr inline const char* MenuBarGuiTextJpnFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Gui/Text/MenuBar.jpn")>();
	constexpr inline const char* StatusBarGuiTextJpnFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Gui/Text/StatusBar.jpn")>();
	constexpr inline const char* TopBarGuiTextJpnFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Gui/Text/TopBar.jpn")>();
	constexpr inline const char* CodingSelectGuiTextJpnFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Gui/Text/CodingSelect.jpn")>();
	constexpr inline const char* NewProjectGuiTextJpnFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Gui/Text/NewProject.jpn")>();
	constexpr inline const char* ProjectFormGuiTextJpnFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Gui/Text/ProjectForm.jpn")>();
	constexpr inline const char* PopupSelectGuiTextJpnFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Gui/Text/PopupSelect.jpn")>();
	constexpr inline const char* ProjectSaveAsGuiTextJpnFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Gui/Text/ProjectSaveAs.jpn")>();
	constexpr inline const char* TextEditorGuiTextJpnFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Gui/Text/TextEditor.jpn")>();
	constexpr inline const char* CodingTabGuiTextJpnFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Gui/Text/CodingTab.jpn")>();
	constexpr inline const char* ProjectGuiTextJpnFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Gui/Text/Project.jpn")>();

#endif	


#ifdef Fluidum_Path
	constexpr inline const char* TempProjectFolderPath = Fluidum_Path "/Resources/Project/TempProject/";
	constexpr inline const char* RecentProjectHistoryFilePath = Fluidum_Path "/Resources/Project/RecentProject.history";

#else
	constexpr inline const char* TempProjectFolderPath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Project/TempProject/")>();
	constexpr inline const char* RecentProjectHistoryFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 4, FU::File::PathArg("/Resources/Project/RecentProject.history")>();

#endif	



}

