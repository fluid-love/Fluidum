#include "text.h"

#include "../../../Libraries/IconFontCppHeaders/IconsMaterialDesign.h"

namespace FD::Internal::Text {
	using Language = ::FD::Text::Language;


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

	if (Getter::get() == Language::Japanese)
		ifs = std::ifstream(Internal::Resource::TitleGuiTextJpnFilePath, std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open Title.jpn.");

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
	this->error_openProjectFile = data;

	std::getline(ifs, data);
	this->error_illegalFile = data;

	std::getline(ifs, data);
	this->error_brokenFile = data;

	std::getline(ifs, data);
	this->error_internal = data;


}

FD::Internal::Text::StatusBar::StatusBar() {
	std::ifstream ifs{};

	if (Getter::get() == Language::Japanese)
		ifs = std::ifstream(Internal::Resource::StatusBarGuiTextJpnFilePath, std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open StatusBar.jpn.");

	std::string data = "";

	this->task = ICON_MD_EVENT_NOTE;

	std::getline(ifs, data);
	this->version = data;//Version

	std::getline(ifs, data);
	this->fps = data;//FPS:

	std::getline(ifs, data);
	this->taskInfo = data;//���s���̃^�X�N�͂���܂���D
}

FD::Internal::Text::MenuBar::MenuBar() {
	std::ifstream ifs{};

	if (Getter::get() == Language::Japanese)
		ifs = std::ifstream(Internal::Resource::MenuBarGuiTextJpnFilePath, std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open MenuBar.jpn.");

	std::string data = "";

	std::getline(ifs, data);
	this->file = data;//�t�@�C��

	std::getline(ifs, data);
	this->create = data;//�V�K�쐬

	std::getline(ifs, data);
	this->open = data;//�J��

	std::getline(ifs, data);
	this->save = data;//�ۑ�

	std::getline(ifs, data);
	this->saveFileAs = data;//���O��t���ĕۑ�

	std::getline(ifs, data);
	this->terminate = data;//�I��

	std::getline(ifs, data);
	this->edit = data;//�ҏW

	std::getline(ifs, data);
	this->calc = data;//�v�Z

	std::getline(ifs, data);
	this->run = data;//���s

	std::getline(ifs, data);
	this->runIgnoreStep = data;//�X�e�b�v�Ȃ��Ŏ��s

	std::getline(ifs, data);
	this->calcInfo = data;//���

	std::getline(ifs, data);
	this->view = data;//�\��

	std::getline(ifs, data);
	this->help = data;//�w���v

	std::getline(ifs, data);
	this->credit = data;//�N���W�b�g

}

FD::Internal::Text::TopBar::TopBar() {
	std::ifstream ifs{};

	if (Getter::get() == Language::Japanese)
		ifs = std::ifstream(Internal::Resource::TopBarGuiTextJpnFilePath, std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open TopBar.jpn.");

	std::string data = "";

	std::getline(ifs, data);
	this->package = data;//�p�b�P�[�W

	std::getline(ifs, data);
	this->layout = data;//���C�A�E�g

	std::getline(ifs, data);
	this->templates = data;//�e���v���[�g

	std::getline(ifs, data);
	this->tempProject = data;//���̃v���W�F�N�g

	std::getline(ifs, data);
	this->save = data;

	std::getline(ifs, data);
	this->newProject = data;

	std::getline(ifs, data);
	this->includeFile = data;


}

FD::Internal::Text::CodingSelect::CodingSelect() {

	std::ifstream ifs{};

	if (Getter::get() == Language::Japanese)
		ifs = std::ifstream(Internal::Resource::CodingSelectGuiTextJpnFilePath, std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open CodingSelect.jpn.");

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

FD::Internal::Text::NewProject::NewProject() {

	std::ifstream ifs{};

	if (Getter::get() == Language::Japanese)
		ifs = std::ifstream(Internal::Resource::NewProjectGuiTextJpnFilePath, std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open NewProject.jpn.");

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

	if (Getter::get() == Language::Japanese)
		ifs = std::ifstream(Internal::Resource::ProjectFormGuiTextJpnFilePath, std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open ProjectForm.jpn.");

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

}

FD::Internal::Text::PopupSelect::PopupSelect() {

	std::ifstream ifs{};

	if (Getter::get() == Language::Japanese)
		ifs = std::ifstream(Internal::Resource::PopupSelectGuiTextJpnFilePath, std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open PopupSelect.jpn.");

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

	if (Getter::get() == Language::Japanese)
		ifs = std::ifstream(Internal::Resource::TextEditorGuiTextJpnFilePath, std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open TextEditor.jpn.");

	std::string data = "";

	//std::getline(ifs, data);
	//this->yes = data;

	//std::getline(ifs, data);
	//this->no = data;

	//std::getline(ifs, data);
	//this->cancel = data;


}























