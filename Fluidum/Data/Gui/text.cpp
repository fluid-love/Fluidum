#include "text.h"

#include "../../../External/IconFontCppHeaders/IconsMaterialDesign.h"
#include "../../../External/IconFontCppHeaders/IconsFontAwesome5.h"

#include "../../../External/magic_enum/include/magic_enum.hpp"

#include "Texts/taskmanager.h"

namespace FD::Text::Internal {

	using Language = ::FD::Text::Language;

	[[nodiscard]] std::string newLine(std::ifstream& ifs, const std::string& src) {
		std::string temp{};
		temp.push_back('\n');
		std::getline(ifs, temp);
		return src + '\n' + temp;
	}

	enum class Type : UT {
		Common,
		Analysis,
		BarExit,
		ProjectNewFile,
		ProjectDirectory,
		ProjectCheckPath,
		ProjectSelect,
		ProjectProperty,
		CodingTab,
		Console,
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
		TopBar,
		TaskManager
	};

	template<Type T>
	[[nodiscard]] std::string makePath(const Language lang) {
		using namespace ::FD::Internal::Resource;
		using enum Type;

		std::string path{};
		if (lang == Language::Japanese)
			path += JapaneseGuiTextFolderPath;
		else if (lang == Language::English)
			path += EnglishGuiTextFolderPath;
		else if (lang == Language::Chinese)
			path += ChineseGuiTextFolderPath;
		else {
			::FD::Internal::GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Internal Error.");;
			std::terminate();
		}

		if constexpr (T == Common) {
			path += "Common";
		}
		else if constexpr (T == Analysis) {
			path += "Analysis";
		}
		else if constexpr (T == BarExit) {
			path += "BarExit";
		}
		else if constexpr (T == ProjectNewFile) {
			path += "ProjectNewFile";
		}
		else if constexpr (T == ProjectDirectory) {
			path += "ProjectDirectory";
		}
		else if constexpr (T == ProjectCheckPath) {
			path += "ProjectCheckPath";
		}
		else if constexpr (T == ProjectSelect) {
			path += "ProjectSelect";
		}
		else if constexpr (T == ProjectProperty) {
			path += "ProjectProperty";
		}
		else if constexpr (T == CodingTab) {
			path += "CodingTab";
		}
		else if constexpr (T == Console) {
			path += "Console";
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
		else if constexpr (T == TaskManager) {
			path += "TaskManager";
		}
		else {
			static_assert(FU::Concept::DelayAssert_N<T>);
		}
		return path;
	}

}

namespace FD::Text::Internal {

	//forward declaration
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
		Text::Internal::LangType::type = Text::Language::Japanese;
	else if (data == "English")
		Text::Internal::LangType::type = Text::Language::English;
	else
		throw std::runtime_error("Failed to read GuiTextType.");

}

FD::Text::Internal::Common::Common(const CommonText index)
	: text(init(index))
{}

FD::Text::Internal::GuiText FD::Text::Internal::Common::init(const CommonText index) const {
	using T = std::underlying_type_t<CommonText>;

	const T i = static_cast<T>(index);

	assert(std::numeric_limits<T>::max() != i);
	assert(i < magic_enum::enum_count<CommonText>());

	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::Common>(Getter::get()), std::ios::in);

	if (!ifs) {
		::FD::Internal::GMessenger.add<FU::Log::Type::Warning>(__FILE__, __LINE__, "Failed to open Common file.");;
		return GuiText{ "Error" };
	}

	std::string buf{};

	for (T j = 0; j < i + 1; j++) {
		std::getline(ifs, buf);
	}

	return GuiText{ buf };
}

FD::Text::Internal::Title::Title() {
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
	this->erase_icon = ICON_FA_ERASER "  " + data;

	std::getline(ifs, data);
	this->clear_icon = ICON_FA_ERASER "  " + data;

	std::getline(ifs, data);
	this->confirm_clearHistory = Internal::newLine(ifs, data);

	std::getline(ifs, data);
	this->error_notSupported = data;

	std::getline(ifs, data);
	this->error_loadProjectHistory = data;

	std::getline(ifs, data);
	this->error_openProjectFile = Internal::newLine(ifs, data);

	std::getline(ifs, data);
	this->error_illegalFile = data;

	std::getline(ifs, data);
	this->error_brokenFile = data;

	std::getline(ifs, data);
	this->error_unexpected = data;
}

