#include "project.h"

namespace FD::History::Project {

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
		if (str == "Interactive")
			return Interactive;

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
		if (type == Interactive)
			return "Interactive";

		Internal::GMessenger.add<FU::Log::Type::Warning>(__FILE__, __LINE__, "Internal Error.");
		return {};

	}

	[[nodiscard]] bool isError(const Type type) {
		return static_cast<std::underlying_type_t<Type>>(type) == std::numeric_limits<std::underlying_type_t<Type>>::max();
	}

}

void FD::History::ProjectWrite::update(const Project::Type type) {

	std::vector<FD::History::Project::Type> temp = load();
	Size size = temp.size();

	auto itr = std::find(temp.begin(), temp.end(), type);

	std::ofstream ofs(Internal::Resource::RecentProjectTemplateHistoryFilePath, std::ios::binary | std::ios::trunc);
	if (!ofs)
		throw std::runtime_error("Failed to open RecentTemplateProject.history");

	//new
	if (itr == temp.end() && size < Limits::ProjectLogMax) {
		size++;
		ofs << size << std::endl;
		for (auto x : temp) {
			const std::string str = Project::toString(x);
			if (str.empty())
				continue;
			ofs << str << std::endl;
		}
		const std::string str = Project::toString(type);
		if (!str.empty())
			ofs << str << std::endl;
	}
	else if (itr == temp.end() && size == Limits::ProjectLogMax) {
		ofs << size << std::endl;
		//ignore front elm
		for (Size i = 0; i < Limits::ProjectLogMax; i++) {
			const std::string str = Project::toString(temp[i]);
			if (str.empty())
				continue;
			ofs << str << std::endl;
		}
		const std::string str = Project::toString(type);
		if (!str.empty())
			ofs << str << std::endl;
	}
	else {
		temp.erase(itr);
		ofs << size << std::endl;
		for (auto x : temp) {
			const std::string str = Project::toString(x);
			if (str.empty())
				continue;
			ofs << str << std::endl;
		}
		const std::string str = Project::toString(type);
		if (!str.empty())
			ofs << str << std::endl;
	}
}

void FD::History::ProjectWrite::clear() {
	std::ifstream ofs(Internal::Resource::RecentProjectTemplateHistoryFilePath, std::ios::trunc);
	if (!ofs)
		throw std::runtime_error("Failed to open RecentTemplateProject.history");
}

void FD::History::ProjectWrite::erase(const Size index) {

	std::vector<FD::History::Project::Type> temp = load();
	Size size = temp.size();

	if (index >= temp.size()) {
		assert(false);
		return;
	}

	temp.erase(temp.begin() + index);

	//write
	std::ofstream ofs(Internal::Resource::RecentProjectTemplateHistoryFilePath, std::ios::binary | std::ios::trunc);
	if (!ofs)
		throw std::runtime_error("Failed to open RecentTemplateProject.history");

	ofs << size << std::endl;
	for (auto x : temp) {
		const std::string str = Project::toString(x);
		if (str.empty())
			continue;
		ofs << str << std::endl;
	}

}

std::vector<FD::History::Project::Type> FD::History::ProjectWrite::load() {

	std::ifstream ifs(Internal::Resource::RecentProjectTemplateHistoryFilePath);
	if (!ifs)
		throw std::runtime_error("Failed to open RecentTemplateProject.history");

	if (FU::File::empty(ifs)) {
		return {};
	}

	std::string buf{};

	UIF16 size{};
	std::getline(ifs, buf);
	size = static_cast<UIF16>(std::stol(buf));

	//limit max 50
	std::vector<Project::Type> temp{};
	{
		temp.reserve(Limits::ProjectLogMax);

		for (UIF16 i = 0; i < size; i++) {
			std::getline(ifs, buf);
			Project::Type type = Project::toType(buf);
			if (Project::isError(type))//error
				continue;

			temp.emplace_back(type);
		}
	}

	return temp;
}

std::vector<FD::History::Project::Type> FD::History::ProjectRead::recent() const {
	return ProjectWrite::load();
}