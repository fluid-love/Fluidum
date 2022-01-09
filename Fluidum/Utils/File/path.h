#pragma once

#include <iostream>
#include <string>
#include <array>
#include <algorithm>
#include <fstream>
#include <vector>
#include <memory>
#include <cassert>
#include <filesystem>

#include <boost/predef.h>

#include "../Type/type.h"

namespace FU::File {

	//[compile-time] get length of string(const char*)
	[[nodiscard]] consteval Size Strlen(const char* str) noexcept {
		return *str ? 1 + Strlen(str + 1) : 0;
	}

	struct PathArg final {
		template<Size N>
		constexpr PathArg(const char(&path)[N]) : ptr(path) {}
		const char* ptr;
	};

	template<PathArg FullPath, const Size BackCount, PathArg AddPath>
	class MakePath final {
	private:
		static consteval auto GetFullPath() {
			constexpr auto fullPathSize = Strlen(FullPath.ptr);
			constexpr auto addPathSize = Strlen(AddPath.ptr);

			//back is '\0'
			std::array<char, fullPathSize + addPathSize + 1> result = {};
			std::fill(result.begin(), result.end(), '\0');

			std::copy(FullPath.ptr, FullPath.ptr + fullPathSize, result.begin());

			for (Size count = 0; count < BackCount; count++) {
				for (IMax i = fullPathSize - 1; i >= 0; i--) {
#ifdef BOOST_OS_WINDOWS
					if (result.at(i) == '/' || result.at(i) == '\\') {
						result.at(i) = '\0';
						break;
					}
#else 
					if (result.at(i) == '/') {
						result.at(i) = '\0';
						break;
					}
#endif
					result.at(i) = '\0';
				}
			}

			for (Size j = 0; j < addPathSize; j++) {
				for (Size i = 0; i < result.size(); i++) {
					if (result.at(i) == '\0') {
						result.at(i) = AddPath.ptr[j];
						break;
					}
				}
			}

#ifdef BOOST_OS_WINDOWS
			//\\ to /
			for (auto& x : result) {
				if (x == '\\')
					x = '/';
			}
#endif

			assert(result.back() == '\0');

			return result;
		}

	private:
		//auto == std::array<char,size>
		static constexpr inline auto Path = GetFullPath();

	public:
		//decltype(Path) == std::array
		//return as const char*
		[[nodiscard]] constexpr operator const char* ()const noexcept {
			return Path.data();
		}
	};


}

namespace FU::File {

	//C:/test/test/ -> test
	//C:/test/test -> test
	[[nodiscard]] std::string directoryName(const std::string& path);

	//C:/test/test/ -> C:/test/test/
	//C:/test/test -> C:/test/
	[[nodiscard]] std::string directory(const std::string& path);

	//C:/test/test.cpp -> test.cpp
	[[nodiscard]] std::string fileName(const std::string& path);

	//Windows: '\\' -> '/'
	//Back '/'
	//lexically_normal
	[[nodiscard]] std::string consistentDirectory(const std::string& dir);

	void hide(const std::string& path);

	//Windows  "\" "/" ":" "*" "?" """ "<" ">" "|"     only"." ".." ...
	[[nodiscard]] bool containForbiddenCharactor(const std::string& name);

	//Windows "a.." -> "a"
	//		  "a."  -> "a"
	//        "a "  -> "a"
	[[nodiscard]] std::string finalName(const std::string& name);

	//Windows / or \\  
	void tryPushSlash(std::string& path);

	//Windows MAX_PATH(260) return 259 (260 - 1null)
	[[nodiscard]] Size maxPathSize() noexcept;

	[[nodiscard]] std::string absolute(const std::string& path);

	[[nodiscard]] bool isAbsolute(const std::string& path);

	/*
		C:/test/hoge/huga.txt

		(aaa, 0)
		C:/test/hoge/aaa

		(aaa, 1)
		C:/test/aaa/huga.txt
	*/
	[[nodiscard]] std::string changeName(const std::string& path, const std::string& newName, const Size depth);

}