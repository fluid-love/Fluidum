#include "graphicspipeline.h"

FVK::Internal::GraphicsPipeline::GraphicsPipeline(ManagerPassKey, const Data::GraphicsPipelineData& data, Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::GraphicsPipeline::create(const Data::GraphicsPipelineData& data, Parameter& parameter) {
	this->fillParameter(data, parameter);

	auto result = data.get<FvkType::LogicalDevice>().device.createGraphicsPipeline(nullptr, *parameter.pInfo);
	if (result.result != vk::Result::eSuccess) {
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to create GraphicsPipeline({}).", vk::to_string(result.result));
		Exception::throwFailedToCreate();
	}

	//no-throw
	this->info.graphicsPipeline = result.value;
	this->info.device = data.get<FvkType::LogicalDevice>().device;

	static_assert(noexcept(info.graphicsPipeline = result.value));
	static_assert(noexcept(info.device = data.get<FvkType::LogicalDevice>().device));
}

const FVK::Internal::Data::GraphicsPipelineInfo& FVK::Internal::GraphicsPipeline::get() const noexcept {
	assert(info.graphicsPipeline);
	return this->info;
}

void FVK::Internal::GraphicsPipeline::destroy() noexcept {
	assert(info.graphicsPipeline);
	this->info.device.destroyPipeline(this->info.graphicsPipeline);
}

void FVK::Internal::GraphicsPipeline::fillParameter(const Data::GraphicsPipelineData& data, Parameter& parameter) {
	parameter.pInfo->layout = data.get<FvkType::GraphicsPipelineLayout>().graphicsPipelineLayout;
	parameter.pInfo->renderPass = data.get<FvkType::RenderPass>().renderPass;

	//no-throw
	const auto& shaderModuleInfos = data.get<FvkType::ShaderModule_Vector>();
	const auto size = shaderModuleInfos.size();
	assert(parameter.pInfo->stageCount == size);

	for (Size i = 0; i < size; i++) {
		parameter.pInfo->pStageCreateInfos[i].module = shaderModuleInfos[i].get().shaderModule;
	}

	parameter.pInfo->pStages = alignPipelineShaderStageCreateInfo(parameter, size);
}

const vk::PipelineShaderStageCreateInfo* FVK::Internal::GraphicsPipeline::alignPipelineShaderStageCreateInfo(const Parameter& parameter, const std::size_t size) {
	for (std::size_t i = 0; i < size; i++)
		align.emplace_back(parameter.pInfo->pStageCreateInfos[i]);
	return align.data();
}
