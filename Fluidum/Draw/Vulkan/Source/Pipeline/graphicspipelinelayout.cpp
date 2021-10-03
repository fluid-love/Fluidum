#include "graphicspipelinelayout.h"

FVK::Internal::GraphicsPipelineLayout::GraphicsPipelineLayout(ManagerPassKey, const Data::GraphicsPipelineLayoutData& data, Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::GraphicsPipelineLayout::create(const Data::GraphicsPipelineLayoutData& data, Parameter& parameter) {
	
	auto descriptorSetLayoutInfos = data.get<FvkType::DescriptorSetLayout_Vector>();
	std::vector<vk::DescriptorSetLayout> descriptorSetLayouts(descriptorSetLayoutInfos.size());
	for (std::size_t i = 0,size = descriptorSetLayoutInfos.size(); i < size; i++) {
		descriptorSetLayouts[i] = descriptorSetLayoutInfos[i].get().descriptorSetLayout;
	}

	parameter.pInfo->pSetLayouts = descriptorSetLayouts.data();
	auto result = data.get<FvkType::LogicalDevice>().device.createPipelineLayout(*parameter.pInfo);

	if (result.result != vk::Result::eSuccess)
		Exception::throwFailedToCreate("Failed to create GraphicsPipelineLayout");

	this->info.graphicsPipelineLayout = result.value;
	this->info.device = data.get<FvkType::LogicalDevice>().device;
}

const FVK::Internal::Data::GraphicsPipelineLayoutInfo& FVK::Internal::GraphicsPipelineLayout::get() const noexcept {
	assert(info.graphicsPipelineLayout);
	return this->info;
}

void FVK::Internal::GraphicsPipelineLayout::destroy() {
	assert(info.graphicsPipelineLayout);
	this->info.device.destroyPipelineLayout(info.graphicsPipelineLayout);
}
