#pragma once

#include "../Common/common.h"

namespace FD::Project {
	struct CreateProjectInfo final {
		const char* projectName;
		const char* projectFolderPath;
	};

	enum class ExceptionType {
		NotFoundProjectFolder,
		AlreadyProjectFolderExist,
		FailedToOpenProjectFile,
		IllegalFile,
		BrokenFile
	};

	struct HistoryInfo final {
		std::string projectName{};
		std::string projectFilePath{};
		std::string ymd_h{};
	};

	enum class CodeType : uint8_t {
		Empty,      //main�R�[�h���ݒ肳��Ă��Ȃ�
		Error,      //error ���ʎq�����L�ȊO
		Python,
		Lua,
		AngelScript
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
		void loadExistProject(const char* path) const;

		//���݂̃v���W�F�N�g��ۑ����āC���݂̃v���W�F�N�g��V�����p�X��copy����.
		void saveAs(const char* newName, const char* dstProjectFolderPath) const;

		//���݂̃v���W�F�N�g��ۑ�
		void save() const;

		//���݂̏���V���Ƀo�b�N�A�b�v���Ƃ�
		//�A�v���I�����ɍŐV�̃o�b�N�A�b�v�ȊO�͍폜�����
		void backup() const;

		void setMainCodePath(const char* path) const;
		void addIncludeCodePath(const char* path) const;
		void IncludeCodePath(const char* path) const;

	private:
		void write(const char* path) const;

		void makeBackupFolder() const;
		void makeCodeFolder() const;

		//ProjectFolder�������ꂽ��ړ������肵�Ă��邩
		void checkIsProjectFolderExist() const;

		void updateHistory() const;
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

		_NODISCARD std::vector<std::string> getIncludeCodeFilePathes() const;
		_NODISCARD std::string getMainCodeFilePath() const;

		_NODISCARD std::string getProjectFolderPath() const;
		_NODISCARD std::string getBackupFolderPath() const;
		_NODISCARD std::string getCodeFolderPath() const;
		_NODISCARD std::string getProjectName() const;

		_NODISCARD bool isDefaultProject() const;

		_NODISCARD Project::CodeType getCurrentMainCodeType() const;
	};

}