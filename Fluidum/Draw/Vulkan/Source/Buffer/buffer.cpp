#include "buffer.h"
#include "../PhysicalDevice/physicaldevice.h"

void FVK::Internal::Buffer::createBuffer(const vk::PhysicalDevice physicalDevice, const vk::Device device, const vk::DeviceSize size, const vk::BufferUsageFlags usage, const vk::MemoryPropertyFlags properties, vk::Buffer& buffer, vk::DeviceMemory& bufferMemory) {

	{
		vk::BufferCreateInfo bufferInfo = {
			.size = size,
			.usage = usage,
			.sharingMode = vk::SharingMode::eExclusive,
		};

		auto result = device.createBuffer(bufferInfo);

		if (result.result != vk::Result::eSuccess) {
			GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to create Buffer({}).", vk::to_string(result.result));
			Exception::throwFailedToCreate();
		}

		//no-throw
		buffer = result.value;
		static_assert(noexcept(buffer = result.value));
	}

	{
		//no-throw
		vk::MemoryRequirements memRequirements = device.getBufferMemoryRequirements(buffer);

		try {
			vk::MemoryAllocateInfo allocInfo = {
				.allocationSize = memRequirements.size,
				.memoryTypeIndex = PhysicalDevice::findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties),
			};

			auto result = device.allocateMemory(allocInfo);

			if (result.result != vk::Result::eSuccess) {
				GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to allocate Memory({}).", vk::to_string(result.result));
				Exception::throwFailedToCreate();
			}

			//no-throw
			bufferMemory = result.value;
			static_assert(noexcept(bufferMemory = result.value));
		}
		catch (const ::FVK::Exception::Error& e) {
			//no-throw
			if (e.code() == ::FVK::Exception::ErrorType::NotSupported)
				GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Required memory type is not supported.");
			else if (e.code() == ::FVK::Exception::ErrorType::FailedToCreate)
				;
			else
				GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Internal Error.");

			//no-throw
			device.destroyBuffer(buffer);

			std::rethrow_exception(std::current_exception());
		}
		catch (...) {
			//no-throw
			GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Internal Error.");

			//no-throw
			device.destroyBuffer(buffer);

			std::rethrow_exception(std::current_exception());
		}
	}

	{
		try {
			auto result = device.bindBufferMemory(buffer, bufferMemory, 0);

			if (result != vk::Result::eSuccess) {
				GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to bind Memory({}).", vk::to_string(result));
				Exception::throwFailedToCreate();
			}
		}
		catch (const ::FVK::Exception::Error& e) {
			assert(e.code() == ::FVK::Exception::ErrorType::FailedToCreate);

			//no-throw
			device.destroyBuffer(buffer);
			device.freeMemory(bufferMemory);

			std::rethrow_exception(std::current_exception());
		}
		catch (...) {
			//no-throw
			GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Internal Error.");

			//no-throw
			device.destroyBuffer(buffer);
			device.freeMemory(bufferMemory);

			std::rethrow_exception(std::current_exception());
		}
	}
}

VkCommandBuffer FVK::Internal::Buffer::beginSingleTimeCommands(const vk::Device device, const vk::CommandPool commandPool) {
	vk::CommandBufferAllocateInfo allocInfo = {
		.commandPool = commandPool,
		.level = vk::CommandBufferLevel::ePrimary,
		.commandBufferCount = 1,
	};

	std::vector<vk::CommandBuffer> commandBuffers{};
	{
		auto result = device.allocateCommandBuffers(allocInfo);

		if (result.result != vk::Result::eSuccess) {
			GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to allocate CommandBuffer({}).", vk::to_string(result.result));
			Exception::throwFailedToCreate();
		}

		//no-throw
		commandBuffers = std::move(result.value);
	}

	{
		try {
			vk::CommandBufferBeginInfo beginInfo = {
				.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit
			};

			assert(!commandBuffers.empty());
			auto result = commandBuffers.at(0).begin(beginInfo);

			if (result != vk::Result::eSuccess) {
				GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to allocate CommandBuffer({}).", vk::to_string(result));
				Exception::throwFailedToCreate();
			}
		}
		catch (const ::FVK::Exception::Error& e) {
			assert(e.code() == ::FVK::Exception::ErrorType::FailedToCreate);

			//no-throw
			device.freeCommandBuffers(commandPool, 1, commandBuffers.data());

			std::rethrow_exception(std::current_exception());
		}
		catch (...) {
			//no-throw
			GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Internal Error.");

			//no-throw
			device.freeCommandBuffers(commandPool, 1, commandBuffers.data());

			std::rethrow_exception(std::current_exception());
		}
	}

	return commandBuffers[0];
}

void FVK::Internal::Buffer::endSingleTimeCommands(const vk::CommandBuffer commandBuffer, const vk::Device device, const vk::CommandPool commandPool, const vk::Queue graphicsQueue) {
	{
		auto result = commandBuffer.end();

		if (result != vk::Result::eSuccess) {
			GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to end CommandBuffer({}).", vk::to_string(result));
			Exception::throwFailedToCreate();
		}
	}

	{
		vk::SubmitInfo submitInfo = {
			.commandBufferCount = 1,
			.pCommandBuffers = &commandBuffer,
		};

		//no-throw
		auto result = graphicsQueue.submit(1, &submitInfo, nullptr);
		static_assert(std::is_nothrow_constructible_v<vk::Result, vk::Result>);

		if (result != vk::Result::eSuccess) {
			GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to submit Queue({}).", vk::to_string(result));
			Exception::throwFailedToCreate();
		}

		result = graphicsQueue.waitIdle();

		if (result != vk::Result::eSuccess) {
			GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to waitIdle({}).", vk::to_string(result));
			Exception::throwFailedToCreate();
		}
	}

	device.freeCommandBuffers(commandPool, 1, &commandBuffer);
}

void FVK::Internal::Buffer::copyBuffer(const vk::Device device, const vk::CommandPool commandPool, const vk::Queue graphicsQueue, vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size) {
	vk::CommandBuffer commandBuffer = beginSingleTimeCommands(device, commandPool);

	vk::BufferCopy copyRegion = {
		.size = size
	};

	//no-throw
	commandBuffer.copyBuffer(srcBuffer, dstBuffer, 1, &copyRegion);

	//the commandbuffer is not released when an exception is thrown.
	try {
		endSingleTimeCommands(commandBuffer, device, commandPool, graphicsQueue);
	}
	catch (...) {
		device.freeCommandBuffers(commandPool, 1, &commandBuffer);
		std::rethrow_exception(std::current_exception());
	}
}
