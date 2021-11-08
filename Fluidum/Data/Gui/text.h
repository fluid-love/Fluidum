#pragma once

#include "../Common/common.h"

namespace FD::Text {
	enum class Language : uint8_t {
		Japanese,
		English,
		Chinese
	};
}

namespace FD::Internal::Text {

	using GuiText = ::FU::GuiText::Text;

	struct Title final {
		Title();
		~Title() = default;

		GuiText recentProject;
		GuiText newProject;
		GuiText openProject;
		GuiText document;

		GuiText error_openProjectFile;
		GuiText error_illegalFile;
		GuiText error_brokenFile;
		GuiText error_internal;

	};

	struct TitleBar final {
		TitleBar();
		~TitleBar() = default;

		GuiText popupMessage;
		GuiText saveAndExit;
		GuiText withoutSaving;
		GuiText cancel;

	};

	struct StatusBar final {
		StatusBar();
		~StatusBar() = default;

		GuiText task;
		GuiText version;
		GuiText fps;
		GuiText taskInfo;

	};


	struct MenuBar final {
		MenuBar();
		~MenuBar() = default;

		GuiText file;
		GuiText create;
		GuiText open;
		GuiText save;
		GuiText saveFileAs;
		GuiText terminate;

		GuiText edit;

		GuiText calc;
		GuiText run;
		GuiText runIgnoreStep;
		GuiText calcInfo;

		GuiText view;

		GuiText help;
		GuiText document;

		GuiText checkCurrentProject;
		GuiText cancel;
		GuiText saveAndOpen;
		GuiText ignore;

		GuiText saveAndExit;
		GuiText exitWithoutSaving;

		GuiText error_openProjectFile;
		GuiText error_illegalFile;
		GuiText error_brokenFile;
		GuiText error_internal;



	};

	struct TopBar final {
		TopBar();
		~TopBar() = default;

		GuiText package;
		GuiText layout;
		GuiText templates;
		GuiText tempProject;

		GuiText save;
		GuiText newProject;
		GuiText includeFile;

		GuiText error_mainfile;

	};

	struct LeftBar final {
		LeftBar();
		~LeftBar() = default;

		GuiText popup_save;
		GuiText popup_saveAndClose;
		GuiText popup_withoutSaving;
		GuiText popup_cancel;

	};

	struct CodingSelect final {
		CodingSelect();
		~CodingSelect() = default;

		GuiText selectPlease;
		GuiText quick;
		GuiText empty_lua;
		GuiText empty_py;
		GuiText empty_as;
		GuiText openFile;
		GuiText openFileDescription;
		GuiText newFile;
		GuiText newFileDescription;
		GuiText cancel;
		GuiText select;

		GuiText folderPath;
		GuiText fileName;

		GuiText error_notFoundDirectory;
		GuiText error_alreadyExistFile;
		GuiText error_emptyForm;

	};

	struct CodingNew final {
		CodingNew();
		~CodingNew() = default;

		GuiText title;
		GuiText recent;
		GuiText recent_empty;
		GuiText cancel;
		GuiText create;
		GuiText folderPath;
		GuiText fileName;

		GuiText empty;
		GuiText empty_Description;
		GuiText emptyLua;
		GuiText emptyLua_Description;
		GuiText emptyPython;
		GuiText emptyPython_Description;
		GuiText emptyAngelScript;
		GuiText emptyAngelScript_Description;

		GuiText error_fill;
		GuiText error_directoryNotFound;
		GuiText error_fileAlreadyExist;


	};

	struct NewProject final {
		NewProject();
		~NewProject() = default;

		GuiText newProject;
		GuiText search;
		GuiText filter;
		GuiText all;
		GuiText recent;
		GuiText NotApplicable;
		GuiText empty;
		GuiText emptyDescription;
		GuiText emptyLua;
		GuiText emptyLuaDescription;
		GuiText emptyPython;
		GuiText emptyPythonDescription;
		GuiText emptyAngelScript;
		GuiText emptyAngelScriptDescription;
		GuiText algorithm;
		GuiText interactive;
		GuiText interactiveDescription;
		GuiText cancel;
		GuiText select;

	};

	struct ProjectForm final {
		ProjectForm();
		~ProjectForm() = default;

		GuiText fillPlease;
		GuiText folderPath;
		GuiText projectName;
		GuiText alreadyExist;
		GuiText error;
		GuiText back;
		GuiText cancel;
		GuiText create;

		GuiText notSaved;
		GuiText saveAndCreate;
		GuiText createWithoutSaving;

