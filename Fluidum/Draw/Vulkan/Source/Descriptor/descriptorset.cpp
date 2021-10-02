#include "FVKdescriptorset.h"

FVK::Internal::DescriptorSet::DescriptorSet(ManagerPassKey, const Data::DescriptorSetData& data, Parameter& parameter) {
	this->allocate(data, parameter);
}

void FVK::Internal::DescriptorSet::allocate(const Data::DescriptorSetData& data, Parameter& parameter) {

	parameter.pInfo->descriptorPool = data.get<FvkType::DescriptorPool>().descriptorPool;

	const auto& descriptorSetLayoutRefs = data.get<FvkType::DescriptorSetLayout_Vector>();

	std::vector<vk::DescriptorSetLayout> descriptorSetLayouts(descriptorSetLayoutRefs.size());
	for (std::size_t i = 0, size = descriptorSetLayoutRefs.size(); i < size; i++) {
		descriptorSetLayouts[i] = descriptorSetLayoutRefs[i].get().descriptorSetLayout;
	}
	parameter.pInfo->pSetLayouts = descriptorSetLayouts.data();

	{
		auto result = data.get<FvkType::LogicalDevice>().device.allocateDescriptorSets(*parameter.pInfo);

		if (result.result != vk::Result::eSuccess)
			Exception::throwFailedToCreate("Failed to allocate descriptorsets");

		this->info.descriptorSet = result.value.at(0);
	}

	this->info.device = data.get<FvkType::LogicalDevice>().device;
	this->info.descriptorPool = data.get<FvkType::DescriptorPool>().descriptorPool;
}

const FVK::Internal::Data::DescriptorSetInfo& FVK::Internal::DescriptorSet::get() const noexcept {
	assert(this->info.descriptorSet);
	return this->info;
}

void FVK::Internal::DescriptorSet::destroy() {
	assert(this->info.descriptorSet);
	auto result = this->info.device.freeDescriptorSets(this->info.descriptorPool, this->info.descriptorSet);
	if (result != vk::Result::eSuccess)
		Exception::throwFailedToDestroy("Failed to free descriptorset");
}
