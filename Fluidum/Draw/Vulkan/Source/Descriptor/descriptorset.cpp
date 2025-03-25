#include "descriptorset.h"

FVK::Internal::DescriptorSet::DescriptorSet(ManagerPassKey, const Data::DescriptorSetData& data, Parameter& parameter) {
	this->allocate(data, parameter);
}

void FVK::Internal::DescriptorSet::allocate(const Data::DescriptorSetData& data, Parameter& parameter) {

	parameter.pInfo->descriptorPool = data.get<FvkType::DescriptorPool>().descriptorPool;

	const auto& descriptorSetLayoutRefs = data.get<FvkType::DescriptorSetLayout_Vector>();

	std::vector<vk::DescriptorSetLayout> descriptorSetLayouts(descriptorSetLayoutRefs.size());
	for (FU::Type::VecSize<decltype(descriptorSetLayouts)::value_type> i = 0, size = descriptorSetLayoutRefs.size(); i < size; i++) {
		descriptorSetLayouts[i] = descriptorSetLayoutRefs[i].get().descriptorSetLayout;
	}

	parameter.pInfo->pSetLayouts = descriptorSetLayouts.data();

	{
		auto result = data.get<FvkType::LogicalDevice>().device.allocateDescriptorSets(*parameter.pInfo);

		if (result.result != vk::Result::eSuccess) {
			GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to allocate DescriptorSets({}).", vk::to_string(result.result));
			Exception::throwFailedToCreate();
		}

		assert(!result.value.empty());

		//no-throw
		this->info.descriptorSet = result.value[0];
		static_assert(noexcept(info.descriptorSet = result.value[0]));
	}

	//no-throw
	this->info.device = data.get<FvkType::LogicalDevice>().device;
	this->info.descriptorPool = data.get<FvkType::DescriptorPool>().descriptorPool;

	static_assert(noexcept(info.device = data.get<FvkType::LogicalDevice>().device));
	static_assert(noexcept(info.descriptorPool = data.get<FvkType::DescriptorPool>().descriptorPool));

}

const FVK::Internal::Data::DescriptorSetInfo& FVK::Internal::DescriptorSet::get() const noexcept {
	assert(this->info.descriptorSet);
	return this->info;
}

void FVK::Internal::DescriptorSet::destroy() noexcept {
	assert(this->info.descriptorSet);
	auto result = this->info.device.freeDescriptorSets(this->info.descriptorPool, this->info.descriptorSet);
	if (result != vk::Result::eSuccess) {
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to free DescriptorSets({}).", vk::to_string(result));
		Exception::throwFailedToDestroy();
	}
}
