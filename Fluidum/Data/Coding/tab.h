#pragma once

#include "../Common/common.h"
#include "../../TextEditor/include.h"

//forward
namespace FD {
	namespace Coding {
		class TabWrite;
		class TabRead;
	}
	class ProjectWrite;
}

namespace FD::Internal::Coding {

	class TabData final {
	private:
		static inline std::vector<std::string> filePathes{};
		static inline std::vector<std::string> displayFiles{};
		static inline std::mutex mtx{};

		static inline std::atomic_bool save = false;
	private:
		friend ::FD::Coding::TabWrite;
		friend ::FD::Coding::TabRead;
		friend ::FD::ProjectWrite;
	};


}



namespace FD::Coding {

	//tab ÇΩÇæÇÃçÏã∆èÍ
	class TabWrite final {
	public:
		explicit TabWrite(Internal::PassKey) {}
		~TabWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(TabWrite)

	public:
		//LimitFileSizeMax == 1000
		//AlreadyExist
		//NotFound(file)
		void addFile(const std::string& path) const;

		FTE::TextEditor* getEditor(const std::string& path) const;

		//NotFound
		void eraseFile(const std::string& path) const;

		//NotFound
		void addDisplayFile(const std::string& path) const;
		void eraseDisplayFile(const std::string& path) const;

		void save() const;
		void setIsTextSaved(const std::string& path,const bool val) const;
	
	public:
		void save(const std::string& path) const;
		void saveAll() const;

	public:
		enum class Exception : uint8_t {
			LimitFileSizeMax,
			NotFound,
			AlreadyExist,
		};

	private:
		void update() const;

	};

	//current project
	class TabRead final {
	public:
		explicit TabRead(Internal::PassKey) noexcept {};
		~TabRead() = default;
		FluidumUtils_Class_Delete_CopyMove(TabRead)

	public:

		_NODISCARD bool update() const;
		_NODISCARD bool isDisplayFileChanged() const;

		_NODISCARD std::vector<std::string> getFilePathes() const;
		_NODISCARD std::vector<std::string> getDisplayFilePaths() const;

		_NODISCARD bool update_isTextSaved() const;
		_NODISCARD bool isTextSaved(const std::string& path) const;
		_NODISCARD bool isAllTextSaved() const;

	};

}