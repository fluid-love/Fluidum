#pragma once

#include "../Common/common.h"

namespace FD::Text {

	enum class Language : UT {
		Japanese,
		English,
		Chinese
	};

}

namespace FD::Text::Internal {

	using GuiText = ::FU::GuiText::Text;

}

namespace FD::Text {

	enum class CommonText : UT {
		InternalError,
		UnexceptedError,
		CollapseWindow
	};

	namespace Internal {

		struct Common final {
			explicit Common(const CommonText index);
			~Common() noexcept = default;
			FluidumUtils_Class_Delete_CopyMove(Common);

		public:
			[[nodiscard]] inline operator const GuiText& () const & noexcept {
				return this->text;
			}

			[[nodiscard]] inline const char* c_str() const & noexcept {
				return this->text.operator const char* ();
			}

			[[nodiscard]] inline const std::string& string_cr() const & noexcept {
				return this->text.operator const std::string & ();
			}

		private:
			const GuiText text;

		private:
			[[nodiscard]] GuiText init(const CommonText index) const;

		};

	}

}

namespace FD::Text::Internal {

	struct Title final {
		Title();
		~Title() noexcept = default;

		GuiText recentProject;
		GuiText newProject;
		GuiText openProject;
		GuiText document;

		GuiText error_notSupported;
		GuiText error_loadProjectHistory;
		GuiText error_openProjectFile;
		GuiText error_illegalFile;
		GuiText error_brokenFile;
		GuiText error_noexpected;

	};

	struct Layout final {
		Layout();
		~Layout() = default;

		GuiText splitVerticalCurrentPos;
		GuiText splitHorizonalCurrentPos;

		GuiText splitVerticalCenterLine;
		GuiText splitHorizonalCenterLine;

		GuiText merge;

		GuiText reset;

		GuiText error_max;

		GuiText confirm_reset;
	};

	struct TitleBar final {
		TitleBar();
		~TitleBar() = default;

		GuiText tempProject;
	};

	struct BarExit final {
		BarExit();
		~BarExit() = default;

		GuiText popup_projectMessage;
		GuiText popup_codingTabMessage;

		GuiText popup_saveAndExit;
		GuiText popup_withoutSaving;
		GuiText popup_cancel;

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
		GuiText run_debug;
		GuiText run_nomal;
		GuiText calcInfo;

		GuiText view;
		GuiText coding;
		GuiText tab;
		GuiText debugInfo;
		GuiText flu;
		GuiText analysis;
		GuiText function;
		GuiText plot;
		GuiText genome;
		GuiText animation;
		GuiText project;
		GuiText console;

		GuiText project_;
		GuiText property_icon;

		GuiText extension;
		GuiText manage;

		GuiText window;
		GuiText layoutTemplates;
		GuiText clear;

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

		GuiText confirm_changeLayout;


	};

	struct TopBar final {
		TopBar();
		~TopBar() = default;

		GuiText package;
		GuiText layout;
		GuiText templates;

		GuiText save;
		GuiText newProject;
		GuiText includeFile;

		GuiText run;

		GuiText error_mainfile;
		GuiText error_notSetProperty;
	};

	struct LeftBar final {
		LeftBar();
		~LeftBar() = default;

		GuiText popup_save;
		GuiText popup_saveAndClose;
		GuiText popup_withoutSaving;
		GuiText popup_cancel;

	};

	struct ProjectSelect final {
		ProjectSelect();
		~ProjectSelect() = default;

		GuiText selectPlease;
		GuiText quick;
		GuiText empty_lua;
		GuiText empty_py;
		GuiText empty_cpp;
		GuiText openFile;
		GuiText openFileDescription;
		GuiText newFile;
		GuiText newFileDescription;
		GuiText newDirectory;
		GuiText newDirectoryDescription;
		GuiText cancel;
		GuiText select;

		GuiText folderPath;
		GuiText fileName;

		GuiText error_notFoundDirectory;
		GuiText error_alreadyExistFile;
		GuiText error_emptyForm;
		GuiText error_forbiddenCharactor;
		GuiText error_unexpected;

	};

