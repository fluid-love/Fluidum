#pragma once

#include "../Common/common.h"

namespace FD::History::File {

	enum class Type : UT {
		None,
		Empty,
		Empty_Lua,
		Empty_Python,
		Empty_Cpp
	};

}

namespace FD::History::File::Template {

	//short -> .exe
	//long  -> Fluidum/Resources/
	constexpr const char* EmptyLua = "--EmptyFile";
	constexpr const char* EmptyPy = "#EmptyFile";
	constexpr const char* EmptyCpp = "//EmptyFile";

}

namespace FD::History {

	class FileWrite final {
	public:
		explicit FileWrite(::FD::Internal::PassKey) {}
		~FileWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(FileWrite);

	public:
		void update(const File::Type type) const;

	private:
		static std::vector<FD::History::File::Type> load();

	private:
		friend class FileRead;

	};

}

namespace FD::History {

	class FileRead final {
	public:
		explicit FileRead(::FD::Internal::PassKey) {}
		~FileRead() = default;
		FluidumUtils_Class_Delete_CopyMove(FileRead);

	public:
		[[nodiscard]] std::vector<File::Type> recent() const;

	};

}