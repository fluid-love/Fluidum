#include "vertexbuffer.h"

FVK::Internal::VertexBuffer::VertexBuffer(ManagerPassKey, const Data::VertexBufferData& data, const Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::VertexBuffer::create(const Data::VertexBufferData& data, const Parameter& parameter) {
	vk::Buffer stagingBuffer = nullptr;
	vk::DeviceMemory stagingBufferMemory = nullptr;

	//size
	if (parameter.bufferSize < parameter.size) {
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "bufferSize is greater than vector size.");
		Exception::throwFailedToCreate();
	}

	this->info.vertexBufferSize = static_cast<vk::DeviceSize>(parameter.bufferSize);

	//strong
	Buffer::createBuffer(
		data.get<FvkType::PhysicalDevice>().physicalDevice,
		data.get<FvkType::LogicalDevice>().device,
		info.vertexBufferSize,
		vk::BufferUsageFlagBits::eTransferSrc,
		vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
		stagingBuffer,
		stagingBufferMemory
	);


	try {
		void* vkdata;
		auto result = data.get<FvkType::LogicalDevice>().device.mapMemory(stagingBufferMemory, 0, info.vertexBufferSize);
		if (result.result != vk::Result::eSuccess) {
			GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to map Memory({}).", vk::to_string(result.result));
			Exception::throwFailedToCreate();
		}
		vkdata = result.value;
		memcpy(vkdata, parameter.vertices, parameter.size);
	}
	catch (...) {
		//no-throw
		data.get<FvkType::LogicalDevice>().device.destroyBuffer(stagingBuffer);
		data.get<FvkType::LogicalDevice>().device.freeMemory(stagingBufferMemory);

		std::rethrow_exception(std::current_exception());
	}


	//no-throw
	data.get<FvkType::LogicalDevice>().device.unmapMemory(stagingBufferMemory);

	try {
		//strong
		Buffer::createBuffer(
			data.get<FvkType::PhysicalDevice>().physicalDevice,
			data.get<FvkType::LogicalDevice>().device,
			info.vertexBufferSize,
			vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
			vk::MemoryPropertyFlagBits::eDeviceLocal,
			info.vertexBuffer,
			info.vertexBufferMemory
		);
	}
	catch (...) {
		//no-throw
		data.get<FvkType::LogicalDevice>().device.destroyBuffer(stagingBuffer);
		data.get<FvkType::LogicalDevice>().device.freeMemory(stagingBufferMemory);

		std::rethrow_exception(std::current_exception());
	}


	//no-throw
	vk::Queue queue = data.get<FvkType::LogicalDevice>().device.getQueue(data.get<FvkType::PhysicalDevice>().graphicsFamily, 0);
	static_assert(std::is_nothrow_constructible_v<vk::Queue, vk::Queue&&>);


	try {
		//strong
		Buffer::copyBuffer(
			data.get<FvkType::LogicalDevice>().device,
			data.get<FvkType::CommandPool>().commandPool,
			queue, stagingBuffer,
			info.vertexBuffer,
			info.vertexBufferSize
		);
	}
	catch (...) {
		//no-throw
		data.get<FvkType::LogicalDevice>().device.destroyBuffer(stagingBuffer);
		data.get<FvkType::LogicalDevice>().device.freeMemory(stagingBufferMemory);
		this->destroy();

		std::rethrow_exception(std::current_exception());
	}

	//no-throw
	data.get<FvkType::LogicalDevice>().device.destroyBuffer(stagingBuffer);
	data.get<FvkType::LogicalDevice>().device.freeMemory(stagingBufferMemory);

	//no-throw
	this->info.device = data.get<FvkType::LogicalDevice>().device;
	static_assert(noexcept(info.device = data.get<FvkType::LogicalDevice>().device));

}

const FVK::Internal::Data::VertexBufferInfo& FVK::Internal::VertexBuffer::get() const noexcept {
	assert(this->info.vertexBuffer);
	return this->info;
}

void FVK::Internal::VertexBuffer::destroy() noexcept {
	assert(this->info.vertexBuffer && this->info.vertexBufferMemory);
	this->info.device.freeMemory(this->info.vertexBufferMemory);
	this->info.device.destroyBuffer(this->info.vertexBuffer);
}