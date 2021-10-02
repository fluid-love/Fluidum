#include "FVKindexbuffer.h"
#include "FVKbuffer.h"

FVK::Internal::IndexBuffer::IndexBuffer(ManagerPassKey, const Data::IndexBufferData& data, const Parameter& parameter) { 
	this->create(data,parameter);
}

void FVK::Internal::IndexBuffer::create(const Data::IndexBufferData& data, const Parameter& parameter) {
	
	vk::Buffer stagingBuffer = nullptr;
	vk::DeviceMemory stagingBufferMemory = nullptr;

	//size	
	if (parameter.bufferSize < parameter.size)
		throw std::runtime_error("BufferSize < VerticesSize");
	this->info.indexBufferSize = static_cast<vk::DeviceSize>(parameter.bufferSize);

	Buffer::createBuffer(
		data.get<FvkType::PhysicalDevice>().physicalDevice,
		data.get<FvkType::LogicalDevice>().device,
		info.indexBufferSize,
		vk::BufferUsageFlagBits::eTransferSrc,
		vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
		stagingBuffer,
		stagingBufferMemory
	);

	void* vkdata;
	auto result = data.get<FvkType::LogicalDevice>().device.mapMemory(stagingBufferMemory, 0, info.indexBufferSize);
	if (result.result != vk::Result::eSuccess)
		Exception::throwFailedToCreate("Failed to map memory");
	vkdata = result.value;
	memcpy(vkdata, parameter.indices, parameter.size);

	data.get<FvkType::LogicalDevice>().device.unmapMemory(stagingBufferMemory);

	Buffer::createBuffer(
		data.get<FvkType::PhysicalDevice>().physicalDevice,
		data.get<FvkType::LogicalDevice>().device,
		info.indexBufferSize,
		vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
		vk::MemoryPropertyFlagBits::eDeviceLocal,
		info.indexBuffer,
		info.indexBufferMemory
	);

	vk::Queue queue = data.get<FvkType::LogicalDevice>().device.getQueue(data.get<FvkType::PhysicalDevice>().graphicsFamily, 0);

	Buffer::copyBuffer(
		data.get<FvkType::LogicalDevice>().device,
		data.get<FvkType::CommandPool>().commandPool,
		queue, stagingBuffer,
		info.indexBuffer,
		info.indexBufferSize
	);

	data.get<FvkType::LogicalDevice>().device.destroyBuffer(stagingBuffer);
	data.get<FvkType::LogicalDevice>().device.freeMemory(stagingBufferMemory);

	this->info.device = data.get<FvkType::LogicalDevice>().device;
}

const FVK::Internal::Data::IndexBufferInfo& FVK::Internal::IndexBuffer::get() const noexcept {
	assert(this->info.indexBuffer && this->info.indexBufferMemory);
	return this->info;
}

void FVK::Internal::IndexBuffer::destroy() {
	assert(this->info.indexBuffer && this->info.indexBufferMemory);
	this->info.device.freeMemory(this->info.indexBufferMemory);
	this->info.device.destroyBuffer(this->info.indexBuffer);
}
