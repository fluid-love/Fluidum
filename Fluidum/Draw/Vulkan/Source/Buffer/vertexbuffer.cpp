#include "FVKvertexbuffer.h"

FVK::Internal::VertexBuffer::VertexBuffer(ManagerPassKey, const Data::VertexBufferData& data,const Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::VertexBuffer::create(const Data::VertexBufferData& data,const Parameter& parameter) {
	vk::Buffer stagingBuffer = nullptr;
	vk::DeviceMemory stagingBufferMemory = nullptr;

	//size	
	if (parameter.bufferSize < parameter.size)
		throw std::runtime_error("BufferSize < VerticesSize");
	this->info.vertexBufferSize = static_cast<vk::DeviceSize>(parameter.bufferSize);

	Buffer::createBuffer(
		data.get<FvkType::PhysicalDevice>().physicalDevice,
		data.get<FvkType::LogicalDevice>().device,
		info.vertexBufferSize,
		vk::BufferUsageFlagBits::eTransferSrc,
		vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
		stagingBuffer,
		stagingBufferMemory
	);

	void* vkdata;
	auto result = data.get<FvkType::LogicalDevice>().device.mapMemory(stagingBufferMemory, 0, info.vertexBufferSize);
	if (result.result != vk::Result::eSuccess)
		Exception::throwFailedToCreate("Failed to map memory");
	vkdata = result.value;
	memcpy(vkdata, parameter.vertices, parameter.size);

	data.get<FvkType::LogicalDevice>().device.unmapMemory(stagingBufferMemory);

	Buffer::createBuffer(
		data.get<FvkType::PhysicalDevice>().physicalDevice,
		data.get<FvkType::LogicalDevice>().device,
		info.vertexBufferSize,
		vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
		vk::MemoryPropertyFlagBits::eDeviceLocal,
		info.vertexBuffer,
		info.vertexBufferMemory
	);

	vk::Queue queue = data.get<FvkType::LogicalDevice>().device.getQueue(data.get<FvkType::PhysicalDevice>().graphicsFamily, 0);

	Buffer::copyBuffer(
		data.get<FvkType::LogicalDevice>().device,
		data.get<FvkType::CommandPool>().commandPool,
		queue, stagingBuffer,
		info.vertexBuffer,
		info.vertexBufferSize
	);

	data.get<FvkType::LogicalDevice>().device.destroyBuffer(stagingBuffer);
	data.get<FvkType::LogicalDevice>().device.freeMemory(stagingBufferMemory);

	this->info.device = data.get<FvkType::LogicalDevice>().device;
}

const FVK::Internal::Data::VertexBufferInfo& FVK::Internal::VertexBuffer::get() const noexcept {
	assert(this->info.vertexBuffer);
	return this->info;
}

void FVK::Internal::VertexBuffer::destroy() {
	assert(this->info.vertexBuffer && this->info.vertexBufferMemory);
	this->info.device.freeMemory(this->info.vertexBufferMemory);
	this->info.device.destroyBuffer(this->info.vertexBuffer);
}