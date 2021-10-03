#include "renderpass.h"

FVK::Internal::RenderPass::RenderPass(ManagerPassKey, const Data::RenderPassData& data, const Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::RenderPass::create(const Data::RenderPassData& data, const Parameter& parameter) {

	auto result = data.get<FvkType::LogicalDevice>().device.createRenderPass(*parameter.pInfo);

	if (result.result != vk::Result::eSuccess)
		Exception::throwFailedToCreate("Failed to create RenderPass");

	this->info.renderPass = result.value;

	this->info.device = data.get<FvkType::LogicalDevice>().device;
}

const FVK::Internal::Data::RenderPassInfo& FVK::Internal::RenderPass::get() const noexcept {
	assert(this->info.renderPass);
	return this->info;
}

void FVK::Internal::RenderPass::destroy() {
	assert(this->info.renderPass);
	this->info.device.destroyRenderPass(this->info.renderPass);
}
