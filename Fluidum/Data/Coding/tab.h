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

		static inline std::mutex mtx{};
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
		void addFile(const char* path) const;
		void eraseFile(const char* path) const;



	};

	//current project
	class TabRead final {
	public:
		explicit TabRead(Internal::PassKey) noexcept {};
		~TabRead() = default;
		FluidumUtils_Class_Delete_CopyMove(TabRead)

	public:


		_NODISCARD std::vector<std::string> getFilePathes() const;

	};

}