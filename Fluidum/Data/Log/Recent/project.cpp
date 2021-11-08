#include "project.h"

namespace FD::Log::Project {
	constexpr inline uint8_t LimitMax = 50;
}

void FD::Log::ProjectWrite::update(const Project::Type type) const {
	using UT = std::underlying_type_t<Project::Type>;

	std::ifstream ifs(Internal::Resource::RecentProjectTemplateHistoryFilePath, std::ios::binary);
	if (!ifs)
		throw std::runtime_error("Failed to open RecentTemplateProject.history");

	uint16_t size{};
	ifs.read(reinterpret_cast<char*>(&size), sizeof(uint16_t));

	//limit max 50
	std::vector<Project::Type> temp{};
	{
		temp.reserve(Project::LimitMax);

		for (uint16_t i = 0; i < size; i++) {

			UT val{};
			ifs.read(reinterpret_cast<char*>(val), sizeof(UT));
			temp.emplace_back(static_cast<Project::Type>(val));
		}
	}
	ifs.close();

	auto itr = std::find(temp.begin(), temp.end(), type);


	std::ofstream ofs(Internal::Resource::RecentProjectTemplateHistoryFilePath, std::ios::binary | std::ios::trunc);
	if (!ofs)
		throw std::runtime_error("Failed to open RecentTemplateProject.history");
	//new
	if (itr == temp.end() && size < Project::LimitMax) {
		size++;
		ofs.write(reinterpret_cast<const char*>(&size), sizeof(size));
		ofs.write(reinterpret_cast<const char*>(temp.data()), sizeof(UT) * temp.size());
		ofs.write(reinterpret_cast<const char*>(&type), sizeof(UT));
	}
	else if (itr == temp.end() && size == Project::LimitMax) {
		ofs.write(reinterpret_cast<const char*>(size), sizeof(size));
		//ignore front elm
		ofs.write(reinterpret_cast<const char*>(temp.data() + 1), sizeof(UT) * (temp.size() - 1));
		ofs.write(reinterpret_cast<const char*>(&type), sizeof(UT));
	}
	else {
		temp.erase(itr);
		ofs.write(reinterpret_cast<const char*>(&size), sizeof(size));
		ofs.write(reinterpret_cast<const char*>(temp.data()), sizeof(UT) * temp.size());
		ofs.write(reinterpret_cast<const char*>(&type), sizeof(UT));
	}
}

std::vector<FD::Log::Project::Type> FD::Log::ProjectRead::recent() const {
	using UT = std::underlying_type_t<Project::Type>;

	std::ifstream ifs(Internal::Resource::RecentProjectTemplateHistoryFilePath, std::ios::binary);
	if (!ifs)
		throw std::runtime_error("Failed to open RecentTemplateProject.history");

	uint16_t size{};
	ifs.read(reinterpret_cast<char*>(&size), sizeof(uint16_t));

	assert(size <= Project::LimitMax);

	std::vector<Project::Type> result(size);
	result.reserve(Project::LimitMax);

	ifs.read(reinterpret_cast<char*>(result.data()), sizeof(UT) * size);

	return result;
}