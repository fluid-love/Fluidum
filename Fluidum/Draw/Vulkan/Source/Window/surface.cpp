#include "surface.h"

FVK::Internal::Surface::Surface(ManagerPassKey, const Data::SurfaceData& data, const Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::Surface::create(const Data::SurfaceData& data, const Parameter& parameter) {
	vk::SurfaceKHR castSurface{};

#ifdef FluidumUtils_Type_OS_Windows

	vk::Win32SurfaceCreateInfoKHR createInfo{
		.hinstance = GetModuleHandle(NULL),
		.hwnd = data.get<FvkType::Window>().window,
	};

	const auto result = data.get<FvkType::Instance>().instance.createWin32SurfaceKHR(createInfo);

	if (result.result != vk::Result::eSuccess) {
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to create Surface.({}).", vk::to_string(result.result));
		Exception::throwFailedToCreate();
	}
	info.surface = result.value;

#endif 

	this->fillInfo(data);
}

const FVK::Internal::Data::SurfaceInfo& FVK::Internal::Surface::get() const noexcept {
	assert(this->info.surface);
	return this->info;
}

void FVK::Internal::Surface::destroy() noexcept {
	assert(this->info.surface);
	vkDestroySurfaceKHR(this->info.instance, info.surface, nullptr);
}

void FVK::Internal::Surface::fillInfo(const Data::SurfaceData& data) noexcept {
	this->info.window = data.get<FvkType::Window>().window;
	this->info.instance = data.get<FvkType::Instance>().instance;
}
