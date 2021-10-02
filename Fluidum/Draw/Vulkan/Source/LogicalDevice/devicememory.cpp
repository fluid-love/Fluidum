#include "FVKdevicememory.h"

FVK::Internal::DeviceMemory::DeviceMemory(ManagerPassKey, const Data::DeviceMemoryData& data, const Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::DeviceMemory::create(const Data::DeviceMemoryData& data, const Parameter& parameter) {

	auto result = data.get<FvkType::LogicalDevice>().device.allocateMemory(*parameter.pInfo);

	if (result.result != vk::Result::eSuccess)
		Exception::throwFailedToCreate("Failed to allocate memory");

	this->info.deviceMemory = result.value;

	this->info.device = data.get<FvkType::LogicalDevice>().device;
}

const FVK::Internal::Data::DeviceMemoryInfo& FVK::Internal::DeviceMemory::get() const noexcept {
	assert(this->info.deviceMemory);
	return this->info;
}

void FVK::Internal::DeviceMemory::destroy() {
	assert(this->info.deviceMemory);
	this->info.device.freeMemory(this->info.deviceMemory);
}
