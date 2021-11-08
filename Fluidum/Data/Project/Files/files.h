#pragma once

#include "container.h"

namespace FD {
	class ProjectWrite;

	class LuaFilesWrite;
	class LuaFilesRead;

	class FluidumFilesWrite;
	class FluidumFilesRead;

	class ProjectFilesWrite;
	class ProjectFilesRead;

	class UserFilesWrite;
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
		friend class LuaFilesWrite;
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
		friend class ProjectFilesWrite;
		friend class ProjectFilesRead;
	};

	struct UserFilesData final {
	private:
		static inline FileList userFiles{};

		static inline std::mutex mtx{};
		static inline std::atomic_bool save = false;
	private:
		friend class ProjectWrite;
		friend class UserFilesWrite;
		friend class UserFilesRead;
	};
}

namespace FD::Project {
	enum class CodeType : uint8_t {
		Empty,      //mainコードが設定されていない
		Error,      //error 識別子が下記以外
		Python,
		Lua,
		AngelScript
	};
}

namespace FD {

	class LuaFilesWrite final {
	public:
		explicit LuaFilesWrite(Internal::PassKey) {}
		~LuaFilesWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(LuaFilesWrite)

	public:
		void closeAll();

	public:
		_NODISCARD std::vector<Project::List::FileInfo>* fileList();

	public:
		_NODISCARD std::unique_lock<std::mutex> getLock();

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

		_NODISCARD std::string mainCodeFilePath() const;
		_NODISCARD bool isMainCodeFileExist() const;

		_NODISCARD Project::CodeType getCurrentMainCodeType() const;

	};

}

namespace FD {

	//lock
	class ProjectFilesWrite final {
	public:
		explicit ProjectFilesWrite(Internal::PassKey) {}
		~ProjectFilesWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(ProjectFilesWrite)

	public:
		template<Project::Internal::IsFileInfoElm T>
		Project::List::FileInfo* addFile(const std::string& parent, const std::string& path, const T& info) {
			using namespace Project::Internal;
			return ProjectFilesData::projectFiles.add(parent, path, info);
		}

		void changeName(const std::string& path, const std::string& newName);

		void eraseFile(const std::string& path);

		void sync(const std::string& top);

		//.open = false
		void closeAll();

	public:
		_NODISCARD std::vector<Project::List::FileInfo>* fileList();

	public:
		_NODISCARD std::unique_lock<std::mutex> getLock();

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

	};

}

namespace FD {

	//lock
	class UserFilesWrite final {
	public:
		explicit UserFilesWrite(Internal::PassKey) {}
		~UserFilesWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(UserFilesWrite)

	public:
		template<Project::Internal::IsFileInfoElm T>
		Project::List::FileInfo* addFile(const std::string& parent, const std::string& path, const T& info) {
			using namespace Project::Internal;
			return UserFilesData::userFiles.add(parent, path, info);
		}

		//already exists(in the same directory) -> return false

		bool tryChangeName(const std::string& path, const std::string& newName);

		void eraseFile(const std::string& path);

		_NODISCARD std::string makeTempName();

		void closeAll();

		//not exist -> .exist = false
		void sync();

	public:
		_NODISCARD std::vector<Project::List::FileInfo>* fileList();

	public:
		_NODISCARD std::unique_lock<std::mutex> getLock();

	public:
		void save() const;

	};

	class UserFilesRead final {
	public:
		explicit UserFilesRead(Internal::PassKey) noexcept {}
		~UserFilesRead() = default;
		FluidumUtils_Class_Delete_CopyMove(UserFilesRead)

	public:


	};

}