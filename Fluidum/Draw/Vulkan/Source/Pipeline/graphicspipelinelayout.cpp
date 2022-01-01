#include "graphicspipelinelayout.h"

FVK::Internal::GraphicsPipelineLayout::GraphicsPipelineLayout(ManagerPassKey, const Data::GraphicsPipelineLayoutData& data, Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::GraphicsPipelineLayout::create(const Data::GraphicsPipelineLayoutData& data, Parameter& parameter) {
	if (!parameter.pInfo) {
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Parameter::pInfo == nullptr.");
		Exception::throwFailedToCreate();
	}

	auto descriptorSetLayoutInfos = data.get<FvkType::DescriptorSetLayout_Vector>();
	std::vector<vk::DescriptorSetLayout> descriptorSetLayouts(descriptorSetLayoutInfos.size());
	for (Size i = 0, size = descriptorSetLayoutInfos.size(); i < size; i++) {
		descriptorSetLayouts[i] = descriptorSetLayoutInfos[i].get().descriptorSetLayout;
	}

	parameter.pInfo->pSetLayouts = descriptorSetLayouts.data();

	try {
		auto result = data.get<FvkType::LogicalDevice>().device.createPipelineLayout(*parameter.pInfo);
		if (result.result != vk::Result::eSuccess) {
			GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to create GraphicsPipelineLayout({}).", vk::to_string(result.result));
			Exception::throwFailedToCreate();
		}

		//no-throw
		this->info.graphicsPipelineLayout = result.value;
		this->info.device = data.get<FvkType::LogicalDevice>().device;

		static_assert(noexcept(info.graphicsPipelineLayout = result.value));
		static_assert(noexcept(info.device = data.get<FvkType::LogicalDevice>().device));
	}
	catch (...) {
		//Guarantees strong exception safety
		parameter.pInfo->pSetLayouts = {};//pSetLayouts is pointer.
		std::rethrow_exception(std::current_exception());
	}
}

const FVK::Internal::Data::GraphicsPipelineLayoutInfo& FVK::Internal::GraphicsPipelineLayout::get() const noexcept {
	assert(info.graphicsPipelineLayout);
	return this->info;
}

void FVK::Internal::GraphicsPipelineLayout::destroy() noexcept {
	assert(info.graphicsPipelineLayout);
	this->info.device.destroyPipelineLayout(info.graphicsPipelineLayout);
}
