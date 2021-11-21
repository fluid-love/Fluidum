#include "text.h"

#include "../../../Libraries/IconFontCppHeaders/IconsMaterialDesign.h"
#include "../../../Libraries/IconFontCppHeaders/IconsFontAwesome5.h"

namespace FD::Internal::Text {
	using Language = ::FD::Text::Language;

	std::string newLine(std::ifstream& ifs, const std::string& src) {
		std::string temp{};
		temp.push_back('\n');
		std::getline(ifs, temp);
		return src + '\n' + temp;
	}

	enum class Type : uint8_t {
		Analysis,
		BarExit,
		CodingNew,
		CodingSelect,
		CodingTab,
		Layout,
		LeftBar,
		MenuBar,
		NewProject,
		PopupSelect,
		Project,
		ProjectForm,
		ProjectSaveAs,
		StatusBar,
		TextEditor,
		Title,
		TitleBar,
		TopBar
	};


	template<Type T>
	std::string makePath(const Language lang) {
		using enum Type;
		std::string path{};
		if (lang == Language::Japanese)
			path += Resource::JapaneseGuiTextFolderPath;
		else if (lang == Language::English)
			path += Resource::EnglishGuiTextFolderPath;
		else if (lang == Language::Chinese)
			path += Resource::ChineseGuiTextFolderPath;
		else
			abort();

		if constexpr (T == Analysis) {
			path += "Analysis";
		}
		else if constexpr (T == BarExit) {
			path += "BarExit";
		}
		else if constexpr (T == CodingNew) {
			path += "CodingNew";
		}
		else if constexpr (T == CodingSelect) {
			path += "CodingSelect";
		}
		else if constexpr (T == CodingTab) {
			path += "CodingTab";
		}
		else if constexpr (T == Layout) {
			path += "Layout";
		}
		else if constexpr (T == LeftBar) {
			path += "LeftBar";
		}
		else if constexpr (T == MenuBar) {
			path += "MenuBar";
		}
		else if constexpr (T == NewProject) {
			path += "NewProject";
		}
		else if constexpr (T == PopupSelect) {
			path += "PopupSelect";
		}
		else if constexpr (T == Project) {
			path += "Project";
		}
		else if constexpr (T == ProjectForm) {
			path += "ProjectForm";
		}
		else if constexpr (T == ProjectSaveAs) {
			path += "ProjectSaveAs";
		}
		else if constexpr (T == StatusBar) {
			path += "StatusBar";
		}
		else if constexpr (T == TextEditor) {
			path += "TextEditor";
		}
		else if constexpr (T == TopBar) {
			path += "TopBar";
		}
		else if constexpr (T == Title) {
			path += "Title";
		}
		else if constexpr (T == TitleBar) {
			path += "TitleBar";
		}
		else {
			static_assert(FU::Concept::DelayAssert_N<T>);
		}
		return path;
	}



}

namespace FD::Internal::Text {

	//�O���錾
	class Getter;

	class LangType final {
	private:
		static inline Language type = Language::Japanese;

	private:
		friend class Getter;
		friend class GuiTextRead;
	};

	class Getter final {
	public:
		static Language get() noexcept {
			return LangType::type;
		}
	};

}

FD::GuiTextRead::GuiTextRead(Internal::PassKey) {
	std::ifstream ifs(Internal::Resource::GuiTextFilePath, std::ios::in);

	std::string data{};
	std::getline(ifs, data);

	if (data == "Japanese")
		Internal::Text::LangType::type = Text::Language::Japanese;
	else if (data == "English")
		Internal::Text::LangType::type = Text::Language::English;
	else
		throw std::runtime_error("Failed to read GuiTextType.");

}

FD::Internal::Text::Title::Title() {
	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::Title>(Getter::get()), std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open Title.");

	std::string data = "";

	std::getline(ifs, data);
	this->recentProject = data;

	std::getline(ifs, data);
	this->newProject = data;

	std::getline(ifs, data);
	this->openProject = data;

	std::getline(ifs, data);
	this->document = data;

	std::getline(ifs, data);
	this->error_openProjectFile = Internal::Text::newLine(ifs, data);

	std::getline(ifs, data);
	this->error_illegalFile = data;

	std::getline(ifs, data);
	this->error_brokenFile = data;

	std::getline(ifs, data);
	this->error_internal = data;
}

FD::Internal::Text::TitleBar::TitleBar() {
	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::TitleBar>(Getter::get()), std::ios::in);
	if (!ifs)
		throw std::runtime_error("Failed to open TitleBar.");

}

