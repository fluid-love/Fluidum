#include "window.h"

bool* FD::WindowWrite::getCloseFlag() const noexcept {
	using namespace Internal::Scene;
	return &WindowData::closeFlag;
}

void FD::WindowWrite::setIsMaximise(const bool flag) const {
	using namespace Internal::Scene;
	std::lock_guard<std::mutex> lock(WindowData::mtx);
	WindowData::isMaximize = flag;
}

void FD::WindowWrite::setIsMinimise(const bool flag) const {
	using namespace Internal::Scene;
	std::lock_guard<std::mutex> lock(WindowData::mtx);
	WindowData::isMinimize = flag;
}

void FD::WindowWrite::setSize(const int32_t width, const int32_t heigth) const {
	using namespace Internal::Scene;
	std::lock_guard<std::mutex> lock(WindowData::mtx);
	WindowData::savedSize = { width,heigth };
}

void FD::WindowWrite::setPos(const int32_t x, const int32_t y) const {
	using namespace Internal::Scene;
	std::lock_guard<std::mutex> lock(WindowData::mtx);
	WindowData::savedSize = { x,y };
}

bool FD::WindowRead::getIsMaximise() const {
	using namespace Internal::Scene;
	std::lock_guard<std::mutex> lock(WindowData::mtx);
	return WindowData::isMaximize;
}

bool FD::WindowRead::getIsMinimise() const {
	using namespace Internal::Scene;
	std::lock_guard<std::mutex> lock(WindowData::mtx);
	return WindowData::isMinimize;
}

std::pair<int32_t, int32_t> FD::WindowRead::getSavedSize() const {
	using namespace Internal::Scene;
	std::lock_guard<std::mutex> lock(WindowData::mtx);
	return WindowData::savedSize;
}

std::pair<int32_t, int32_t> FD::WindowRead::getSavedPos() const {
	using namespace Internal::Scene;
	std::lock_guard<std::mutex> lock(WindowData::mtx);
	return WindowData::savedPos;
}

namespace FD {
	struct {
		bool saveAsAndExit = false;
		std::mutex mtx{};
	}GExitData;
}

void FD::ExitWrite::saveAsAndExit() const {
	using namespace Internal::Scene;
	std::lock_guard<std::mutex> lock(GExitData.mtx);
	GExitData.saveAsAndExit = true;
}

bool FD::ExitRead::saveAsAndExit() const {
	using namespace Internal::Scene;
	std::lock_guard<std::mutex> lock(GExitData.mtx);
	const auto result = GExitData.saveAsAndExit;
	if (GExitData.saveAsAndExit)
		GExitData.saveAsAndExit = false;
	return result;
}