FD::Text::Internal::TitleBar::TitleBar() {
	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::TitleBar>(Getter::get()), std::ios::in);
	if (!ifs)
		throw std::runtime_error("Failed to open TitleBar.");

	std::string data = "";

	std::getline(ifs, data);
	this->tempProject = data;

}

FD::Text::Internal::Layout::Layout() {
	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::Layout>(Getter::get()), std::ios::in);
	if (!ifs)
		throw std::runtime_error("Failed to open Layout.");

	std::string data = "";

	std::getline(ifs, data);
	this->splitVerticalCurrentPos = ICON_MD_IMPORT_EXPORT "  " + data;

	std::getline(ifs, data);
	this->splitHorizonalCurrentPos = ICON_MD_COMPARE_ARROWS "  " + data;

	std::getline(ifs, data);
	this->splitVerticalCenterLine = ICON_MD_IMPORT_EXPORT "  " + data;

	std::getline(ifs, data);
	this->splitHorizonalCenterLine = ICON_MD_COMPARE_ARROWS "  " + data;

	std::getline(ifs, data);
	this->merge = ICON_MD_HEALING "  " + data;

	std::getline(ifs, data);
	this->reset = ICON_MD_CHECK_BOX_OUTLINE_BLANK "  " + data;

	std::getline(ifs, data);
	this->error_max = data;

	std::getline(ifs, data);
	this->confirm_reset = data;
}

FD::Text::Internal::BarExit::BarExit() {
	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::BarExit>(Getter::get()), std::ios::in);
	if (!ifs)
		throw std::runtime_error("Failed to open BarExit.");

	std::string data = "";

	std::getline(ifs, data);
	this->popup_projectMessage = Internal::newLine(ifs, data);

	std::getline(ifs, data);
	this->popup_codingTabMessage = Internal::newLine(ifs, data);

	std::getline(ifs, data);
	this->popup_saveAndExit = data;

	std::getline(ifs, data);
	this->popup_withoutSaving = data;

	std::getline(ifs, data);
	this->popup_cancel = data;
}

