#include "FDfile.h"
#include <filesystem>
#include "../common/FDlogfilepath.h"

FD::File::File(Internal::ManagerKey) {
	//�f�B���N�g�����Ȃ���΍��D����Ώ����č��D
	if (std::filesystem::is_directory(Internal::TempLogDirectory)) {
		if (!std::filesystem::remove_all(Internal::TempLogDirectory))
			throw std::runtime_error("Failed to remove \"FluidumTempLog\" directory.");
	}
	if (!std::filesystem::create_directory(Internal::TempLogDirectory))
		throw std::runtime_error("Failed to create \"FluidumTempLog\" directory.");

	if (std::filesystem::is_directory(Internal::PropertyLogDirectory)) {
		if (!std::filesystem::remove_all(Internal::PropertyLogDirectory))
			throw std::runtime_error("Failed to remove \"FluidumPropertyLog\" directory.");
	}
	if (!std::filesystem::create_directory(Internal::PropertyLogDirectory))
		throw std::runtime_error("Failed to create \"FluidumPropertyLog\" directory.");


	//Log�͂����Ȃ��@�Ȃ���΍��
	if (!std::filesystem::is_directory(Internal::LogDirectory)) {
		if (!std::filesystem::create_directory(Internal::LogDirectory))
			throw std::runtime_error("Failed to create \"FluidumLog\" directory.");
	}
}