FD::Internal::Text::Layout::Layout() {
	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::Layout>(Getter::get()), std::ios::in);
	if (!ifs)
		throw std::runtime_error("Failed to open Layout.");

	std::string data = "";

	std::getline(ifs, data);
	this->splitVerticalCurrentPos = ICON_MD_BORDER_VERTICAL "  " + data;

	std::getline(ifs, data);
	this->splitHorizonalCurrentPos = ICON_MD_BORDER_HORIZONTAL "  " + data;

	std::getline(ifs, data);
	this->splitCrossCurrentPos = ICON_MD_BORDER_INNER "  " + data;

	std::getline(ifs, data);
	this->splitVerticalCenterLine = ICON_MD_BORDER_VERTICAL "  " + data;

	std::getline(ifs, data);
	this->splitHorizonalCenterLine = ICON_MD_BORDER_HORIZONTAL "  " + data;

	std::getline(ifs, data);
	this->splitCrossCenterLine = ICON_MD_BORDER_INNER "  " + data;

	std::getline(ifs, data);
	this->merge = ICON_MD_HEALING "  " + data;

	std::getline(ifs, data);
	this->reset = ICON_MD_CHECK_BOX_OUTLINE_BLANK "  " + data;

	std::getline(ifs, data);
	this->horizonal = ICON_MD_COMPARE_ARROWS "  " + data;

	std::getline(ifs, data);
	this->vertical = ICON_MD_IMPORT_EXPORT "  " + data;

	std::getline(ifs, data);
	this->error_max = data;

}

FD::Internal::Text::BarExit::BarExit() {
	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::BarExit>(Getter::get()), std::ios::in);
	if (!ifs)
		throw std::runtime_error("Failed to open BarExit.");

	std::string data = "";

	std::getline(ifs, data);
	this->popup_projectMessage = Internal::Text::newLine(ifs, data);

	std::getline(ifs, data);
	this->popup_codingTabMessage = Internal::Text::newLine(ifs, data);

	std::getline(ifs, data);
	this->popup_saveAndExit = data;

	std::getline(ifs, data);
	this->popup_withoutSaving = data;

	std::getline(ifs, data);
	this->popup_cancel = data;
}

FD::Internal::Text::StatusBar::StatusBar() {
	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::StatusBar>(Getter::get()), std::ios::in);
	if (!ifs)
		throw std::runtime_error("Failed to open StatusBar.");

	std::string data = "";

	this->task = ICON_MD_EVENT_NOTE;

	std::getline(ifs, data);
	this->version = data;//Version

	std::getline(ifs, data);
	this->fps = data;//FPS:

	std::getline(ifs, data);
	this->taskInfo = data;
}

FD::Internal::Text::MenuBar::MenuBar() {
	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::MenuBar>(Getter::get()), std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open MenuBar.");

	std::string data = "";

	std::getline(ifs, data);
	this->file = data;

	std::getline(ifs, data);
	this->create = ICON_FA_PLUS "   " + data;;

	std::getline(ifs, data);
	this->open = ICON_FA_FOLDER_OPEN "   " + data;

	std::getline(ifs, data);
	this->save = ICON_FA_SAVE "   " + data;

	std::getline(ifs, data);
	this->saveFileAs = ICON_FA_COPY "   " + data;

	std::getline(ifs, data);
	this->terminate = ICON_FA_SIGN_OUT_ALT "   " + data;

	std::getline(ifs, data);
	this->edit = data;

	std::getline(ifs, data);
	this->calc = data;

	std::getline(ifs, data);
	this->run_debug = ICON_FA_BUG "   " + data;

	std::getline(ifs, data);
	this->run_nomal = ICON_FA_RUNNING "   " + data;

	std::getline(ifs, data);
	this->calcInfo = data;

	std::getline(ifs, data);
	this->view = data;

	std::getline(ifs, data);
	this->coding = ICON_MD_CODE "   " + data;

	std::getline(ifs, data);
	this->tab = ICON_MD_LIST "   " + data;

	std::getline(ifs, data);
	this->debugInfo = ICON_MD_BUG_REPORT "   " + data;

	std::getline(ifs, data);
	//Account Tree: code point = e97a
	this->flu = "\xee\xa5\xba" "   " + data;

	std::getline(ifs, data);
	this->analysis = ICON_MD_DONUT_LARGE "   " + data;

	std::getline(ifs, data);
	this->function = ICON_MD_YOUTUBE_SEARCHED_FOR "   " + data;

	std::getline(ifs, data);
	this->plot = ICON_MD_MULTILINE_CHART "   " + data;

	std::getline(ifs, data);
	//Biotech: code point = ea3a
	this->genome = "\xee\xa8\xba" "   " + data;

	std::getline(ifs, data);
	//Animation: code point = e71c
	this->animation = "\xee\x9c\x9c" "   " + data;

	std::getline(ifs, data);
	//Category: code point = e861
	this->project = "\xee\xa1\xa1" "   " + data;

	std::getline(ifs, data);
	this->console = ICON_MD_RATE_REVIEW "   " + data;

	std::getline(ifs, data);
	this->project_ = data;

	std::getline(ifs, data);
	this->extension = data;

	std::getline(ifs, data);
	this->manage = ICON_FA_ARCHIVE "   " + data;

	std::getline(ifs, data);
	this->window = data;

	std::getline(ifs, data);
	this->layoutTemplates = ICON_MD_DASHBOARD "   " + data;

	std::getline(ifs, data);
	this->clear = ICON_MD_CHECK_BOX_OUTLINE_BLANK "   " + data;

	std::getline(ifs, data);
	this->help = data;

	std::getline(ifs, data);
	this->document = data;

	std::getline(ifs, data);
	this->checkCurrentProject = data;

	std::getline(ifs, data);
	this->cancel = data;

	std::getline(ifs, data);
	this->saveAndOpen = data;

	std::getline(ifs, data);
	this->ignore = data;

	std::getline(ifs, data);
	this->saveAndExit = data;

	std::getline(ifs, data);
	this->exitWithoutSaving = data;

	std::getline(ifs, data);
	this->error_openProjectFile = data;

	std::getline(ifs, data);
	this->error_illegalFile = data;

	std::getline(ifs, data);
	this->error_brokenFile = data;

	std::getline(ifs, data);
	this->error_internal = data;

	std::getline(ifs, data);
	this->confirm_changeLayout = Internal::Text::newLine(ifs, data);

}

