#include "descriptorpool.h"

FVK::Internal::DescriptorPool::DescriptorPool(ManagerPassKey, const Data::DescriptorPoolData& data, const Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::DescriptorPool::create(const Data::DescriptorPoolData& data, const Parameter& parameter) {

	auto result = data.get<FvkType::LogicalDevice>().device.createDescriptorPool(*parameter.pInfo);

	if (result.result != vk::Result::eSuccess)
		Exception::throwFailedToCreate("Failed to create uniform buffer");

	this->info.descriptorPool = result.value;

	this->info.device = data.get<FvkType::LogicalDevice>().device;
}

const FVK::Internal::Data::DescriptorPoolInfo& FVK::Internal::DescriptorPool::get() const noexcept {
	assert(this->info.descriptorPool);
	return this->info;
}

void FVK::Internal::DescriptorPool::destroy() noexcept {
	assert(this->info.descriptorPool);
	this->info.device.destroyDescriptorPool(this->info.descriptorPool);
}
