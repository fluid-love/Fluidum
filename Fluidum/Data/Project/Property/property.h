#pragma once

#include "../../Common/common.h"

namespace FD::Project::Property {

	enum class ProjectType : UT {
		None,

		Lua,
		Python,
		Cpp
	};

	[[nodiscard]] constexpr const char* projectTypeToCChar(const ProjectType type) noexcept {
		using enum ProjectType;
		if (type == None)
			return "None";
		if (type == Lua)
			return "Lua";
		if (type == Python)
			return "Pyhton";
		if (type == Cpp)
			return "C++";

		assert(false);
		return "";
	}

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
		static inline ProjectType currentProjectType = ProjectType::None;

	private:
		static inline std::mutex mtx{};
		static inline std::atomic_bool save{};

	private:
		friend class ::FD::Project::PropertyWrite;
		friend class ::FD::Project::PropertyRead;
		friend class ::FD::Project::PropertyLuaWrite;
		friend class ::FD::Project::PropertyLuaRead;
		friend class ::FD::ProjectWrite;

	};

}

namespace FD::Project {

	class PropertyWrite final {
	public:
		explicit PropertyWrite(::FD::Internal::PassKey) noexcept {};
		~PropertyWrite() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(PropertyWrite);

	public:
		void save() noexcept;

	public:
		//no-throw
		void currentProjectType(const Property::ProjectType type) noexcept;

	};

}

namespace FD::Project {

	class PropertyRead final {
	public:
		explicit PropertyRead(::FD::Internal::PassKey) noexcept {};
		~PropertyRead() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(PropertyRead);

	public:
		//no-throw
		[[nodiscard]] Property::ProjectType currentProjectType() const noexcept;

		[[nodiscard]] bool isLua() const noexcept;
		[[nodiscard]] bool isPython() const noexcept;
		[[nodiscard]] bool isCpp() const noexcept;

	};

}