FD::Internal::Text::TopBar::TopBar() {
	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::TopBar>(Getter::get()), std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open TopBar.");

	std::string data = "";

	std::getline(ifs, data);
	this->package = data;

	std::getline(ifs, data);
	this->layout = data;

	std::getline(ifs, data);
	this->templates = data;

	std::getline(ifs, data);
	this->tempProject = data;

	std::getline(ifs, data);
	this->save = data;

	std::getline(ifs, data);
	this->newProject = data;

	std::getline(ifs, data);
	this->includeFile = data;

	std::getline(ifs, data);
	this->error_mainfile = data;

}

FD::Internal::Text::LeftBar::LeftBar() {
	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::LeftBar>(Getter::get()), std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open LeftBar.");

	std::string data = "";

	std::getline(ifs, data);
	this->popup_save = Internal::Text::newLine(ifs, data);;

	std::getline(ifs, data);
	this->popup_saveAndClose = data;

	std::getline(ifs, data);
	this->popup_withoutSaving = data;

	std::getline(ifs, data);
	this->popup_cancel = data;
}

FD::Internal::Text::CodingSelect::CodingSelect() {

	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::CodingSelect>(Getter::get()), std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open CodingSelect.");

	std::string data = "";

	std::getline(ifs, data);
	this->selectPlease = data;

	std::getline(ifs, data);
	this->quick = data;

	std::getline(ifs, data);
	this->empty_lua = data;

	std::getline(ifs, data);
	this->empty_py = data;

	std::getline(ifs, data);
	this->empty_as = data;

	std::getline(ifs, data);
	this->openFile = data;

	std::getline(ifs, data);
	this->openFileDescription = data;

	std::getline(ifs, data);
	this->newFile = data;

	std::getline(ifs, data);
	this->newFileDescription = data;

	std::getline(ifs, data);
	this->cancel = data;

	std::getline(ifs, data);
	this->select = data;

	std::getline(ifs, data);
	this->folderPath = data;

	std::getline(ifs, data);
	this->fileName = data;

	std::getline(ifs, data);
	this->error_notFoundDirectory = data;

	std::getline(ifs, data);
	this->error_alreadyExistFile = data;

	std::getline(ifs, data);
	this->error_emptyForm = data;
}

