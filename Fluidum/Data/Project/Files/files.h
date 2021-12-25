#pragma once

#include "supported.h"

namespace FD {
	class ProjectWrite;

	class LuaFilesWrite_Lock;
	class LuaFilesRead;

	class FluidumFilesWrite;
	class FluidumFilesRead;

	class ProjectFilesWrite_Lock;
	class ProjectFilesRead;

	class UserFilesWrite_Lock;
	class UserFilesRead;

}

namespace FD::Project::Internal {

	struct LibraryFilesData final {
	private:
		static inline FileList luaLibraries{};

		static inline std::mutex mtx{};
		static inline std::atomic_bool save = false;
	private:
		friend class ProjectWrite;
		friend class LuaFilesWrite_Lock;
		friend class LuaFilesRead;
	};

	struct FluidumFilesData final {
	private:
		static inline std::string mainCodeFilePath{};

		static inline FileList luaLibraries{};

		static inline std::mutex mtx{};
		static inline std::atomic_bool save = false;
	private:
		friend class ProjectWrite;
		friend class FluidumFilesWrite;
		friend class FluidumFilesRead;
	};

	struct ProjectFilesData final {
	private:
		static inline FileList projectFiles{};

		static inline std::mutex mtx{};
		static inline std::atomic_bool save = false;
	private:
		friend class ProjectWrite;
		friend class ProjectFilesWrite_Lock;
		friend class ProjectFilesRead;
	};

	struct UserFilesData final {
	private:
		static inline FileList userFiles{};

		static inline std::mutex mtx{};
		static inline std::atomic_bool save = false;
	private:
		friend class ProjectWrite;
		friend class UserFilesWrite_Lock;
		friend class UserFilesRead;
	};
}

namespace FD {

	class LuaFilesWrite_Lock final {
	public:
		explicit LuaFilesWrite_Lock(Internal::PassKey) {}
		~LuaFilesWrite_Lock() = default;
		FluidumUtils_Class_Delete_CopyMove(LuaFilesWrite_Lock)

	public:
		void collapseAll();

	public:
		[[nodiscard]] std::vector<Project::FileList::FileInfo>* fileList();

	public:
		[[nodiscard]] std::unique_lock<std::mutex> getLock();

	public:
		void save() const;
	};


	class LuaFilesRead final {
	public:
		explicit LuaFilesRead(Internal::PassKey) {}
		~LuaFilesRead() = default;
		FluidumUtils_Class_Delete_CopyMove(LuaFilesRead)


	};
}

namespace FD {

	class FluidumFilesWrite final {
	public:
		explicit FluidumFilesWrite(Internal::PassKey) {}
		~FluidumFilesWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(FluidumFilesWrite)

	public:
		void setMainCodePath(const std::string& path) const;
		void unsetMainCodePath() const;

	public:
		void save() const;

	};

	class FluidumFilesRead final {
	public:
		explicit FluidumFilesRead(Internal::PassKey) noexcept {}
		~FluidumFilesRead() = default;
		FluidumUtils_Class_Delete_CopyMove(FluidumFilesRead)

	public:

		[[nodiscard]] std::string mainCodeFilePath() const;
		[[nodiscard]] bool isMainCodeFileExist() const;

		[[nodiscard]] Project::File::SupportedFileType getCurrentMainCodeType() const;

	};

}

namespace FD {

	//lock
	class ProjectFilesWrite_Lock final {
	public:
		explicit ProjectFilesWrite_Lock(Internal::PassKey) {}
		~ProjectFilesWrite_Lock() = default;
		FluidumUtils_Class_Delete_CopyMove(ProjectFilesWrite_Lock)

	public:
		template<Project::Internal::IsFileInfoElm T>
		Project::FileList::FileInfo* add(const std::string& parent, const std::string& path, const T& info) {
			using namespace Project::Internal;
			return ProjectFilesData::projectFiles.add(parent, path, info);
		}

		//already exists(in the same directory) -> return false
		bool tryChangeName(const std::string& path, const std::string& newName);

		void remove(const std::string& path);

		void sync(const std::string& top);

		//.open = false
		void collapseAll();

	public:
		[[nodiscard]] std::vector<Project::FileList::FileInfo>* fileList();

	public:
		[[nodiscard]] std::unique_lock<std::mutex> getLock();

	public:
		void save() const;

	private:
		std::vector<std::string> findOpenPaths() const;

	};

	class ProjectFilesRead final {
	public:
		explicit ProjectFilesRead(Internal::PassKey) noexcept {}
		~ProjectFilesRead() = default;
		FluidumUtils_Class_Delete_CopyMove(ProjectFilesRead)

	public:
		/*
		return 1

		parent -> child.txt
			   |
			   -> child2.txt
			   |
			   -> child_dir -> ...
							|
							-> ...
		*/
		[[nodiscard]] std::size_t numOfDirectories(const std::string& parent) const;

		/*
		return 2

		parent -> child.txt
			   |
			   -> child2.txt
			   |
			   -> child_dir -> ...
							|
							-> ...
		*/
		[[nodiscard]] std::size_t numOfFiles(const std::string& parent) const;


	};

}

namespace FD {

	//lock
	class UserFilesWrite_Lock final {
	public:
		explicit UserFilesWrite_Lock(Internal::PassKey) {}
		~UserFilesWrite_Lock() = default;
		FluidumUtils_Class_Delete_CopyMove(UserFilesWrite_Lock)

	public:
		template<Project::Internal::IsFileInfoElm T>
		Project::FileList::FileInfo* add(const std::string& parent, const std::string& path, const T& info) {
			using namespace Project::Internal;
			return UserFilesData::userFiles.add(parent, path, info);
		}

		//already exists(in the same directory) -> return false
		bool tryChangeName(const std::string& path, const std::string& newName);

		void remove(const std::string& path);

		void collapseAll();

		//not exist -> .exist = false
		void sync();

	public:
		[[nodiscard]] std::vector<Project::FileList::FileInfo>* fileList();

	public:
		[[nodiscard]] std::unique_lock<std::mutex> getLock();

	public:
		void save() const;

	};

	class UserFilesRead final {
	public:
		explicit UserFilesRead(Internal::PassKey) noexcept {}
		~UserFilesRead() = default;
		FluidumUtils_Class_Delete_CopyMove(UserFilesRead)

	public:
		//see ProjectFilesRead::numOfDirectories and ProjectFilesRead::numOfFiles
		[[nodiscard]] std::size_t numOfVirtualFolder(const std::string& parent) const;
		[[nodiscard]] std::size_t numOfFiles(const std::string& parent) const;

	};

}