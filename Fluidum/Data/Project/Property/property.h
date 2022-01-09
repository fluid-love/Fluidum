#pragma once

#include "../../Common/common.h"

namespace FD::Project::Property {

	enum class ProjectType : UT {
		None,

		Lua,
		Python,
		Cpp
	};

}

//forward declaration
namespace FD {

	class ProjectWrite;

	namespace Project {

		class PropertyWrite;
		class PropertyRead;

		class PropertyLuaWrite;
		class PropertyLuaRead;

	}

}

namespace FD::Project::Property::Internal {

	class Data final {
	private:
		FluidumUtils_Class_Delete_ConDestructor(Data);

	private:
		

	private:
		friend class ::FD::Project::PropertyWrite;
		friend class ::FD::Project::PropertyRead;

	};

}

namespace FD::Project {

	class PropertyWrite final {
	public:
		explicit PropertyWrite(::FD::Internal::PassKey) noexcept;
		~PropertyWrite() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(PropertyWrite);

	public:


	};

}

namespace FD::Project {

	class PropertyRead final {
	public:
		explicit PropertyRead(::FD::Internal::PassKey) noexcept {};
		~PropertyRead() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(PropertyRead);

	public:


	};

}