FD::Internal::Text::CodingNew::CodingNew() {
	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::CodingNew>(Getter::get()), std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open CodingNew.");

	std::string data = "";

	std::getline(ifs, data);
	this->title = data;

	std::getline(ifs, data);
	this->recent = data;

	std::getline(ifs, data);
	this->recent_empty = data;

	std::getline(ifs, data);
	this->cancel = data;

	std::getline(ifs, data);
	this->create = data;

	std::getline(ifs, data);
	this->folderPath = data;

	std::getline(ifs, data);
	this->fileName = data;

	std::getline(ifs, data);
	this->empty = data;

	std::getline(ifs, data);
	this->empty_Description = data;

	std::getline(ifs, data);
	this->emptyLua = data;

	std::getline(ifs, data);
	this->emptyLua_Description = data;

	std::getline(ifs, data);
	this->emptyPython = data;

	std::getline(ifs, data);
	this->emptyPython_Description = data;

	std::getline(ifs, data);
	this->emptyAngelScript = data;

	std::getline(ifs, data);
	this->emptyAngelScript_Description = data;

	std::getline(ifs, data);
	this->error_fill = data;

	std::getline(ifs, data);
	this->error_directoryNotFound = data;

	std::getline(ifs, data);
	this->error_fileAlreadyExist = data;
}

FD::Internal::Text::NewProject::NewProject() {

	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::NewProject>(Getter::get()), std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open NewProject.");

	std::string data = "";

	std::getline(ifs, data);
	this->newProject = data;

	std::getline(ifs, data);
	this->search = data;

	std::getline(ifs, data);
	this->filter = data;

	std::getline(ifs, data);
	this->all = data;

	std::getline(ifs, data);
	this->recent = data;

	std::getline(ifs, data);
	this->NotApplicable = data;

	std::getline(ifs, data);
	this->empty = data;

	std::getline(ifs, data);
	this->emptyDescription = data;

	std::getline(ifs, data);
	this->emptyLua = data;

	std::getline(ifs, data);
	this->emptyLuaDescription = data;

	std::getline(ifs, data);
	this->emptyPython = data;

	std::getline(ifs, data);
	this->emptyPythonDescription = data;

	std::getline(ifs, data);
	this->emptyAngelScript = data;

	std::getline(ifs, data);
	this->emptyAngelScriptDescription = data;

	std::getline(ifs, data);
	this->algorithm = data;

	std::getline(ifs, data);
	this->interactive = data;

	std::getline(ifs, data);
	this->interactiveDescription = data;

	std::getline(ifs, data);
	this->cancel = data;

	std::getline(ifs, data);
	this->select = data;

}

FD::Internal::Text::ProjectForm::ProjectForm() {

	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::ProjectForm>(Getter::get()), std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open ProjectForm.");

	std::string data = "";

	std::getline(ifs, data);
	this->fillPlease = data;

	std::getline(ifs, data);
	this->folderPath = data;

	std::getline(ifs, data);
	this->projectName = data;

	std::getline(ifs, data);
	this->alreadyExist = data;

	std::getline(ifs, data);
	this->error = data;

	std::getline(ifs, data);
	this->back = data;

	std::getline(ifs, data);
	this->cancel = data;

	std::getline(ifs, data);
	this->create = data;

	std::getline(ifs, data);
	this->notSaved = data;

	std::getline(ifs, data);
	this->saveAndCreate = data;

	std::getline(ifs, data);
	this->createWithoutSaving = data;

	std::getline(ifs, data);
	this->error_emptyForm = data;

	std::getline(ifs, data);
	this->error_notFoundDirectory = data;

	std::getline(ifs, data);
	this->error_alreadyExist = data;

	std::getline(ifs, data);
	this->error_failedToCreate = data;
}

FD::Internal::Text::ProjectSaveAs::ProjectSaveAs() {

	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::ProjectSaveAs>(Getter::get()), std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open ProjectSaveAs.");

	std::string data = "";

	std::getline(ifs, data);
	this->title = data;

	std::getline(ifs, data);
	this->folderPath = data;

	std::getline(ifs, data);
	this->projectName = data;

	std::getline(ifs, data);
	this->cancel = data;

	std::getline(ifs, data);
	this->save = data;

	std::getline(ifs, data);
	this->checkCurrentProject = data;

	std::getline(ifs, data);
	this->saveAndWrite = data;

	std::getline(ifs, data);
	this->ignore = data;

	std::getline(ifs, data);
	this->error_empty = data;

	std::getline(ifs, data);
	this->error_alreadyExist = data;

	std::getline(ifs, data);
	this->error_notFound = data;

	std::getline(ifs, data);
	this->error_failed = data;
}

FD::Internal::Text::PopupSelect::PopupSelect() {

	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::PopupSelect>(Getter::get()), std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open PopupSelect.");

	std::string data = "";

	std::getline(ifs, data);
	this->yes = data;

	std::getline(ifs, data);
	this->no = data;

	std::getline(ifs, data);
	this->cancel = data;


}

