#pragma once

#include "filelist.h"

namespace FD::Project::File {

	enum class SupportedFileType : uint8_t {
		None,

		Lua,  
		Python,
		Cpp,



	};


	//not supported -> return Error
	[[nodiscard]] SupportedFileType getSupportedFileType(const std::string& name);

	[[nodiscard]] bool isFileFormatSupported(const std::string& name);

}