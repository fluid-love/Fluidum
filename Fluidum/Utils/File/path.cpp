#include "path.h"
#include "../Text/guitext.h"
#include "../Type/type.h"

#ifdef FluidumUtils_Type_OS_Windows
#include <Windows.h>
#endif

std::string FU::File::directoryName(const std::string& path) {
	std::string result = path;

#ifdef FluidumUtils_Type_OS_Windows
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

#ifdef FluidumUtils_Type_OS_Windows
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
	std::filesystem::path path(dir);
	std::string result = path.lexically_normal().generic_string();
#ifdef FluidumUtils_Type_OS_Windows
	auto itr = result.begin();
	while (true) {
		itr = std::find(itr, result.end(), '\\');

		if (itr != result.end())
			*itr = '/';
		else
			break;
	}
#endif
	return result;
}

void FU::File::hide(const std::string& path) {

#ifdef FluidumUtils_Type_OS_Windows
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
#ifdef FluidumUtils_Type_OS_Windows
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

	const auto dotCount = std::count(name.begin(), name.end(), '.');
	const auto spaceCount = std::count(name.begin(), name.end(), ' ');

#else
#error NotSupported
#endif
	return (itr != name.end()) || (dotCount == name.size()) || (spaceCount == name.size());
}

std::string FU::File::finalName(const std::string& name) {
	assert(!containForbiddenCharactor(name) && !name.empty());

	std::string result = name;

	Size count = 0;

	for (auto itr = result.rbegin(), end = result.rend(); itr != end; itr++) {
		if (*itr == '.' || *itr == ' ')
			count++;
		else
			break;
	}

	if (count == 0)
		return name;

	result.erase(result.end() - count, result.end());
	return result;
}

void FU::File::tryPushSlash(std::string& path) {
	if (path.empty())
		return;
#ifdef FluidumUtils_Type_OS_Windows
	if (path.back() != '/' && path.back() != '\\')
		path.push_back('/');
#else
	if (path.back() != '/')
		path.push_back('/');
#endif
}

FU::Size FU::File::maxPathSize() noexcept {
#ifdef FluidumUtils_Type_OS_Windows
	return MAX_PATH;
#else
#error NotSupported
#endif
}

std::string FU::File::absolute(const std::string& path) {
	std::filesystem::path p(path);
	if (p.is_absolute())
		return path;

	const auto abso = std::filesystem::absolute(path);
	return abso.string();
}

bool FU::File::isAbsolute(const std::string& path) {
	const std::filesystem::path p(path);
	return p.is_absolute();
}

std::string FU::File::changeName(const std::string& path, const std::string& newName, const Size depth) {
	std::string back{};
	back.resize(path.size());
	{
		std::string temp = path;
		auto itr = temp.rbegin();
		Size dis = 0;

		for (Size i = 0; i < depth; i++) {

#ifdef FluidumUtils_Type_OS_Windows
			auto find = std::find_if(itr, temp.rend(), [](auto& x) {return (x == '/') || (x == '\\'); });
#endif
			if (find == temp.rend())
				return newName;

			std::copy((find + 1).base(), itr.base(), std::inserter(back, back.begin()));

			dis += static_cast<Size>(std::distance(itr, find));
			itr = (find + 1);
		}

	}

	std::string front = path;

	for (Size i = 0; i < depth + 1; i++) {
		front = ::FU::File::directory(front);
		front.pop_back();
	}
	front.push_back('/');

	(front += newName) += back;
	return front;
}