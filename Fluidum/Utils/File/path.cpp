#include "path.h"

#ifdef BOOST_OS_WINDOWS
#include <Windows.h>
#include "../Text/guitext.h"
#endif

std::string FU::File::directoryName(const std::string& path) {
	std::string result = path;

#ifdef BOOST_OS_WINDOWS
	if (result.back() == '/' || result.back() == '\\')
		result.pop_back();
	auto itr = std::find_if(result.rbegin(), result.rend(), [](auto& x) {return (x == '/') || (x == '\\'); });
#else
	if (result.back() == '/')
		result.pop_back();
	auto itr = std::find(result.rbegin(), result.rend(), '/');
#endif
	if (itr == result.rend())
		throw std::runtime_error("Arg \"path\" is not directory.");

	result.erase(result.begin(), std::next(itr).base() + 1);

	return result;
}

std::string FU::File::directory(const std::string& path) {
	std::string result = path;

#ifdef BOOST_OS_WINDOWS
	if (result.back() == '/' || result.back() == '\\')
		return path;
	auto itr = std::find_if(result.rbegin(), result.rend(), [](auto& x) {return (x == '/') || (x == '\\'); });
#else
	if (result.back() == '/')
		result.pop_back();
	auto itr = std::find(result.rbegin(), result.rend(), '/');
#endif
	if (itr == result.rend())
		throw std::runtime_error("Arg \"path\" is not directory.");

	result.erase(itr.base(), result.end());

	return result;
}

std::string FU::File::fileName(const std::string& path) {
	const std::filesystem::path p = path;
	return p.filename().string();
}

std::string FU::File::consistentDirectory(const std::string& dir) {
	std::string result = std::filesystem::path(dir).lexically_normal().generic_string();
#ifdef BOOST_OS_WINDOWS
	auto itr = result.begin();
	while (true) {
		itr = std::find(itr, result.end(), '\\');

		if (itr != result.end())
			*itr = '/';
		else
			break;
	}
#endif
	if (result.back() != '/')
		result.push_back('/');
	return result;
}

void FU::File::hide(const std::string& path) {

#ifdef BOOST_OS_WINDOWS
	//set attribute
	const std::wstring wstr = FU::Text::utf8ToUtf16(path);
	int att = GetFileAttributes(wstr.data());
	if ((att & FILE_ATTRIBUTE_HIDDEN) == 0) {
		SetFileAttributes(wstr.data(), att | FILE_ATTRIBUTE_HIDDEN);
	}
#else
#error NotSupported
#endif
}

bool FU::File::containForbiddenCharactor(const std::string& name) {
#ifdef BOOST_OS_WINDOWS
	auto itr = std::find_if(name.begin(), name.end(), [](char x)
		{
			return
				(x == '\\') ||
				(x == '/') ||
				(x == ':') ||
				(x == '*') ||
				(x == '?') ||
				(x == '\"') ||
				(x == '<') ||
				(x == '>"') ||
				(x == '|');
		}
	);
#else
#error NotSupported
#endif
	return itr != name.end();
}

void FU::File::tryPushSlash(std::string& path) {
#ifdef BOOST_OS_WINDOWS
	if (path.back() != '/' && path.back() != '\\')
		path.push_back('/');
#else
	if (path.back() != '/')
		path.push_back('/');
#endif
}