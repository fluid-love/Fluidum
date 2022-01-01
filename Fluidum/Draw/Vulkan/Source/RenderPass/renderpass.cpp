#include "renderpass.h"

FVK::Internal::RenderPass::RenderPass(ManagerPassKey, const Data::RenderPassData& data, const Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::RenderPass::create(const Data::RenderPassData& data, const Parameter& parameter) {

	auto result = data.get<FvkType::LogicalDevice>().device.createRenderPass(*parameter.pInfo);

	if (result.result != vk::Result::eSuccess) {
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to create RenderPass({}).", vk::to_string(result.result));
		Exception::throwFailedToCreate();
	}

	//no-throw
	this->info.renderPass = result.value;
	static_assert(noexcept(info.renderPass = result.value));

	//no-throw
	this->info.device = data.get<FvkType::LogicalDevice>().device;
	static_assert(noexcept(info.device = data.get<FvkType::LogicalDevice>().device));
}

const FVK::Internal::Data::RenderPassInfo& FVK::Internal::RenderPass::get() const noexcept {
	assert(this->info.renderPass);
	return this->info;
}

void FVK::Internal::RenderPass::destroy() noexcept {
	assert(this->info.renderPass);
	this->info.device.destroyRenderPass(this->info.renderPass);
	static_assert(noexcept(info.device.destroyRenderPass(info.renderPass)));
}
