#pragma once

#include "../../Common/common.h"

namespace FD::Log::File {
	enum class Type : uint8_t {
		None,
		Empty,
		Empty_Lua,
		Empty_Python,
		Empty_Cpp
	};

}

namespace FD::File::Template {
	//short -> .exe
	//long  -> Fluidum/Resources/
	constexpr const char* EmptyLua = "--EmptyFile";
	constexpr const char* EmptyPy = "#EmptyFile";
	constexpr const char* EmptyAS = "//EmptyFile";
}

namespace FD::Log {

	class FileWrite final {
	public:
		explicit FileWrite(::FD::Internal::PassKey) {}
		~FileWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(FileWrite)

	public:
		void update(const File::Type type) const;

	};


	class FileRead final {
	public:
		explicit FileRead(::FD::Internal::PassKey) {}
		~FileRead() = default;
		FluidumUtils_Class_Delete_CopyMove(FileRead)

	public:
		_NODISCARD std::vector<File::Type> recent() const;

	};

}