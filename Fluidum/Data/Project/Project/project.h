#pragma once

#include "../Files/supported.h"

namespace FD::Project {
	struct CreateProjectInfo final {
		const char* projectName;
		const char* projectFolderPath;
	};

	enum class ExceptionType {
		NotFoundProjectFile,
		NotFoundProjectFolder,
		NotFoundProjectFiles,
		AlreadyProjectFolderExist,
		FailedToOpenProjectFile,
		IllegalFile,
		BrokenFile,
		Unexpected//e.g. when std::basic_string<char>::operator=() throw exception. 
	};

	struct HistoryInfo final {
		std::string projectName{};
		std::string projectFilePath{};
		std::string ymd_h{};
	};
}

namespace FD {

	//���݂̃v���W�F�N�g
	//�N�����̃v���W�F�N�g�͏���ɍ�邪�ۑ����Ȃ��Ɣj��
	class ProjectWrite final {
	public:
		//�f�t�H���g�ŉ��̃v���W�F�N�g���쐬����
		explicit ProjectWrite(Internal::PassKey);
		~ProjectWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(ProjectWrite)


	public:
		//���݂̃v���W�F�N�g��ۑ������ɁC�V�����v���W�F�N�g���쐬����D
		void createNewProject(const Project::CreateProjectInfo& info) const;

		//���݂̃v���W�F�N�g��ۑ������ɁC�����̃v���W�F�N�g��ǂݍ��ށD
		void loadProject(const char* path) const;

		//���݂̃v���W�F�N�g��ۑ����āC���݂̃v���W�F�N�g��V�����p�X��copy����.
		void saveAs(const char* newName, const char* dstProjectFolderPath) const;

		//���݂̏���V���Ƀo�b�N�A�b�v���Ƃ�
		//�A�v���I�����ɍŐV�̃o�b�N�A�b�v�ȊO�͍폜�����
		void backup() const;

		bool eraseProjectHistory(const std::string& fprojPath);

	private:
		void save_tab() const;
		void save_scene() const;

	private:
		void save_files_recursive(std::ofstream& ofs, const Project::FileList::FileInfo* info) const;
		
		void save_fluidumFiles() const;
		void save_projectFiles() const;
		void save_userFiles() const;
		
		void save_layout() const;

	private:
		void writeProjectInfo(const char* path) const;

	private:
		void tryCreateFluidumFolder() const;
		void tryCreateFluidumFiles() const;

	private:
		//ProjectFolder�������ꂽ��ړ������肵�Ă��邩
		void checkIsProjectFolderExist() const;
		void checkIsProjectFileExist() const;


		void updateHistory() const;

	private://read
		void readProjectInfo(std::ifstream& ifs) const;

		Project::FileList::FileInfo readFiles_element(std::ifstream& ifs) const;
	
		void read_files_recursive(std::ifstream& ifs, Project::FileList::FileInfo* parent) const;
		void read_fluidumFiles() const;
		void read_projectFiles() const;
		void read_userFiles() const;

		void read_layout() const;

		void read_tab() const;
		void read_scene() const;

	private:
		void save_thread();
		const std::jthread saveThread{ &ProjectWrite::save_thread,this };
	};

	//���݂̃v���W�F�N�g
	class ProjectRead final {
	public:
		explicit ProjectRead(Internal::PassKey) noexcept {};
		~ProjectRead() = default;
		FluidumUtils_Class_Delete_CopyMove(ProjectRead)

	public:
		_NODISCARD std::array<Project::HistoryInfo, 50> getProjectHistory() const;

		//�ۑ�����Ă��Ȃ�->true
		_NODISCARD bool isDataChanged() const;

		_NODISCARD std::string getProjectFolderPath() const;
		_NODISCARD std::string getBackupFolderPath() const;
		_NODISCARD std::string getSrcFolderPath() const;
		_NODISCARD std::string getProjectName() const;

		_NODISCARD bool isDefaultProject() const;

		_NODISCARD std::vector<FU::Class::ClassCode::CodeType> loadSceneFile() const;
	};

}