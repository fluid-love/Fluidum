#include "FVKcommandbuffer.h"

FVK::Internal::CommandBuffer::CommandBuffer(ManagerPassKey, const Data::CommandBufferData& data, Parameter& parameter) {
	this->allocate(data, parameter);
}

void FVK::Internal::CommandBuffer::allocate(const Data::CommandBufferData& data, Parameter& parameter) {

	parameter.pInfo->commandPool = data.get<FvkType::CommandPool>().commandPool;

	auto result = data.get<FvkType::LogicalDevice>().device.allocateCommandBuffers(*parameter.pInfo);

	if (result.result != vk::Result::eSuccess)
		Exception::throwFailedToCreate("Failed to allocate CommandBuffer");
	this->info.commandBuffers = result.value;

	this->info.device = data.get<FvkType::LogicalDevice>().device;
	this->info.commandPool = data.get<FvkType::CommandPool>().commandPool;
}


const FVK::Internal::Data::CommandBufferInfo& FVK::Internal::CommandBuffer::get() const noexcept {
	assert(!this->info.commandBuffers.empty());
	return this->info;
}

void FVK::Internal::CommandBuffer::destroy() {
	assert(!this->info.commandBuffers.empty());
	this->info.device.freeCommandBuffers(this->info.commandPool, static_cast<uint32_t>(info.commandBuffers.size()), this->info.commandBuffers.data());
}