#pragma once

#include "../Common/common.h"

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
		static inline std::string displayFile{};
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
		void addFile(const char* path) const;

		void temp(const std::string& path, std::string&& code) const;
		std::string getTemp(const std::string& path) const;

		//NotFound
		void eraseFile(const char* path) const;

		//NotFound
		void setDisplayFile(const char* path) const;

		void save() const;

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
		_NODISCARD std::string getDisplayFilePath() const;

	};

}