#pragma once

#include "../Common/common.h"

namespace FD::History::Project {

	enum class Type : UT {
		Empty,
		Empty_Lua,
		Empty_Python,
		Empty_Cpp,
		Interactive
	};

}

namespace FD::History {

	//single thread
	class ProjectWrite final {
	public:
		explicit ProjectWrite(::FD::Internal::PassKey) {}
		~ProjectWrite() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(ProjectWrite);

	public:
		enum class Exception : UT {

		};

	public:
		void update(const Project::Type type);

		//Clear "All"
		void clear();

		//latest 0, 1, 2,...
		void erase(const Size index);

	private:
		static std::vector<Project::Type> load();

	private:
		friend class ProjectRead;

	};

}

namespace FD::History {

	//single thread
	class ProjectRead final {
	public:
		explicit ProjectRead(::FD::Internal::PassKey) {}
		~ProjectRead() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(ProjectRead);

	public:
		[[nodiscard]] std::vector<Project::Type> recent() const;

	};

}