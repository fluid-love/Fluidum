#include "file.h"

namespace FD::History::File {

	[[nodiscard]] Type toType(const std::string& str) {
		using enum Type;

		if (str == "Empty")
			return Empty;
		if (str == "EmptyLua")
			return Empty_Lua;
		if (str == "EmptyPython")
			return Empty_Python;
		if (str == "EmptyCpp")
			return Empty_Cpp;

		Internal::GMessenger.add<FU::Log::Type::Warning>(__FILE__, __LINE__, "The file may be corrupt.");
		return static_cast<Type>(std::numeric_limits<std::underlying_type_t<Type>>::max());

	}

	[[nodiscard]] std::string toString(const Type type) {
		using enum Type;

		if (type == Empty)
			return "Empty";
		if (type == Empty_Lua)
			return "EmptyLua";
		if (type == Empty_Python)
			return "EmptyPython";
		if (type == Empty_Cpp)
			return "EmptyCpp";

		Internal::GMessenger.add<FU::Log::Type::Warning>(__FILE__, __LINE__, "Internal Error.");
		return {};

	}

	[[nodiscard]] bool isError(const Type type) {
		return static_cast<std::underlying_type_t<Type>>(type) == std::numeric_limits<std::underlying_type_t<Type>>::max();
	}

}

void FD::History::FileWrite::update(const File::Type type) const {

	std::vector<FD::History::File::Type> temp = load();
	Size size = temp.size();

	auto itr = std::find(temp.begin(), temp.end(), type);

	std::ofstream ofs(Internal::Resource::RecentProjectTemplateHistoryFilePath, std::ios::binary | std::ios::trunc);
	if (!ofs)
		throw std::runtime_error("Failed to open RecentFile.history");

	//new
	if (itr == temp.end() && size < Limits::FileLogMax) {
		size++;
		ofs << size << std::endl;
		for (auto x : temp) {
			const std::string str = File::toString(x);
			if (str.empty())
				continue;
			ofs << str << std::endl;
		}
		const std::string str = File::toString(type);
		if (!str.empty())
			ofs << str << std::endl;
	}
	else if (itr == temp.end() && size == Limits::FileLogMax) {
		ofs << size << std::endl;
		//ignore front elm
		for (Size i = 0; i < Limits::FileLogMax; i++) {
			const std::string str = File::toString(temp[i]);
			if (str.empty())
				continue;
			ofs << str << std::endl;
		}
		const std::string str = File::toString(type);
		if (!str.empty())
			ofs << str << std::endl;
	}
	else {
		temp.erase(itr);
		ofs << size << std::endl;
		for (auto x : temp) {
			const std::string str = File::toString(x);
			if (str.empty())
				continue;
			ofs << str << std::endl;
		}
		const std::string str = File::toString(type);
		if (!str.empty())
			ofs << str << std::endl;
	}
}

std::vector<FD::History::File::Type> FD::History::FileWrite::load() {

	std::ifstream ifs(Internal::Resource::RecentFileHistoryFilePath);
	if (!ifs)
		throw std::runtime_error("Failed to open RecentTemplateProject.history");

	std::string buf{};

	UIF16 size{};
	std::getline(ifs, buf);
	size = static_cast<UIF16>(std::stol(buf));

	//limit max 50
	std::vector<File::Type> temp{};
	{
		temp.reserve(Limits::FileLogMax);

		for (UIF16 i = 0; i < size; i++) {
			std::getline(ifs, buf);
			File::Type type = File::toType(buf);
			if (File::isError(type))//error
				continue;

			temp.emplace_back(type);
		}
	}

	return temp;
}

std::vector<FD::History::File::Type> FD::History::FileRead::recent() const {
	return FileWrite::load();
}