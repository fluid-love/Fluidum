#include "api.h"

#ifdef FluidumDraw_Vulkan

namespace FDR::Internal {
	void messengerCallback(const std::string& message) {
		FDR::Internal::GMessenger.callCallback(message);
	}
}

void FDR::mainLoop(const char* title, void(*function)(), bool* const windowCloseFlag) {
	try {
		//FluidumVKのメッセージをFluidumDrawのメッセージとして呼びなおす
		FVK::setMessengerCallbackFunction(Internal::messengerCallback);

		//FluidumVKの初期化
		FVK::initialize();

		FDR::Internal::Initialization::FullScreenWithBarInfo info;
		info.title = title;
		FDR::Internal::Initialization::initialize(info);

		//FU::Log::Mesengerはthread safe
		Internal::GMessenger.add<FU::Log::Type::None>("Initialized FluidumDraw.");

		//loop
		FDR::Internal::mainLoop(function, windowCloseFlag);

		try {
			FVK::terminate();
		}
		catch (const std::exception& e) {
			Internal::GMessenger.add<FU::Log::Type::Error>(e.what());
			throw Exception::Error();
		}

		Internal::GMessenger.add<FU::Log::Type::None>("Terminated FluidumDraw.");
	}
	catch (const std::exception& e) {
		Internal::GMessenger.add<FU::Log::Type::Error>(e.what());
		throw Exception::Error();
	}
	catch (...) {
		std::rethrow_exception(std::current_exception());
	}
}

void FDR::setMessengerCallbackFunction(const MessengerCallbackType callback) {
	Internal::GMessenger.setCallback(callback);
}

std::pair<int32_t, int32_t> FDR::getWindowSize() {
	return FVK::getWindowSize(Internal::BaseWindowKey);
}

std::pair<int32_t, int32_t> FDR::getWindowPos() {
	return FVK::getWindowPos(Internal::BaseWindowKey);
}

void FDR::setWindowSize(const int32_t width, const int32_t height) {
	FVK::setWindowSize(Internal::BaseWindowKey, width, height);
}

void FDR::setWindowSize(const ImVec2& size) {
	setWindowSize(static_cast<int32_t>(size.x), static_cast<int32_t>(size.y));
}

void FDR::setWindowPos(const int32_t x, const int32_t y) {
	FVK::setWindowPos(Internal::BaseWindowKey, x, y);
}

void FDR::setWindowPos(const ImVec2& pos) {
	setWindowPos(static_cast<int32_t>(pos.x), static_cast<int32_t>(pos.y));
}

void FDR::setWindowSizePos(const int32_t width, const int32_t height, const int32_t x, const int32_t y) {
	setWindowSize(width, height);
	setWindowPos(x, y);
}

void FDR::setWindowSizePos(const ImVec2& size, const ImVec2& pos) {
	setWindowSize(size);
	setWindowPos(pos);
}

void FDR::miximizeWindow() {
	FVK::maximizeWindow(Internal::BaseWindowKey);
}

void FDR::minimizeWindow() {
	FVK::minimizeWindow(Internal::BaseWindowKey);
}

FDR::ImGuiImage FDR::createImGuiImage(const char* filePath) {
	try {
		ImGuiImage image = Internal::createImGuiImage(filePath);
		Internal::GMessenger.add<FU::Log::Type::None>("ImGuiImage was created successfully. Filepath is \"{}\".", filePath);
		return image;
	}
	catch (const std::exception& e) {
		Internal::GMessenger.add<FU::Log::Type::Error>(e.what());
		throw Exception::Error();
	}
}

ImFont* FDR::getDefaultFontMiniSize() {
	return FVK::getImGuiFonts(Internal::BaseImGuiKey).at(3);
}






















#endif
