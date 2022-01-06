#pragma once

#include "../Files/supported.h"

namespace FD {

	class ProjectWrite final {
	public:
		//Create a temporary project.
		explicit ProjectWrite(Internal::PassKey) noexcept;
		~ProjectWrite() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(ProjectWrite)

	public:
		struct CreateInfo final {
			std::string projectName{};
			std::string projectDirectoryPath{};
		};

		enum class Exception : UT {
			NotFoundHistory,
			NotFoundDirectory,
			NotFoundProjectFile,
			NotFoundProjectDirectory,
			NotFoundProjectFiles,
			AlreadyProjectDirctoryExist,
			AlreadyExist,
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

	public:
		//strong
		//Create a new project without saving the current one.
		void createNewProject(const CreateInfo& info);

		/*
		Exception:
			Unexpected
			FailedToOpenProjectFile
			std::exception
			std::system_error
		*/
		//strong
		//Load an existing project without saving the current one.
		void loadProject(const std::string& path);

		/*
			NotFoundDirectory
			AlreadyExist,
			NotFoundProjectDirectory
			NotFoundProjectFile
			std::filesystem::error
		*/
		//strong
		//Save current project. The newly saved project replaces the current project.
		void saveAs(const std::string& newName, const std::string& dstProjectDirectoryPath);

		//strong
		//Make a new backup.
		//Delete all backups except the latest one when the application is closed.
		void backup();

		/*
			NotFoundHistory
			std::filesystem::filesystem_error
			std::exception
		*/
		//strong
		void removeHistory(const std::string& fprojPath);

	private:
		/*
			std::system_error(std::filesystem::filesystem_error)
			std::exception
		*/
		//strong
		void save_tab();
		void save_scene();

		void save_files_recursive(std::ofstream& ofs, const Project::FileList::FileInfo* info);
		void save_fluidumFiles();
		void save_projectFiles();
		void save_userFiles();

		void save_layout();

	private:
		void writeProjectInfo(const std::string& path);

	private:
		/*
		Exception:
			std::system_error(std::filesystem::filesystem_error)
			std::exception
		*/
		//no-exception-safety
		//If an exception is thrown, directories will be left as it is.
		void tryCreateFluidumDirectory();

		/*
		Exception:
			NotFoundProjectFiles
			std::system_error(std::filesystem::filesystem_error)
			std::exception
		*/
		//no-exception-safety
		//If an exception is thrown, files will be left as it is.
		void tryCreateFluidumFiles();

	private:
		/*
		Exception:
			NotFoundProjectDirectory
			Unexpected -> if throw std::silesystem_filesystem_error
		*/
		//strong		
		void checkIsProjectFolderExist() const;

		/*
		Exception:
			NotFoundProjectFile
			Unexpected -> if throw std::silesystem_filesystem_error
		*/
		//strong	
		void checkIsProjectFileExist() const;

	private:
		//strong
		void updateHistory();

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

}

namespace FD {

	class ProjectRead final {
	public:
		explicit ProjectRead(Internal::PassKey) noexcept {};
		~ProjectRead() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(ProjectRead)

	public:
		using HistoryInfo = ProjectWrite::HistoryInfo;
		using Exception = ProjectWrite::Exception;

	public:
		/*
		Exception:
			std::exception
			Unexpected
		*/
		//strong
		[[nodiscard]] std::array<HistoryInfo, 50> loadProjectHistory() const;
		[[nodiscard]] std::vector<FU::Class::ClassCode::CodeType> loadSceneFile() const;

	public:
		[[nodiscard]] std::string projectDirectoryPath() const;
		[[nodiscard]] std::string backupDirectoryPath() const;
		[[nodiscard]] std::string srcDirectoryPath() const;
		[[nodiscard]] std::string projectName() const;

	public:
		[[nodiscard]] bool isDefaultProject() const;

	};

}