	struct ProjectNewFile final {
		ProjectNewFile();
		~ProjectNewFile() = default;

		GuiText title;

		GuiText search;
		GuiText all;

		GuiText recent;
		GuiText recentEmpty;

		GuiText back;
		GuiText cancel;
		GuiText create;

		GuiText folderPath;
		GuiText fileName;

		GuiText empty;
		GuiText empty_description;
		GuiText emptyLua;
		GuiText emptyLua_description;
		GuiText emptyPython;
		GuiText emptyPython_description;
		GuiText emptyCpp;
		GuiText emptyCpp_description;

		GuiText error_unexpected;


	};

	struct ProjectDirectory final {
		ProjectDirectory();
		~ProjectDirectory() = default;

		GuiText title;

		GuiText parent;
		GuiText name;

		GuiText back;
		GuiText cancel;
		GuiText create;

		GuiText error_unexpected;

	};

	struct ProjectCheckPath final {
		ProjectCheckPath();
		~ProjectCheckPath() noexcept = default;

		GuiText error_fill;
		GuiText error_parentDoesNotExist;
		GuiText error_directoryAlreadyExist;
		GuiText error_fileAlreadyExist;
		GuiText error_forbiddenCharactor;
		GuiText error_unexpected;

	};

	struct NewProject final {
		NewProject();
		~NewProject() noexcept = default;

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
		GuiText emptyCpp;
		GuiText emptyCppDescription;
		GuiText algorithm;
		GuiText cancel;
		GuiText select;

		GuiText recent_erase;
		GuiText recent_clear;
		GuiText recent_message;

	};

	struct ProjectForm final {
		ProjectForm();
		~ProjectForm() noexcept = default;

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
		GuiText error_maxSize;
		GuiText error_absolute;
		GuiText error_notFoundDirectory;
		GuiText error_alreadyExist;
		GuiText error_forbiddenCharactor;
		GuiText error_failedToCreate;



	};

	struct ProjectSaveAs final {
		ProjectSaveAs();
		~ProjectSaveAs() noexcept = default;

		GuiText title;

		GuiText directoryPath;
		GuiText projectName;

		GuiText cancel;
		GuiText save;

		GuiText confirm_notSaved;
		GuiText confirm_save;
		GuiText confirm_ignore;

		GuiText error_empty;
		GuiText error_alreadyExist;
		GuiText error_notFoundProject;
		GuiText error_notFound;
		GuiText error_forbiddenCharactor;

	};

	struct PopupSelect final {
		PopupSelect();
		~PopupSelect() noexcept = default;

		GuiText yes;
		GuiText no;
		GuiText cancel;
	};

	struct TextEditor final {
		TextEditor();
		~TextEditor() noexcept = default;

		GuiText editor;

		GuiText file;
		GuiText save;
		GuiText save_icon;
		GuiText saveAs;
		GuiText load;
		GuiText create;
		GuiText quit;

		GuiText edit;
		GuiText readOnly;
		GuiText undo;
		GuiText undo_icon;
		GuiText redo;
		GuiText redo_icon;
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

		GuiText error_forbiddenCharactor;

	};

	struct CodingTab final {
		CodingTab();
		~CodingTab() = default;

		GuiText tab;

		GuiText popup_save;
		GuiText popup_saveAndClose;
		GuiText popup_withoutSaving;
		GuiText popup_cancel;

		GuiText error_limitMaxSize;
		GuiText error_alreadyExist;
		GuiText error_notExist;
		GuiText error_unexpected;
	};

	struct Project final {
		Project();
		~Project() = default;

		GuiText explorer;

		GuiText sync;
		GuiText add;
		GuiText add_icon;
		GuiText add_selectNewFile;
		GuiText add_select;
		GuiText file;
		GuiText file_icon;
		GuiText directory;
		GuiText directory_icon;
		GuiText rename;
		GuiText displayCode;
		GuiText error_openFile;
		GuiText notice_removeFile;
		GuiText notice_removeDirectory;

		GuiText standardFluidumLibrary;
		GuiText mainFile;

		GuiText project;
		GuiText user;
		GuiText mainFileDoesNotExist;

