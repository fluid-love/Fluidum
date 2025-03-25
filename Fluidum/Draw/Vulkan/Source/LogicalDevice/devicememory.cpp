#include "devicememory.h"

FVK::Internal::DeviceMemory::DeviceMemory(ManagerPassKey, const Data::DeviceMemoryData& data, const Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::DeviceMemory::create(const Data::DeviceMemoryData& data, const Parameter& parameter) {

	auto result = data.get<FvkType::LogicalDevice>().device.allocateMemory(*parameter.pInfo);
	if (result.result != vk::Result::eSuccess) {
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to allocate Memory({}).", vk::to_string(result.result));
		Exception::throwFailedToCreate();
	}

	this->info.deviceMemory = result.value;
	this->info.device = data.get<FvkType::LogicalDevice>().device;

	static_assert(noexcept(info.deviceMemory = result.value));
	static_assert(noexcept(info.device = data.get<FvkType::LogicalDevice>().device));

}

const FVK::Internal::Data::DeviceMemoryInfo& FVK::Internal::DeviceMemory::get() const noexcept {
	assert(this->info.deviceMemory);
	return this->info;
}

void FVK::Internal::DeviceMemory::destroy() noexcept {
	assert(this->info.deviceMemory);
	this->info.device.freeMemory(this->info.deviceMemory);
}
