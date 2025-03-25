#include "indexbuffer.h"
#include "buffer.h"

FVK::Internal::IndexBuffer::IndexBuffer(ManagerPassKey, const Data::IndexBufferData& data, const Parameter& parameter) { 
	this->create(data,parameter);
}

void FVK::Internal::IndexBuffer::create(const Data::IndexBufferData& data, const Parameter& parameter) {
	
	vk::Buffer stagingBuffer = nullptr;
	vk::DeviceMemory stagingBufferMemory = nullptr;

	//size	
	if (parameter.bufferSize < parameter.size) {
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "bufferSize is greater than vector size.");
		Exception::throwFailedToCreate();
	}

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

	try {
		void* vkdata;
		auto result = data.get<FvkType::LogicalDevice>().device.mapMemory(stagingBufferMemory, 0, info.indexBufferSize);
		if (result.result != vk::Result::eSuccess) {
			GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to map Memory({}).", vk::to_string(result.result));
			Exception::throwFailedToCreate();
		}
		vkdata = result.value;
		memcpy(vkdata, parameter.indices, parameter.size);
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
			info.indexBufferSize,
			vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
			vk::MemoryPropertyFlagBits::eDeviceLocal,
			info.indexBuffer,
			info.indexBufferMemory
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
			info.indexBuffer,
			info.indexBufferSize
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

const FVK::Internal::Data::IndexBufferInfo& FVK::Internal::IndexBuffer::get() const noexcept {
	assert(this->info.indexBuffer && this->info.indexBufferMemory);
	return this->info;
}

void FVK::Internal::IndexBuffer::destroy() noexcept {
	assert(this->info.indexBuffer && this->info.indexBufferMemory);
	this->info.device.freeMemory(this->info.indexBufferMemory);
	this->info.device.destroyBuffer(this->info.indexBuffer);
}
