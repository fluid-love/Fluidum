#pragma once

#include "../Common/common.h"

namespace FD {
	class ProjectWrite;
	class ProjectFilesWrite;
	class ProjectFilesRead;
}

namespace FD::Project::Internal {
	struct FileData final{
	private:
		static inline std::string mainCodeFilePath{};
		static inline std::vector<std::string> includeCodeFilePathes{};
		static inline std::vector<std::string> codeFilePathes{};

		static inline std::mutex mtx{};
		static inline std::atomic_bool save = false;
	private:
		friend class ProjectWrite;
		friend class ProjectFilesWrite;
		friend class ProjectFilesRead;
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

	class ProjectFilesWrite final {
	public:
		explicit ProjectFilesWrite(Internal::PassKey) {}
		~ProjectFilesWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(ProjectFilesWrite)

	public:
		void setMainCodePath(const std::string& path) const;
		void addIncludeCodeFilePath(const std::string& path) const;
		void addCodeFilePath(const std::string& path) const;

		void unsetMainCodePath() const;
		void eraseIncludeCodeFilePath(const std::string& path) const;
		void eraseCodeFilePath(const std::string& path) const;

		void save() const;

	private:
		std::vector<std::string>::iterator findCodeFile(const std::string& path) const;
		std::vector<std::string>::iterator findIncludeCodeFile(const std::string& path) const;

	};

	class ProjectFilesRead final {
	public:
		explicit ProjectFilesRead(Internal::PassKey) noexcept {}
		~ProjectFilesRead() = default;
		FluidumUtils_Class_Delete_CopyMove(ProjectFilesRead)

	public:

		_NODISCARD std::vector<std::string> includeCodeFilePathes() const;
		_NODISCARD std::string mainCodeFilePath() const;
		_NODISCARD bool isMainCodeFileExist() const;

		_NODISCARD Project::CodeType getCurrentMainCodeType() const;
	};

}