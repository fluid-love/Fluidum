#include "file.h"

namespace FD::Log::File {
	constexpr inline uint8_t LimitMax = 50;
}

void FD::Log::FileWrite::update(const File::Type type) const {
	using UT = std::underlying_type_t<File::Type>;

	std::ifstream ifs(Internal::Resource::RecentFileHistoryFilePath, std::ios::binary);
	if (!ifs)
		throw std::runtime_error("Failed to open RecentFile.history");

	uint16_t size{};
	ifs.read(reinterpret_cast<char*>(&size), sizeof(uint16_t));

	//limit max 50
	std::vector<File::Type> temp{};
	{
		temp.reserve(File::LimitMax);

		for (uint16_t i = 0; i < size; i++) {

			UT val{};
			ifs.read(reinterpret_cast<char*>(val), sizeof(UT));
			temp.emplace_back(static_cast<File::Type>(val));
		}
	}
	ifs.close();

	auto itr = std::find(temp.begin(), temp.end(), type);


	std::ofstream ofs(Internal::Resource::RecentProjectTemplateHistoryFilePath, std::ios::binary | std::ios::trunc);
	if (!ofs)
		throw std::runtime_error("Failed to open RecentFile.history");
	//new
	if (itr == temp.end() && size < File::LimitMax) {
		size++;
		ofs.write(reinterpret_cast<const char*>(&size), sizeof(size));
		ofs.write(reinterpret_cast<const char*>(temp.data()), sizeof(UT) * temp.size());
		ofs.write(reinterpret_cast<const char*>(&type), sizeof(UT));
	}
	else if (itr == temp.end() && size == File::LimitMax) {
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

std::vector<FD::Log::File::Type> FD::Log::FileRead::recent() const {
	using UT = std::underlying_type_t<File::Type>;

	std::ofstream ofs(Internal::Resource::RecentProjectTemplateHistoryFilePath, std::ios::binary);
	if (!ofs)
		throw std::runtime_error("Failed to open RecentTemplateProject.history");
	uint16_t in = 0;
	ofs.write(reinterpret_cast<const char*>(&in), sizeof(uint16_t));

	std::ifstream ifs(Internal::Resource::RecentProjectTemplateHistoryFilePath, std::ios::binary);
	if (!ifs)
		throw std::runtime_error("Failed to open RecentTemplateProject.history");

	uint16_t size{};
	ifs.read(reinterpret_cast<char*>(&size), sizeof(uint16_t));

	assert(size <= File::LimitMax);

	std::vector<File::Type> result(size);
	result.reserve(File::LimitMax);

	ifs.read(reinterpret_cast<char*>(result.data()), sizeof(UT) * size);

	return result;
}