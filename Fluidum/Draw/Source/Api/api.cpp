#include "api.h"

#ifdef FluidumDraw_Vulkan

namespace FDR::Internal {

	void messengerCallback(const std::string& message) noexcept {
		FDR::Internal::GMessenger.callCallback(message);
	}

}

void FDR::mainLoop(const char* title, void(*function)(), bool* const windowCloseFlag) {
	try {

		//Recall FluidumVK messages as FluidumDraw messages.
		FVK::setMessengerCallbackFunction(Internal::messengerCallback);

		//Initialize FluidumVK
		FVK::initialize();


		FDR::Internal::Initialization::FullScreenWithBarInfo info;
		info.title = title;
		FDR::Internal::Initialization::initialize(info);
	}
	catch (...) {
		Internal::GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to initialize FluidumDraw.");
		std::terminate();
	}

	Internal::GMessenger.add_str<FU::Log::Type::None>("Initialized FluidumDraw.");

	//loop
	try {
		FDR::Internal::mainLoop(function, windowCloseFlag);
	}
	catch (...) {
		Internal::GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Mainloop threw an exception.");
		FU::Exception::rethrow();
	}

	try {
		FVK::terminate();
	}
	catch (const FVK::Exception::Error& e) {
		using enum FVK::Exception::ErrorType;
		if (e.code() == NotInitialized)
			Internal::GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Internal Error. Failed to terminate FluidumDraw.");
		else if (e.code() == FailedToDestroy)
			Internal::GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to terminate FluidumDraw.");
		else
			Internal::GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Internal Error. Failed to terminate FluidumDraw.");

		std::terminate();
	}
	catch (...) {
		Internal::GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to terminate FluidumDraw.");
		std::terminate();
	}

	Internal::GMessenger.add_str<FU::Log::Type::None>("Terminated FluidumDraw.");
}

void FDR::setMessengerCallbackFunction(const MessengerCallbackType callback) {
	Internal::GMessenger.setCallback(callback);
}

std::pair<FVK::IF32, FVK::IF32> FDR::getWindowSize() {
	return FVK::getWindowSize(Internal::BaseWindowKey);
}

std::pair<FVK::IF32, FVK::IF32> FDR::getFullscreenWindowSize() {
	return FVK::getFulscreenWindowSize();
}

std::pair<FVK::IF32, FVK::IF32> FDR::getWindowPos() {
	return FVK::getWindowPos(Internal::BaseWindowKey);
}

void FDR::setWindowSize(const IF32 width, const IF32 height) {
	FVK::setWindowSize(Internal::BaseWindowKey, width, height);
}

void FDR::setWindowSize(const ImVec2& size) {
	setWindowSize(static_cast<IF32>(size.x), static_cast<IF32>(size.y));
}

void FDR::setWindowPos(const IF32 x, const IF32 y) {
	FVK::setWindowPos(Internal::BaseWindowKey, x, y);
}

void FDR::setWindowPos(const ImVec2& pos) {
	setWindowPos(static_cast<IF32>(pos.x), static_cast<IF32>(pos.y));
}

void FDR::setWindowSizePos(const IF32 width, const IF32 height, const IF32 x, const IF32 y) {
	setWindowSize(width, height);
	setWindowPos(x, y);
}

void FDR::setWindowSizePos(const ImVec2& size, const ImVec2& pos) {
	setWindowSize(size);
	setWindowPos(pos);
}

void FDR::minimizeWindow() {
	FVK::minimizeWindow(Internal::BaseWindowKey);
}

void FDR::fullscreenWindow() {
	FVK::fullscreenWindow(Internal::BaseWindowKey);
}

