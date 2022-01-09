#pragma once

#include "property.h"

namespace FD::Project::Property::Internal {

	class LuaData final {
	private:
		FluidumUtils_Class_Delete_ConDestructor(LuaData);

	private:
		

	private:
		friend class ::FD::Project::PropertyLuaWrite;
		friend class ::FD::Project::PropertyLuaRead;

	};

}

namespace FD::Project {

	class PropertyLuaWrite final {
	public:
		explicit PropertyLuaWrite(::FD::Internal::PassKey) noexcept;
		~PropertyLuaWrite() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(PropertyLuaWrite);

	public:


	};

}

namespace FD::Project {

	class PropertyLuaRead final {
	public:
		explicit PropertyLuaRead(::FD::Internal::PassKey) noexcept {};
		~PropertyLuaRead() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(PropertyLuaRead);

	public:


	};

}