		GuiText error_emptyForm;
		GuiText error_notFoundDirectory;
		GuiText error_alreadyExist;
		GuiText error_failedToCreate;



	};

	struct ProjectSaveAs final {
		ProjectSaveAs();
		~ProjectSaveAs() = default;

		GuiText title;

		GuiText folderPath;
		GuiText projectName;

		GuiText cancel;
		GuiText save;

		GuiText checkCurrentProject;
		GuiText saveAndWrite;
		GuiText ignore;

		GuiText error_empty;
		GuiText error_alreadyExist;
		GuiText error_notFound;
		GuiText error_failed;
	};

	struct PopupSelect final {
		PopupSelect();
		~PopupSelect() = default;

		GuiText yes;
		GuiText no;
		GuiText cancel;
	};

	struct TextEditor final {
		TextEditor();
		~TextEditor() = default;

		GuiText editor;

		GuiText file;
		GuiText save;
		GuiText saveAs;
		GuiText load;
		GuiText create;
		GuiText quit;

		GuiText edit;
		GuiText readOnly;
		GuiText undo;
		GuiText redo;
		GuiText copy;
		GuiText cut;
		GuiText del;
		GuiText paste;
		GuiText selectAll;

		GuiText theme;
		GuiText default_;
		GuiText dark;
		GuiText light;
		GuiText blue;

		GuiText line;
		GuiText column;


	};

	struct CodingTab final {
		CodingTab();
		~CodingTab() = default;

		GuiText error_limitMaxSize;
		GuiText error_alreadyExist;

		GuiText popup_save;
		GuiText popup_saveAndClose;
		GuiText popup_withoutSaving;
		GuiText popup_cancel;

	};

	struct Project final {
		Project();
		~Project() = default;

		GuiText sync;
		GuiText add;
		GuiText file;
		GuiText folder;
		GuiText rename;
		GuiText remove;
		GuiText displayCode;
		GuiText error_openFile;
		GuiText notice_removeFile;
		GuiText notice_removeFolder;

		GuiText standardFluidumLibrary;
		GuiText mainFile;
		GuiText projectFolder;

		GuiText project;
		GuiText user;
		GuiText mainFileDoesNotExist;

		GuiText path;
		GuiText name;
		GuiText createFolder;
		GuiText createVirtualFolder;
		GuiText createFile;
		GuiText addExistFolder;
		GuiText addExistFile;
		GuiText open;
		GuiText close;
		GuiText delete_release;

		GuiText error_maxSize;

		GuiText confirm_releaseVirtualFolder;
		GuiText confirm_releaseFile;
		GuiText confirm_deleteFolder;
		GuiText confirm_deleteFile;

		GuiText error_sameName;
		GuiText error_emptyName;
		GuiText error_forbiddenCharactor;
	};

	struct AnalysisOverview final {
		AnalysisOverview();
		~AnalysisOverview() = default;

		GuiText function;




	};




}

namespace FD::Text {
	using ::FD::Internal::Text::Title;
	using ::FD::Internal::Text::TitleBar;
	using ::FD::Internal::Text::StatusBar;
	using ::FD::Internal::Text::MenuBar;
	using ::FD::Internal::Text::TopBar;
	using ::FD::Internal::Text::LeftBar;
	using ::FD::Internal::Text::CodingSelect;
	using ::FD::Internal::Text::CodingNew;
	using ::FD::Internal::Text::NewProject;
	using ::FD::Internal::Text::ProjectForm;
	using ::FD::Internal::Text::PopupSelect;
	using ::FD::Internal::Text::TextEditor;
	using ::FD::Internal::Text::CodingTab;
	using ::FD::Internal::Text::ProjectSaveAs;
	using ::FD::Internal::Text::Project;
	using ::FD::Internal::Text::AnalysisOverview;

}

namespace FD {

	//はじめから所持する．
	//再起動で言語の変更を可能にする．
	//コンストラクタで現在の言語は何かを読み込む.
	class GuiTextRead final {
	public:
		GuiTextRead(Internal::PassKey);
		~GuiTextRead() = default;
		FluidumUtils_Class_Delete_CopyMove(GuiTextRead)

	public:
		_NODISCARD Text::Language getType() const noexcept;

	};


	//	class StyleColorWrite final {
	//	public:
	//		StyleColorWrite(Internal::
	// ) {}
	//		~StyleColorWrite() = default;
	//		FluidumUtils_Class_Delete_CopyMove(StyleColorWrite)
	//
	//#ifdef FluidumData_DebugMode
	//			StyleColorWrite() = default;
	//#endif 
	//
	//
	//	};

}