FD::Internal::Text::TextEditor::TextEditor() {

	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::TextEditor>(Getter::get()), std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open TextEditor.");

	std::string data = "";

	std::getline(ifs, data);
	this->editor = data;

	std::getline(ifs, data);
	this->file = data;

	std::getline(ifs, data);
	this->save = data;

	std::getline(ifs, data);
	this->saveAs = data;

	std::getline(ifs, data);
	this->load = data;

	std::getline(ifs, data);
	this->create = data;

	std::getline(ifs, data);
	this->quit = data;

	std::getline(ifs, data);
	this->edit = data;

	std::getline(ifs, data);
	this->readOnly = data;

	std::getline(ifs, data);
	this->undo = data;

	std::getline(ifs, data);
	this->redo = data;

	std::getline(ifs, data);
	this->copy = data;

	std::getline(ifs, data);
	this->cut = data;

	std::getline(ifs, data);
	this->del = data;

	std::getline(ifs, data);
	this->paste = data;

	std::getline(ifs, data);
	this->selectAll = data;

	std::getline(ifs, data);
	this->theme = data;

	std::getline(ifs, data);
	this->default_ = data;

	std::getline(ifs, data);
	this->dark = data;

	std::getline(ifs, data);
	this->light = data;

	std::getline(ifs, data);
	this->blue = data;

	std::getline(ifs, data);
	this->line = data;

	std::getline(ifs, data);
	this->column = data;

}


FD::Internal::Text::CodingTab::CodingTab() {

	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::CodingTab>(Getter::get()), std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open CodingTab.");

	std::string data = "";

	std::getline(ifs, data);
	this->error_limitMaxSize = data;

	std::getline(ifs, data);
	this->error_alreadyExist = data;

	std::getline(ifs, data);
	this->popup_save = data;

	std::getline(ifs, data);
	this->popup_saveAndClose = data;

	std::getline(ifs, data);
	this->popup_withoutSaving = data;

	std::getline(ifs, data);
	this->popup_cancel = data;

}

FD::Internal::Text::Project::Project() {
	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::Project>(Getter::get()), std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open Project.");

	std::string data = "";

	std::getline(ifs, data);
	this->sync = data;

	std::getline(ifs, data);
	this->add = data;

	std::getline(ifs, data);
	this->file = data;

	std::getline(ifs, data);
	this->folder = data;

	std::getline(ifs, data);
	this->rename = data;

	std::getline(ifs, data);
	this->remove = data;

	std::getline(ifs, data);
	this->displayCode = data;

	std::getline(ifs, data);
	this->error_openFile = data;

	std::getline(ifs, data);
	this->notice_removeFile = data;

	std::getline(ifs, data);
	this->notice_removeFolder = data;

	std::getline(ifs, data);
	{
		std::string temp = ICON_MD_IMPORT_CONTACTS " ";
		this->standardFluidumLibrary = (temp + data);
	}

	std::getline(ifs, data);
	this->mainFile = data;

	std::getline(ifs, data);
	this->projectFolder = data;

	std::getline(ifs, data);
	this->project = data;

	std::getline(ifs, data);
	this->user = data;

	std::getline(ifs, data);
	this->mainFileDoesNotExist = data;

	std::getline(ifs, data);
	this->path = data;

	std::getline(ifs, data);
	this->name = data;

	std::getline(ifs, data);
	this->createFolder = data;

	std::getline(ifs, data);
	this->createVirtualFolder = data;

	std::getline(ifs, data);
	this->createFile = data;

	std::getline(ifs, data);
	this->addExistFolder = data;

	std::getline(ifs, data);
	this->addExistFile = data;

	std::getline(ifs, data);
	this->open = data;

	std::getline(ifs, data);
	this->close = data;

	std::getline(ifs, data);
	this->delete_release = data;

	std::getline(ifs, data);
	this->error_maxSize = data;

	std::getline(ifs, data);
	this->confirm_releaseVirtualFolder = Internal::Text::newLine(ifs, data);

	std::getline(ifs, data);
	this->confirm_releaseFile = Internal::Text::newLine(ifs, data);

	std::getline(ifs, data);
	this->confirm_deleteFolder = Internal::Text::newLine(ifs, data);

	std::getline(ifs, data);
	this->confirm_deleteFile = Internal::Text::newLine(ifs, data);

	std::getline(ifs, data);
	this->error_sameName = data;

	std::getline(ifs, data);
	this->error_emptyName = data;

	std::getline(ifs, data);
	this->error_forbiddenCharactor = data;

}


















