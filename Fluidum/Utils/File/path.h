#pragma once

#include <iostream>
#include <string>
#include <array>
#include <algorithm>
#include <fstream>
#include <vector>
#include <memory>
#include <cassert>

#include <boost/predef.h>

namespace FU::File {

	//get length of string(const char*)
	_NODISCARD consteval std::size_t Strlen(const char* str) noexcept {
		return *str ? 1 + Strlen(str + 1) : 0;
	}

	struct PathArg final {
		template<std::size_t Size>
		constexpr PathArg(const char(&path)[Size]) : ptr(path) {}
		const char* ptr;
	};

	template<PathArg FullPath, const std::size_t BackCount, PathArg AddPath>
	class MakePath final {
	private:
		static consteval auto GetFullPath() {
			constexpr auto fullPathSize = Strlen(FullPath.ptr);
			constexpr auto addPathSize = Strlen(AddPath.ptr);

			//back is '\0'
			std::array<char, fullPathSize + addPathSize + 1> result = {};
			std::fill(result.begin(), result.end(), '\0');

			std::copy(FullPath.ptr, FullPath.ptr + fullPathSize, result.begin());

			for (std::size_t count = 0; count < BackCount; count++) {
				for (int64_t i = fullPathSize - 1; i >= 0; i--) {
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

			for (uint32_t j = 0; j < addPathSize; j++) {
				for (int32_t i = 0; i < result.size(); i++) {
					if (result.at(i) == '\0') {
						result.at(i) = AddPath.ptr[j];
						break;
					}
				}
			}

			assert(result.back() == '\0');

			return result;
	}

	private:
		//auto == std::array<char,size>
		static constexpr inline auto Path = GetFullPath();

	public:
		//decltype(Path) == std::array
		//return as const char*
		_NODISCARD constexpr operator const char* ()const noexcept {
			return Path.data();
		}
};


}