FD::Text::Internal::StatusBar::StatusBar() {
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

FD::Text::Internal::MenuBar::MenuBar() {
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
	this->property_icon = ICON_FA_WRENCH "   " + data;

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
	this->confirm_changeLayout = Internal::newLine(ifs, data);

}

FD::Text::Internal::TopBar::TopBar() {
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
	this->save = data;

	std::getline(ifs, data);
	this->newProject = data;

	std::getline(ifs, data);
	this->includeFile = data;

	std::getline(ifs, data);
	this->run = data;

	std::getline(ifs, data);
	this->error_mainfile = data;

	std::getline(ifs, data);
	this->error_notSetProperty = data;

}

FD::Text::Internal::LeftBar::LeftBar() {
	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::LeftBar>(Getter::get()), std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open LeftBar.");

	std::string data = "";

	std::getline(ifs, data);
	this->popup_save = Internal::newLine(ifs, data);;

	std::getline(ifs, data);
	this->popup_saveAndClose = data;

	std::getline(ifs, data);
	this->popup_withoutSaving = data;

	std::getline(ifs, data);
	this->popup_cancel = data;
}

FD::Text::Internal::ProjectSelect::ProjectSelect() {

	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::ProjectSelect>(Getter::get()), std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open ProjectSelect.");

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
	this->empty_cpp = data;

	std::getline(ifs, data);
	this->openFile = data;

	std::getline(ifs, data);
	this->openFileDescription = data;

	std::getline(ifs, data);
	this->newFile = data;

	std::getline(ifs, data);
	this->newFileDescription = data;

	std::getline(ifs, data);
	this->newDirectory = data;

	std::getline(ifs, data);
	this->newDirectoryDescription = data;

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

	std::getline(ifs, data);
	this->error_forbiddenCharactor = data;

	std::getline(ifs, data);
	this->error_unexpected = data;

}

FD::Text::Internal::ProjectNewFile::ProjectNewFile() {
	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::ProjectNewFile>(Getter::get()), std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open ProjectNewFile.");

	std::string data = "";

	std::getline(ifs, data);
	this->title = data;

	std::getline(ifs, data);
	this->search = data;

	std::getline(ifs, data);
	this->all = data;

	std::getline(ifs, data);
	this->recent = data;

	std::getline(ifs, data);
	this->recentEmpty = data;

	std::getline(ifs, data);
	this->back = data;

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
	this->empty_description = data;

	std::getline(ifs, data);
	this->emptyLua = data;

	std::getline(ifs, data);
	this->emptyLua_description = data;

	std::getline(ifs, data);
	this->emptyPython = data;

	std::getline(ifs, data);
	this->emptyPython_description = data;

	std::getline(ifs, data);
	this->emptyCpp = data;

	std::getline(ifs, data);
	this->emptyCpp_description = data;

	std::getline(ifs, data);
	this->error_unexpected = data;
}

FD::Text::Internal::ProjectDirectory::ProjectDirectory() {
	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::ProjectDirectory>(Getter::get()), std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open ProjectDirectory.");

	std::string data = "";

	std::getline(ifs, data);
	this->title = data;

	std::getline(ifs, data);
	this->parent = data;

	std::getline(ifs, data);
	this->name = data;

	std::getline(ifs, data);
	this->back = data;

	std::getline(ifs, data);
	this->cancel = data;

	std::getline(ifs, data);
	this->create = data;

	std::getline(ifs, data);
	this->error_unexpected = data;
}

FD::Text::Internal::ProjectCheckPath::ProjectCheckPath() {

	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::ProjectCheckPath>(Getter::get()), std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open ProjectCheckPath.");

	std::string data = "";

	std::getline(ifs, data);
	this->error_fill = data;

	std::getline(ifs, data);
	this->error_parentDoesNotExist = data;

	std::getline(ifs, data);
	this->error_directoryAlreadyExist = data;

	std::getline(ifs, data);
	this->error_fileAlreadyExist = data;

	std::getline(ifs, data);
	this->error_forbiddenCharactor = data;

	std::getline(ifs, data);
	this->error_unexpected = data;
}

FD::Text::Internal::NewProject::NewProject() {

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
	this->emptyCpp = data;

	std::getline(ifs, data);
	this->emptyCppDescription = data;

	std::getline(ifs, data);
	this->algorithm = data;

	std::getline(ifs, data);
	this->cancel = data;

	std::getline(ifs, data);
	this->select = data;

	std::getline(ifs, data);
	this->recent_erase = ICON_MD_DELETE "  " + data;

	std::getline(ifs, data);
	this->recent_clear = ICON_MD_DELETE_FOREVER "  " + data;

	std::getline(ifs, data);
	this->recent_message = data;

}

FD::Text::Internal::ProjectForm::ProjectForm() {

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
	this->error_maxSize = data;

	std::getline(ifs, data);
	this->error_absolute = data;

	std::getline(ifs, data);
	this->error_notFoundDirectory = data;

	std::getline(ifs, data);
	this->error_alreadyExist = data;

	std::getline(ifs, data);
	this->error_forbiddenCharactor = data;

	std::getline(ifs, data);
	this->error_failedToCreate = data;

}

FD::Text::Internal::ProjectSaveAs::ProjectSaveAs() {

	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::ProjectSaveAs>(Getter::get()), std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open ProjectSaveAs.");

	std::string data = "";

	std::getline(ifs, data);
	this->title = data;

	std::getline(ifs, data);
	this->directoryPath = data;

	std::getline(ifs, data);
	this->projectName = data;

	std::getline(ifs, data);
	this->cancel = data;

	std::getline(ifs, data);
	this->save = data;

	std::getline(ifs, data);
	this->confirm_notSaved = data;

	std::getline(ifs, data);
	this->confirm_save = data;

	std::getline(ifs, data);
	this->confirm_ignore = data;

	std::getline(ifs, data);
	this->error_empty = data;

	std::getline(ifs, data);
	this->error_alreadyExist = data;

	std::getline(ifs, data);
	this->error_notFoundProject = data;

	std::getline(ifs, data);
	this->error_notFound = data;

	std::getline(ifs, data);
	this->error_forbiddenCharactor = data;

}

FD::Text::Internal::PopupSelect::PopupSelect() {

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

FD::Text::Internal::TextEditor::TextEditor() {

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
	this->save_icon = ICON_FA_SAVE "  " + data;

	std::getline(ifs, data);
	this->saveAs = ICON_FA_SAVE "  " + data;

	std::getline(ifs, data);
	this->load = data;

	std::getline(ifs, data);
	this->create = data;

	std::getline(ifs, data);
	this->quit = data;

	std::getline(ifs, data);
	this->edit = data;

	std::getline(ifs, data);
	this->readOnly = ICON_FA_BOOK_OPEN "  " + data;

	std::getline(ifs, data);
	this->undo = data;
	this->undo_icon = ICON_FA_UNDO "  " + data;

	std::getline(ifs, data);
	this->redo = data;
	this->redo_icon = ICON_FA_REDO "  " + data;

	std::getline(ifs, data);
	this->copy = ICON_FA_CLIPBOARD "  " + data;

	std::getline(ifs, data);
	this->cut = ICON_FA_CUT "  " + data;

	std::getline(ifs, data);
	this->del = ICON_FA_ERASER "  " + data;

	std::getline(ifs, data);
	this->paste = ICON_FA_CLIPBOARD_LIST "  " + data;

	std::getline(ifs, data);
	this->selectAll = ICON_FA_GRIP_LINES "  " + data;

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

	std::getline(ifs, data);
	this->error_forbiddenCharactor = data;

}


FD::Text::Internal::CodingTab::CodingTab() {

	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::CodingTab>(Getter::get()), std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open CodingTab.");

	std::string data = "";

	std::getline(ifs, data);
	this->tab = data;

	std::getline(ifs, data);
	this->tooltip_saveSelectedFile = data;

	std::getline(ifs, data);
	this->tooltip_saveAllFiles = data;

	std::getline(ifs, data);
	this->popup_save = data;

	std::getline(ifs, data);
	this->popup_saveAndClose = data;

	std::getline(ifs, data);
	this->popup_withoutSaving = data;

	std::getline(ifs, data);
	this->popup_cancel = data;

	std::getline(ifs, data);
	this->error_limitMaxSize = data;

	std::getline(ifs, data);
	this->error_alreadyExist = data;

	std::getline(ifs, data);
	this->error_notExist = data;

	std::getline(ifs, data);
	this->error_unexpected = data;

}

FD::Text::Internal::Project::Project() {
	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::Project>(Getter::get()), std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open Project.");

	std::string data = "";

	std::getline(ifs, data);
	this->explorer = data;

	std::getline(ifs, data);
	this->sync = data;

	std::getline(ifs, data);
	this->add = data;
	this->add_icon = ICON_MD_ADD "  " + data;

	std::getline(ifs, data);
	this->add_selectNewFile = ICON_MD_SUBDIRECTORY_ARROW_RIGHT ICON_FA_FILE_ALT "  " + data;

	std::getline(ifs, data);
	this->add_select = ICON_FA_LIST "  " + data;

	std::getline(ifs, data);
	this->file = data;
	this->file_icon = ICON_FA_FILE "  " + data;

	std::getline(ifs, data);
	this->directory = data;
	this->directory_icon = ICON_FA_FOLDER "  " + data;

	std::getline(ifs, data);
	this->rename = ICON_FA_PEN "  " + data;

	std::getline(ifs, data);
	this->displayCode = ICON_MD_CODE "  " + data;

	std::getline(ifs, data);
	this->error_openFile = data;

	std::getline(ifs, data);
	this->notice_removeFile = data;

	std::getline(ifs, data);
	this->notice_removeDirectory = data;

	std::getline(ifs, data);
	{
		std::string temp = ICON_MD_IMPORT_CONTACTS " ";
		this->standardFluidumLibrary = (temp + data);
	}

	std::getline(ifs, data);
	this->mainFile = data;

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
	this->virtualFolder = ICON_FA_FOLDER "  " + data;

	std::getline(ifs, data);
	this->newFile = ICON_FA_FILE "  " + data;

	std::getline(ifs, data);
	this->collapseAll = ICON_FA_FOLDER_MINUS "  " + data;

	std::getline(ifs, data);
	this->addExistDirectory = data;

	std::getline(ifs, data);
	this->addExistFile = data;

	std::getline(ifs, data);
	this->open = ICON_FA_FOLDER_OPEN "  " + data;

	std::getline(ifs, data);
	this->close = ICON_FA_FOLDER "  " + data;

	std::getline(ifs, data);
	this->remove = ICON_FA_TRASH_ALT "  " + data;

	std::getline(ifs, data);
	this->release = ICON_FA_TRASH_ALT "  " + data;

	std::getline(ifs, data);
	this->setAsMainFile = ICON_FA_FLAG "  " + data;

	std::getline(ifs, data);
	this->error_maxSize = data;

	std::getline(ifs, data);
	this->confirm_releaseVirtualFolder = Internal::newLine(ifs, data);

	std::getline(ifs, data);
	this->confirm_releaseFile = Internal::newLine(ifs, data);

	std::getline(ifs, data);
	this->confirm_removeDirectory = Internal::newLine(ifs, data);

	std::getline(ifs, data);
	this->confirm_removeFile = Internal::newLine(ifs, data);

	std::getline(ifs, data);
	this->confirm_releaseFile_notSaved = Internal::newLine(ifs, data);

	std::getline(ifs, data);
	this->error_sameName = data;

	std::getline(ifs, data);
	this->error_emptyName = data;

	std::getline(ifs, data);
	this->error_forbiddenCharactor = data;

	std::getline(ifs, data);
	this->error_addDirectory = data;

	std::getline(ifs, data);
	this->error_addFile = data;

	std::getline(ifs, data);
	this->error_removeDirectory = data;

	std::getline(ifs, data);
	this->error_removeFile = data;

	std::getline(ifs, data);
	this->error_releaseVirtualFolder = data;

	std::getline(ifs, data);
	this->error_releaseFile = data;

	std::getline(ifs, data);
	this->error_tabFileSize = data;

	std::getline(ifs, data);
	this->error_existFile = data;

	std::getline(ifs, data);
	this->error_hugeFile = data;

	std::getline(ifs, data);
	this->error_changeName = data;

	std::getline(ifs, data);
	this->error_tab = data;

	std::getline(ifs, data);
	this->error_fileDoesNotExist = Internal::newLine(ifs, data);

	std::getline(ifs, data);
	this->error_unexpected = data;

}

FD::Text::Internal::Console::Console() {
	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::Console>(Getter::get()), std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open Console.");

	std::string data = "";

	std::getline(ifs, data);
	this->console = data;

	std::getline(ifs, data);
	this->clear = data;

	std::getline(ifs, data);
	this->backcolor = data;

}

FD::Text::Internal::ProjectProperty::ProjectProperty() {
	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::ProjectProperty>(Getter::get()), std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open ProjectProperty.");

	std::string data = "";

	std::getline(ifs, data);
	this->projectProperty = data;

	std::getline(ifs, data);
	this->tab_main = data;

	std::getline(ifs, data);
	this->currentType = data;

	std::getline(ifs, data);
	this->change = data;

	std::getline(ifs, data);
	this->entryFilePath = data;

	std::getline(ifs, data);
	this->luaVersion = data;

	std::getline(ifs, data);
	this->currentVersion = data;

	std::getline(ifs, data);
	this->confirm_changeProjectType = data;

	std::getline(ifs, data);
	this->confirm_notSaved = data;

	std::getline(ifs, data);
	this->confirm_save = data;

	std::getline(ifs, data);
	this->confirm_ignore = data;

	std::getline(ifs, data);
	this->info_currentType = data;

	std::getline(ifs, data);
	this->info_entryFilePath = Internal::newLine(ifs, data);

	std::getline(ifs, data);
	this->info_luaVersion = Internal::newLine(ifs, data);;

	std::getline(ifs, data);
	this->bottom_close = data;

	std::getline(ifs, data);
	this->bottom_cancel = data;

	std::getline(ifs, data);
	this->bottom_save = data;

}

FD::Text::Internal::TaskManager::TaskManager() {
	std::ifstream ifs{};

	ifs = std::ifstream(makePath<Type::TaskManager>(Getter::get()), std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open TaskManager.");

	std::string data = "";

	std::getline(ifs, data);
	this->taskManager = data;
	
	std::getline(ifs, data);
	this->cpuLoad = data;

	std::getline(ifs, data);
	this->numOfPhysicalProcessor = data;

	std::getline(ifs, data);
	this->numOfLogicalProcessor = data;

	std::getline(ifs, data);
	this->cpuArchitecture = data;

}












