#include "commandbuffer.h"

FVK::Internal::CommandBuffer::CommandBuffer(ManagerPassKey, const Data::CommandBufferData& data, Parameter& parameter) {
	this->allocate(data, parameter);
}

void FVK::Internal::CommandBuffer::allocate(const Data::CommandBufferData& data, Parameter& parameter) {

	parameter.pInfo->commandPool = data.get<FvkType::CommandPool>().commandPool;

	auto result = data.get<FvkType::LogicalDevice>().device.allocateCommandBuffers(*parameter.pInfo);

	if (result.result != vk::Result::eSuccess) {
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to allocate CommandBuffers({}).", vk::to_string(result.result));
		Exception::throwFailedToCreate();
	}

	//no-throw
	this->info.commandBuffers = std::move(result.value);

	//no-throw
	this->info.device = data.get<FvkType::LogicalDevice>().device;
	this->info.commandPool = data.get<FvkType::CommandPool>().commandPool;

	static_assert(noexcept(info.device = data.get<FvkType::LogicalDevice>().device));
	static_assert(noexcept(info.commandPool = data.get<FvkType::CommandPool>().commandPool));

}

const FVK::Internal::Data::CommandBufferInfo& FVK::Internal::CommandBuffer::get() const noexcept {
	assert(!this->info.commandBuffers.empty());
	return this->info;
}

void FVK::Internal::CommandBuffer::destroy() noexcept {
	assert(!this->info.commandBuffers.empty());
	this->info.device.freeCommandBuffers(this->info.commandPool, static_cast<uint32_t>(info.commandBuffers.size()), this->info.commandBuffers.data());
}