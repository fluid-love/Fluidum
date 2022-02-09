#pragma once

#include "supported.h"

namespace FD {

	class ProjectWrite;

	class ProjectFilesWrite_Lock;
	class ProjectFilesRead_Lock;

	class UserFilesWrite_Lock;
	class UserFilesRead_Lock;
	class UserFilesRead;

}

namespace FD::Project::Internal {

	struct ProjectFilesData final {
	private:
		FluidumUtils_Class_Delete_ConDestructor(ProjectFilesData);

	private:
		static inline FileList projectFiles{};

	private:
		static inline std::mutex mtx{};
		static inline std::atomic_bool save = false;

	private:
		friend class ProjectWrite;
		friend class ProjectFilesWrite_Lock;
		friend class ProjectFilesRead_Lock;

	};

	struct UserFilesData final {
	private:
		FluidumUtils_Class_Delete_ConDestructor(UserFilesData);

	private:
		static inline FileList userFiles{};

	private:
		static inline std::mutex mtx{};
		static inline std::atomic_bool save = false;

	private:
		friend class ProjectWrite;
		friend class UserFilesWrite_Lock;
		friend class UserFilesRead_Lock;
		friend class UserFilesRead;

	};

}

namespace FD {

	//lock
	class ProjectFilesWrite_Lock final {
	public:
		explicit ProjectFilesWrite_Lock(Internal::PassKey) {}
		~ProjectFilesWrite_Lock() = default;
		FluidumUtils_Class_Delete_CopyMove(ProjectFilesWrite_Lock);

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

	class ProjectFilesRead_Lock final {
	public:
		explicit ProjectFilesRead_Lock(Internal::PassKey) noexcept {}
		~ProjectFilesRead_Lock() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(ProjectFilesRead_Lock);

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
		[[nodiscard]] Size numOfDirectories(const std::string& parent) const;

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
		[[nodiscard]] Size numOfFiles(const std::string& parent) const;

	public:
		/*
		return:
			first  : exists
			second : depth
		*/
		[[nodiscard]] std::pair<bool, Size> childExists(const std::string& parent, const std::string& child) const;

		/*
		return:
			not exists -> nullptr
		*/
		[[nodiscard]] static Project::FileList::FileInfo* exists(const std::string& path);

	};

	class ProjectFilesRead final {
	public:
		explicit ProjectFilesRead(Internal::PassKey) noexcept {}
		~ProjectFilesRead() = default;
		FluidumUtils_Class_Delete_CopyMove(ProjectFilesRead);

	public:

	};
}

namespace FD {

	//lock
	class UserFilesWrite_Lock final {
	public:
		explicit UserFilesWrite_Lock(Internal::PassKey) {}
		~UserFilesWrite_Lock() = default;
		FluidumUtils_Class_Delete_CopyMove(UserFilesWrite_Lock);

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

	class UserFilesRead_Lock final {
	public:
		explicit UserFilesRead_Lock(Internal::PassKey) noexcept {}
		~UserFilesRead_Lock() = default;
		FluidumUtils_Class_Delete_CopyMove(UserFilesRead_Lock);

	public:
		//see ProjectFilesRead_Lock::numOfDirectories and ProjectFilesRead_Lock::numOfFiles
		[[nodiscard]] std::size_t numOfVirtualFolder(const std::string& parent) const;
		[[nodiscard]] std::size_t numOfFiles(const std::string& parent) const;

	public:
		//return "Fluidum:/"
		[[nodiscard]] std::string rootDirectory() const;

	public:
		[[nodiscard]] bool containForbiddenCharactor(const std::string& name) const;

		[[nodiscard]] std::string finalName(const std::string& name) const;

	public:
		[[nodiscard]] bool exist(const std::string& path) const;
	};

	class UserFilesRead final {
	public:
		explicit UserFilesRead(Internal::PassKey) noexcept {}
		~UserFilesRead() = default;
		FluidumUtils_Class_Delete_CopyMove(UserFilesRead);

	public:
		//return "Fluidum:/"
		[[nodiscard]] std::string rootDirectory() const;

		[[nodiscard]] bool exist(const std::string& path) const;

	public:
		//ng "." ".." "/" "a/a"
		//ok ".a" "..a" 
		[[nodiscard]] bool containForbiddenCharactor(const std::string& name) const;

		[[nodiscard]] std::string finalName(const std::string& name) const;

	public:
		/*
		*
		/./a  -> /a
		/a/b/../ -> /a

		relative -> absolute

		e.g.
			Fluidum:/a/./b  -> Fluidum:/a/b
			Fluidum:/a/../b -> Fluidum:/b
		*/
		//relative
		[[nodiscard]] std::string canonical(const std::string& current, const std::string& path) const;
		//absolute
		[[nodiscard]] std::string canonical(const std::string& path) const;

	};

}