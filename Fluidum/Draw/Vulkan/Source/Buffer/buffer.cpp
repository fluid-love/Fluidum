#include "buffer.h"
#include "../PhysicalDevice/physicaldevice.h"

void FVK::Internal::Buffer::createBuffer(const vk::PhysicalDevice physicalDevice, const vk::Device device, const vk::DeviceSize size, const vk::BufferUsageFlags usage, const vk::MemoryPropertyFlags properties, vk::Buffer& buffer, vk::DeviceMemory& bufferMemory) {

	vk::BufferCreateInfo bufferInfo = {
		.size = size,
		.usage = usage,
		.sharingMode = vk::SharingMode::eExclusive,
	};

	{
		auto result = device.createBuffer(bufferInfo);

		if (result.result != vk::Result::eSuccess)
			Exception::throwFailedToCreate("Failed to create buffer");

		buffer = result.value;
	}


	vk::MemoryRequirements memRequirements = device.getBufferMemoryRequirements(buffer);

	vk::MemoryAllocateInfo allocInfo = {
		.allocationSize = memRequirements.size,
		.memoryTypeIndex = PhysicalDevice::findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties),
	};

	{
		auto result = device.allocateMemory(allocInfo);

		if (result.result != vk::Result::eSuccess)
			Exception::throwFailedToCreate("Failed to allocate memory");

		bufferMemory = result.value;
	}

	{
		auto result = device.bindBufferMemory(buffer, bufferMemory, 0);

		if (result != vk::Result::eSuccess)
			Exception::throwFailedToCreate("Failed to bind buffer memory");
	}
}

VkCommandBuffer FVK::Internal::Buffer::beginSingleTimeCommands(const vk::Device device, const vk::CommandPool commandPool) {
	vk::CommandBufferAllocateInfo allocInfo = {
		.commandPool = commandPool,
		.level = vk::CommandBufferLevel::ePrimary,
		.commandBufferCount = 1,
	};

	std::vector<vk::CommandBuffer> commandBuffer = {};
	{
		auto result = device.allocateCommandBuffers(allocInfo);

		if (result.result != vk::Result::eSuccess)
			throw std::runtime_error("");

		commandBuffer = result.value;
	}


	vk::CommandBufferBeginInfo beginInfo = {
		.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit
	};

	{
		auto result = commandBuffer[0].begin(beginInfo);

		if (result != vk::Result::eSuccess)
			throw std::runtime_error("");
	}


	return commandBuffer[0];
}

void FVK::Internal::Buffer::endSingleTimeCommands(const vk::CommandBuffer commandBuffer, const vk::Device device, const vk::CommandPool commandPool, const vk::Queue graphicsQueue) {
	{
		auto result = commandBuffer.end();

		if (result != vk::Result::eSuccess)
			Exception::throwFailedToCreate("Failed to end command buffer");
	}

	vk::SubmitInfo submitInfo = {
		.commandBufferCount = 1,
		.pCommandBuffers = &commandBuffer,
	};

	{
		auto result = graphicsQueue.submit(1, &submitInfo, nullptr);

		if (result != vk::Result::eSuccess)
			Exception::throwFailedToCreate("Failed to submit queue");

		result = graphicsQueue.waitIdle();

		if (result != vk::Result::eSuccess)
			Exception::throwFailedToCreate("Failed to wait queue");
	}

	device.freeCommandBuffers(commandPool, 1, &commandBuffer);
}

void FVK::Internal::Buffer::copyBuffer(const vk::Device device, const vk::CommandPool commandPool, const vk::Queue graphicsQueue, vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size) {
	vk::CommandBuffer commandBuffer = beginSingleTimeCommands(device, commandPool);

	vk::BufferCopy copyRegion = {
		.size = size
	};

	commandBuffer.copyBuffer(srcBuffer, dstBuffer, 1, &copyRegion);

	endSingleTimeCommands(commandBuffer, device, commandPool, graphicsQueue);
}
