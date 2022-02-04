#include "surface.h"

FVK::Internal::Surface::Surface(ManagerPassKey, const Data::SurfaceData& data, const Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::Surface::create(const Data::SurfaceData& data, const Parameter& parameter) {
	VkSurfaceKHR castSurface{};

	auto result = glfwCreateWindowSurface(data.get<FvkType::Instance>().instance, data.get<FvkType::Window>().window, nullptr, &castSurface);
	if (result != VK_SUCCESS) {
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to create Surface.({}).", vk::to_string(vk::Result(result)));
		Exception::throwFailedToCreate();
	}
	
	//recast
	info.surface = castSurface;

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
