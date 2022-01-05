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

void FDR::miximizeWindow() {
	FVK::maximizeWindow(Internal::BaseWindowKey);
}

void FDR::minimizeWindow() {
	FVK::minimizeWindow(Internal::BaseWindowKey);
}

FDR::ImGuiImage FDR::createImGuiImage(const char* filePath) {
	try {
		ImGuiImage image = Internal::createImGuiImage(filePath);
		Internal::GMessenger.add_str<FU::Log::Type::None>("ImGuiImage({}) was created successfully.", filePath);
		return image;
	}
	catch (const std::exception& e) {
		Internal::GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__,"Failed to create ImGuiImage. Exception message is {}.", e.what());
		Exception::Internal::throwFailed();
	}
}

ImFont* FDR::getDefaultFontMiniSize() {
	return FVK::getImGuiFonts(Internal::BaseImGuiKey).at(3);
}










#endif
