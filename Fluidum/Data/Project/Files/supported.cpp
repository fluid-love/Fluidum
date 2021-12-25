#include "supported.h"

FD::Project::File::SupportedFileType FD::Project::File::getSupportedFileType(const std::string& name) {
	std::filesystem::path p = name;
	const std::string extension = p.extension().string();

	if (extension == ".py")
		return SupportedFileType::Python;
	else if (extension == ".lua")
		return SupportedFileType::Lua;
	else if (extension == ".cpp" || extension == ".h" || extension == ".c" || extension == ".cxx")
		return SupportedFileType::Cpp;
	return SupportedFileType::None;
}

bool FD::Project::File::isFileFormatSupported(const std::string& name) {
	const SupportedFileType type = getSupportedFileType(name);
	return type != SupportedFileType::None;
}


