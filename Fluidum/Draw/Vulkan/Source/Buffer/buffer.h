#pragma once

#include "../Common/include.h"

namespace FVK::Internal::Buffer {

	//vkBuffer and vkBufferMemory will be enabled. and bind memory.
	/*
	Exception:
		FailedToCreate
	*/
	//strong
	void createBuffer(const vk::PhysicalDevice physicalDevice, const vk::Device device, const vk::DeviceSize size, const vk::BufferUsageFlags usage, const vk::MemoryPropertyFlags properties, vk::Buffer& buffer, vk::DeviceMemory& bufferMemory);

	/*
	Exception:
		std::exception
		FailedToCreate
	*/
	//strong
	VkCommandBuffer beginSingleTimeCommands(const vk::Device device, const vk::CommandPool commandPool);

	/*
	Exception:
		FailedToCreate
	*/
	//strong
	//Note that the commandbuffer is not released when an exception is thrown.
	void endSingleTimeCommands(const vk::CommandBuffer commandBuffer, const vk::Device device, const vk::CommandPool commandPool, const vk::Queue graphicsQueue);
	
	/*
	Exception:
		std::exception
		FailedToCreate
	*/
	//strong
	void copyBuffer(const vk::Device device, const vk::CommandPool commandPool, const vk::Queue graphicsQueue, vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size);

}