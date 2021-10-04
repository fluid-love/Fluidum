#pragma once

#include "../Common/common.h"

namespace FD::Text {
	enum class Language : uint8_t {
		Japanese,
		English
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

		GuiText credit;

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

		GuiText mode;
		GuiText theme;
		GuiText dark;
		GuiText light;
		GuiText window;

	};


}

namespace FD::Text {
	using ::FD::Internal::Text::Title;
	using ::FD::Internal::Text::StatusBar;
	using ::FD::Internal::Text::MenuBar;
	using ::FD::Internal::Text::TopBar;
	using ::FD::Internal::Text::CodingSelect;
	using ::FD::Internal::Text::NewProject;
	using ::FD::Internal::Text::ProjectForm;
	using ::FD::Internal::Text::PopupSelect;
	using ::FD::Internal::Text::TextEditor;

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