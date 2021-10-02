#pragma once

#include "../Common/FVKinclude.h"

namespace FVK::Internal::Buffer {
	void createBuffer(const vk::PhysicalDevice physicalDevice, const vk::Device device, const vk::DeviceSize size, const vk::BufferUsageFlags usage, const vk::MemoryPropertyFlags properties, vk::Buffer& buffer, vk::DeviceMemory& bufferMemory);

	VkCommandBuffer beginSingleTimeCommands(const vk::Device device, const vk::CommandPool commandPool);

	void endSingleTimeCommands(const vk::CommandBuffer commandBuffer, const vk::Device device, const vk::CommandPool commandPool, const vk::Queue graphicsQueue);

	void copyBuffer(const vk::Device device, const vk::CommandPool commandPool, const vk::Queue graphicsQueue, vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size);

}