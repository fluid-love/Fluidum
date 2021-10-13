#include "path.h"

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

	result.erase(result.begin(), std::next(itr).base());

	return result;
}