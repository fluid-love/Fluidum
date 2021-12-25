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

	//現在のプロジェクト
	//起動時のプロジェクトは勝手に作るが保存しないと破棄
	class ProjectWrite final {
	public:
		//デフォルトで仮のプロジェクトを作成する
		explicit ProjectWrite(Internal::PassKey);
		~ProjectWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(ProjectWrite)


	public:
		//現在のプロジェクトを保存せずに，新しいプロジェクトを作成する．
		void createNewProject(const Project::CreateProjectInfo& info) const;

		//現在のプロジェクトを保存せずに，既存のプロジェクトを読み込む．
		void loadProject(const char* path) const;

		//現在のプロジェクトを保存して，現在のプロジェクトを新しいパスへcopyする.
		void saveAs(const char* newName, const char* dstProjectFolderPath) const;

		//現在の情報を新たにバックアップをとる
		//アプリ終了時に最新のバックアップ以外は削除される
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
		//ProjectFolderが消されたり移動したりしているか
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

	//現在のプロジェクト
	class ProjectRead final {
	public:
		explicit ProjectRead(Internal::PassKey) noexcept {};
		~ProjectRead() = default;
		FluidumUtils_Class_Delete_CopyMove(ProjectRead)

	public:
		_NODISCARD std::array<Project::HistoryInfo, 50> getProjectHistory() const;

		//保存されていない->true
		_NODISCARD bool isDataChanged() const;

		_NODISCARD std::string getProjectFolderPath() const;
		_NODISCARD std::string getBackupFolderPath() const;
		_NODISCARD std::string getSrcFolderPath() const;
		_NODISCARD std::string getProjectName() const;

		_NODISCARD bool isDefaultProject() const;

		_NODISCARD std::vector<FU::Class::ClassCode::CodeType> loadSceneFile() const;
	};

}