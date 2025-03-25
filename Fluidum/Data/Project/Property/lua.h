#pragma once

#include "property.h"

namespace FD::Project::Property::Lua {

	enum class StandardFluidumLibrary : UT {
		System,
		Array,
		Plot,
		Genome
	};

	[[nodiscard]] const char* StandardFluidumLibraryTypeToCChar(const StandardFluidumLibrary val) noexcept;

}

namespace FD::Project::Property::Internal {

	class LuaData final {
	private:
		FluidumUtils_Class_Delete_ConDestructor(LuaData);
		FluidumUtils_Class_Delete_CopyMove(LuaData);

	private:
		static inline std::string entryFilePath{};
		static inline std::vector<Lua::StandardFluidumLibrary> disabledFluidumStandardLibrary{};

	private:
		friend class ::FD::Project::PropertyLuaWrite;
		friend class ::FD::Project::PropertyLuaRead;
		friend class ::FD::ProjectWrite;

	};

}

namespace FD::Project {

	class PropertyLuaWrite final {
	public:
		explicit PropertyLuaWrite(::FD::Internal::PassKey) noexcept {};
		~PropertyLuaWrite() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(PropertyLuaWrite);

	public:
		//no-throw
		void save() noexcept;

	public:
		/*
		Exception:
			std::exception
		*/
		//strong
		void entryFilePath(const std::string& path);

	public:
		void enableStandardFluidumLibrary(const Property::Lua::StandardFluidumLibrary val);

		void disableStandardFluidumLibrary(const Property::Lua::StandardFluidumLibrary val);

	};

}

namespace FD::Project {

	class PropertyLuaRead final {
	public:
		explicit PropertyLuaRead(::FD::Internal::PassKey) noexcept {};
		~PropertyLuaRead() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(PropertyLuaRead);

	public:
		/*
		Exception:
			std::exception
		*/
		//strong
		[[nodiscard]] std::string entryFilePath() const;

	public:
		//no-throw
		//exists -> true, not exists -> false
		[[nodiscard]] bool entryFileExists() const noexcept;

	public:
		[[nodiscard]] std::vector<Property::Lua::StandardFluidumLibrary> disabledStandardFluidumLibrary() const;

		[[nodiscard]] bool isStandardFluidumLibraryDisabled(const Property::Lua::StandardFluidumLibrary val) const;

	};

}