		GuiText path;
		GuiText name;
		GuiText virtualFolder;
		GuiText newFile;
		GuiText collapseAll;
		GuiText addExistDirectory;
		GuiText addExistFile;
		GuiText open;
		GuiText close;
		GuiText remove;
		GuiText release;
		GuiText setAsMainFile;

		GuiText error_maxSize;

		GuiText confirm_releaseVirtualFolder;
		GuiText confirm_releaseFile;
		GuiText confirm_removeDirectory;
		GuiText confirm_removeFile;
		GuiText confirm_releaseFile_notSaved;


		GuiText error_sameName;
		GuiText error_emptyName;
		GuiText error_forbiddenCharactor;
		GuiText error_addDirectory;
		GuiText error_addFile;
		GuiText error_removeDirectory;
		GuiText error_removeFile;
		GuiText error_releaseVirtualFolder;
		GuiText error_releaseFile;
		GuiText error_tabFileSize;
		GuiText error_existFile;
		GuiText error_hugeFile;
		GuiText error_changeName;
		GuiText error_tab;
		GuiText error_fileDoesNotExist;
		GuiText error_unexpected;

	};

	struct AnalysisOverview final {
		AnalysisOverview();
		~AnalysisOverview() noexcept = default;

		GuiText function;




	};

	struct Console final {
		Console();
		~Console() noexcept = default;

		GuiText console;
		GuiText clear;
		GuiText backcolor;
	};

	struct ProjectProperty final {
		ProjectProperty();
		~ProjectProperty() noexcept = default;

		GuiText projectProperty;

		GuiText tab_main;

		GuiText currentType;
		GuiText change;

		GuiText entryFilePath;

		GuiText luaVersion;
		GuiText currentVersion;

		GuiText confirm_changeProjectType;
		GuiText confirm_notSaved;

		GuiText confirm_save;
		GuiText confirm_ignore;

		GuiText info_currentType;
		GuiText info_entryFilePath;
		GuiText info_luaVersion;

		GuiText bottom_close;
		GuiText bottom_cancel;
		GuiText bottom_save;


	};

}

namespace FD::Text {

	using ::FD::Text::Internal::Common;
	using ::FD::Text::Internal::Title;
	using ::FD::Text::Internal::Layout;
	using ::FD::Text::Internal::TitleBar;
	using ::FD::Text::Internal::StatusBar;
	using ::FD::Text::Internal::MenuBar;
	using ::FD::Text::Internal::TopBar;
	using ::FD::Text::Internal::LeftBar;
	using ::FD::Text::Internal::ProjectSelect;
	using ::FD::Text::Internal::ProjectNewFile;
	using ::FD::Text::Internal::ProjectDirectory;
	using ::FD::Text::Internal::ProjectCheckPath;
	using ::FD::Text::Internal::NewProject;
	using ::FD::Text::Internal::ProjectForm;
	using ::FD::Text::Internal::PopupSelect;
	using ::FD::Text::Internal::TextEditor;
	using ::FD::Text::Internal::CodingTab;
	using ::FD::Text::Internal::ProjectSaveAs;
	using ::FD::Text::Internal::Project;
	using ::FD::Text::Internal::AnalysisOverview;
	using ::FD::Text::Internal::BarExit;
	using ::FD::Text::Internal::Console;
	using ::FD::Text::Internal::ProjectProperty;

}

namespace FD {

	//はじめから所持する．
	//再起動で言語の変更を可能にする．
	//コンストラクタで現在の言語は何かを読み込む.
	class GuiTextRead final {
	public:
		GuiTextRead(Internal::PassKey);
		~GuiTextRead() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(GuiTextRead);

	public:
		[[nodiscard]] Text::Language getType() const noexcept;

	};


	//	class StyleColorWrite final {
	//	public:
	//		StyleColorWrite(Internal::
	// ) {}
	//		~StyleColorWrite() = default;
	//		FluidumUtils_Class_Delete_CopyMove(StyleColorWrite);
	//
	//#ifdef FluidumData_DebugMode
	//			StyleColorWrite() = default;
	//#endif 
	//
	//
	//	};

}