void FDR::restoreWindow() {

	const auto [w, h] = FVK::getFulscreenWindowSize();

	const bool maximized = FVK::isWindowMaximized(Internal::BaseWindowKey);

	if (!maximized) {

		//save current pos and size.
		{
			const auto [posX, posY] = FVK::getWindowPos(Internal::BaseWindowKey);
			const auto [width, height] = FVK::getWindowSize(Internal::BaseWindowKey);

			std::ofstream ofs(Internal::WindowInfoFilePath, std::ios::trunc);
			ofs << (static_cast<float>(posX) / static_cast<float>(w)) << std::endl;
			ofs << (static_cast<float>(posY) / static_cast<float>(h)) << std::endl;
			ofs << (static_cast<float>(width) / static_cast<float>(w)) << std::endl;
			ofs << (static_cast<float>(height) / static_cast<float>(h)) << std::endl;
		}

		FVK::fullscreenWindow(Internal::BaseWindowKey);
		return;
	}

	int posX, posY, width, height;

	try {
		std::ifstream ifs(Internal::WindowInfoFilePath);
		if (FU::File::empty(ifs))
			throw - 1;

		std::string data{};

		std::getline(ifs, data);
		posX = static_cast<int>(std::stof(data) * w);
		std::getline(ifs, data);
		posY = static_cast<int>(std::stof(data) * h);

		std::getline(ifs, data);
		width = static_cast<int>(std::stof(data) * w);
		std::getline(ifs, data);
		height = static_cast<int>(std::stof(data) * h);

		//check
		const auto [minW, minH] = FVK::getWindowSizeLimitsMin(Internal::BaseWindowKey);

		if (posX >= w || posY >= h)
			throw - 1;
		if (width > w || height > h)
			throw - 1;
		if (width < minW || height < minH)
			throw - 1;
	}
	catch (...) {
		posX = static_cast<int>(-w * 0.04f);
		posY = static_cast<int>(h * 0.04f);
		width = static_cast<int>(w);
		height = static_cast<int>(h);
	}

	//animation
	///for (Size i = 0; i < 100; i++) {
	FVK::setWindowPos(Internal::BaseWindowKey, posX, posY);
	FVK::setWindowSize(Internal::BaseWindowKey, width, height);
	//}

}

bool FDR::isWindowFocused() {
	return FVK::isWindowFocused(Internal::BaseWindowKey);
}

bool FDR::isWindowFullscreen() {
	return FVK::isWindowMaximized(Internal::BaseWindowKey);
}

void FDR::setWindowWidthMaxLimit(const IF32 width) {
	const IF32 full = FVK::getFulscreenWindowSize().first;
	if (width >= full)
		return;
	FVK::setWindowSizeLimitsMax(Internal::BaseWindowKey, width, std::nullopt);
}

void FDR::setWindowHeightMaxLimit(const IF32 height) {
	const IF32 full = FVK::getFulscreenWindowSize().second;
	if (height >= full)
		return;
	FVK::setWindowSizeLimitsMax(Internal::BaseWindowKey, std::nullopt, height);
}

void FDR::setViewportWidthHeight(const float width, const float height) {
	for (auto& x : Internal::Commands::MainViewport) {
		x->setViewportWidth(width);
		x->setViewportHeight(height);
	}
}

void FDR::setScissorExtent(const UI32 width, const UI32 height) {
	for (auto& x : Internal::Commands::MainScissor) {
		x->setScissorExtent({ width, height });
	}
}

void FDR::setScissorOffset(const I32 x, const I32 y) {
	for (auto& z : Internal::Commands::MainScissor) {
		z->setScissorOffset({ x, y });
	}
}


FDR::ImGuiImage FDR::createImGuiImage(const char* filePath) {

	try {
		ImGuiImage image = Internal::createImGuiImage(filePath);
		Internal::GMessenger.add_str<FU::Log::Type::None>("ImGuiImage({}) was created successfully.", filePath);
		return image;
	}
	catch (const std::exception& e) {
		Internal::GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to create ImGuiImage. Exception message is {}.", e.what());
		Exception::Internal::throwFailed();
	}

}

ImFont* FDR::getDefaultFontMiniSize() {
	return FVK::getImGuiFonts(Internal::BaseImGuiKey).at(3);
}










#endif
