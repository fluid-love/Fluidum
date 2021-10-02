#include "FVKdescriptorsetlayout.h"

void FVK::Internal::DescriptorSetLayout::create(const Data::DescriptorSetLayoutData data, const Parameter& parameter) {

	auto result = data.get<FvkType::LogicalDevice>().device.createDescriptorSetLayout(*parameter.pInfo);

	if (result.result != vk::Result::eSuccess)
		Exception::throwFailedToCreate("Failed to create DescriptorSetLayout");

	this->info.descriptorSetLayout = result.value;

	this->info.device = data.get<FvkType::LogicalDevice>().device;
}

FVK::Internal::DescriptorSetLayout::DescriptorSetLayout(ManagerPassKey, const Data::DescriptorSetLayoutData data, const Parameter& parameter) {
	this->create(data, parameter);
}

const FVK::Internal::Data::DescriptorSetLayoutInfo& FVK::Internal::DescriptorSetLayout::get() const noexcept {
	assert(info.descriptorSetLayout);
	return this->info;
}

void FVK::Internal::DescriptorSetLayout::destroy() {
	assert(info.descriptorSetLayout);
	this->info.device.destroyDescriptorSetLayout(this->info.descriptorSetLayout);
}