#include "FVKuniformbuffer.h"
#include "FVKbuffer.h"

FVK::Internal::UniformBuffer::UniformBuffer(ManagerPassKey, const Data::UniformBufferData& data, const Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::UniformBuffer::create(const Data::UniformBufferData& data, const Parameter& parameter) {
	vk::DeviceSize bufferSize = sizeof(Object);

	Buffer::createBuffer(
		data.get<FvkType::PhysicalDevice>().physicalDevice,
		data.get<FvkType::LogicalDevice>().device,
		bufferSize,
		vk::BufferUsageFlagBits::eUniformBuffer,
		vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
		info.uniformBuffer,
		info.uniformBufferMemory
		);

	this->info.device = data.get<FvkType::LogicalDevice>().device;
}

const FVK::Internal::Data::UniformBufferInfo& FVK::Internal::UniformBuffer::get() const noexcept {
	assert(this->info.uniformBuffer);
	return this->info;
}

void FVK::Internal::UniformBuffer::destroy() {
	assert(this->info.uniformBuffer);
	this->info.device.freeMemory(this->info.uniformBufferMemory);
	this->info.device.destroyBuffer(this->info.uniformBuffer);
}
