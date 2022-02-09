#include "fence.h"

FVK::Internal::Fence::Fence(ManagerPassKey, const Data::FenceData& data, const Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::Fence::create(const Data::FenceData& data, const Parameter& parameter) {

	auto result = data.get<FvkType::LogicalDevice>().device.createFence(*parameter.pInfo);
	if (result.result != vk::Result::eSuccess) {
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to create Fence({}).", vk::to_string(result.result));
		Exception::throwFailedToCreate();
	}

	//no-throw
	this->info.fence = result.value;
	this->info.device = data.get<FvkType::LogicalDevice>().device;

	static_assert(noexcept(info.fence = result.value));
	static_assert(noexcept(info.device = data.get<FvkType::LogicalDevice>().device));

}

const FVK::Internal::Data::FenceInfo& FVK::Internal::Fence::get() const noexcept {
	assert(this->info.fence);
	return this->info;
}

void FVK::Internal::Fence::destroy() noexcept {
	assert(this->info.fence);
	this->info.device.destroyFence(this->info.fence);
}
