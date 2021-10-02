#include "FVKfence.h"

FVK::Internal::Fence::Fence(ManagerPassKey, const Data::FenceData& data, const Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::Fence::create(const Data::FenceData& data, const Parameter& parameter) {

	auto result = data.get<FvkType::LogicalDevice>().device.createFence(*parameter.pInfo);

	if (result.result != vk::Result::eSuccess)
		Exception::throwFailedToCreate("Failed to create Fence");

	this->info.fence = result.value;

	this->info.device = data.get<FvkType::LogicalDevice>().device;
}

const FVK::Internal::Data::FenceInfo& FVK::Internal::Fence::get() const noexcept {
	assert(this->info.fence);
	return this->info;
}

void FVK::Internal::Fence::destroy() {
	assert(this->info.fence);
	this->info.device.destroyFence(this->info.fence);
}
