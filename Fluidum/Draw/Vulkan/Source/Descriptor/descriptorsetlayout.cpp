#include "descriptorsetlayout.h"

FVK::Internal::DescriptorSetLayout::DescriptorSetLayout(ManagerPassKey, const Data::DescriptorSetLayoutData data, const Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::DescriptorSetLayout::create(const Data::DescriptorSetLayoutData data, const Parameter& parameter) {

	auto result = data.get<FvkType::LogicalDevice>().device.createDescriptorSetLayout(*parameter.pInfo);

	if (result.result != vk::Result::eSuccess) {
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to create DescriptorSetLayout({}).", vk::to_string(result.result));
		Exception::throwFailedToCreate();
	}

	//no-throw
	this->info.descriptorSetLayout = result.value;
	static_assert(noexcept(info.descriptorSetLayout = result.value));

	//no-throw
	this->info.device = data.get<FvkType::LogicalDevice>().device;
	static_assert(noexcept(info.device = data.get<FvkType::LogicalDevice>().device));
}

const FVK::Internal::Data::DescriptorSetLayoutInfo& FVK::Internal::DescriptorSetLayout::get() const noexcept {
	assert(info.descriptorSetLayout);
	return this->info;
}

void FVK::Internal::DescriptorSetLayout::destroy() noexcept {
	assert(info.descriptorSetLayout);
	this->info.device.destroyDescriptorSetLayout(this->info.descriptorSetLayout);
}