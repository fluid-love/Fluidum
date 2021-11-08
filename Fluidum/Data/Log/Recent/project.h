#pragma once

#include "../../Common/common.h"

namespace FD::Log::Project {
	enum class Type : uint8_t {
		Empty,
		Empty_Lua,
		Empty_Python,
		Empty_AngelScript,
		Interactive
	};
}

namespace FD::Log {

	//single thread
	class ProjectWrite final {
	public:
		explicit ProjectWrite(::FD::Internal::PassKey) {}
		~ProjectWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(ProjectWrite)

	public:
		void update(const Project::Type type) const;

	};

	//single thread
	class ProjectRead final {
	public:
		explicit ProjectRead(::FD::Internal::PassKey) {}
		~ProjectRead() = default;
		FluidumUtils_Class_Delete_CopyMove(ProjectRead)

	public:
		_NODISCARD std::vector<Project::Type> recent() const;
	};

}