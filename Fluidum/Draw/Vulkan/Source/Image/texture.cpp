#include "texture.h"
#include "../Buffer/buffer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../../Libraries/stb/stb_image.h"

FVK::Internal::Texture::Texture(ManagerPassKey, const Data::TextureData& data, const Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::Texture::create(const Data::TextureData& data, const Parameter& parameter) {
	info.filePath = parameter.filePath;

	stbi_uc* pixels = nullptr;
	int32_t texWidth, texHeight, texChannels;
	vk::DeviceSize imageSize;
	pixels = stbi_load(parameter.filePath, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	imageSize = texWidth * texHeight * 4;

	if (!pixels) {
		throw std::runtime_error("Failed to load TextureFile");
	}

	vk::Buffer stagingBuffer;
	vk::DeviceMemory stagingBufferMemory;

	Buffer::createBuffer(data.get<FvkType::PhysicalDevice>().physicalDevice, data.get<FvkType::LogicalDevice>().device, imageSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, stagingBuffer, stagingBufferMemory);

	void* vkdata;
	auto result = data.get<FvkType::LogicalDevice>().device.mapMemory(stagingBufferMemory, 0, imageSize);
	if (result.result != vk::Result::eSuccess)
		throw std::runtime_error("Failed to map memory");

	vkdata = result.value;

	memcpy(vkdata, pixels, static_cast<std::size_t>(imageSize));

	data.get<FvkType::LogicalDevice>().device.unmapMemory(stagingBufferMemory);


	stbi_image_free(pixels);


	createImage(data.get<FvkType::PhysicalDevice>().physicalDevice, data.get<FvkType::LogicalDevice>().device, texWidth, texHeight, vk::Format::eR8G8B8A8Srgb, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::MemoryPropertyFlagBits::eDeviceLocal);

	transitionImageLayout(data, vk::Format::eR8G8B8A8Srgb, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);
	copyBufferToImage(data, stagingBuffer, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
	transitionImageLayout(data, vk::Format::eR8G8B8A8Srgb, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);

	data.get<FvkType::LogicalDevice>().device.destroyBuffer(stagingBuffer);
	data.get<FvkType::LogicalDevice>().device.freeMemory(stagingBufferMemory);

	createImageView(data.get<FvkType::LogicalDevice>().device);

	this->info.device = data.get<FvkType::LogicalDevice>().device;
}

const FVK::Internal::Data::TextureInfo& FVK::Internal::Texture::get() const noexcept {
	assert(this->info.image && this->info.imageMemory && this->info.imageView);
	return this->info;
}

void FVK::Internal::Texture::destroy() {
	assert(this->info.image && this->info.imageMemory && this->info.imageView);
	this->info.device.destroyImageView(this->info.imageView);
	this->info.device.freeMemory(this->info.imageMemory);
	this->info.device.destroyImage(this->info.image);
}

void  FVK::Internal::Texture::createImage(const vk::PhysicalDevice physicalDevice, const vk::Device device, uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties) {

	vk::Extent3D extent = {
		.width = width,
		.height = height,
		.depth = 1,
	};

	vk::ImageCreateInfo imageInfo = {
		.imageType = vk::ImageType::e2D,
		.format = format,
		.extent = extent,
		.mipLevels = 1,
		.arrayLayers = 1,
		.samples = vk::SampleCountFlagBits::e1,
		.tiling = tiling,
		.usage = usage,
		.sharingMode = vk::SharingMode::eExclusive,
		.initialLayout = vk::ImageLayout::eUndefined,
	};

	{
		auto result = device.createImage(imageInfo);

		if (result.result != vk::Result::eSuccess)
			Exception::throwFailedToCreate("Failed to create TextureImage");

		this->info.image = result.value;
	}


	vk::MemoryRequirements memRequirements = device.getImageMemoryRequirements(this->info.image);

	//find memorytype
	vk::PhysicalDeviceMemoryProperties memProperties = physicalDevice.getMemoryProperties();
	uint32_t i = 0;
	bool isMemoryTypeFind = false;
	for (; i < memProperties.memoryTypeCount; i++) {
		if ((memRequirements.memoryTypeBits & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			isMemoryTypeFind = true;
			break;
		}
	}

	if (!isMemoryTypeFind)
		throw std::runtime_error("");

	vk::MemoryAllocateInfo allocInfo = {
		.allocationSize = memRequirements.size,
		.memoryTypeIndex = i
	};


	{
		auto result = device.allocateMemory(allocInfo);

		if (result.result != vk::Result::eSuccess)
			Exception::throwFailedToCreate("Failed to allocate TextureImageMemory");

		this->info.imageMemory = result.value;
	}

	auto result = device.bindImageMemory(this->info.image, this->info.imageMemory, 0);
	if (result != vk::Result::eSuccess)
		Exception::throwFailedToCreate("Failed to bind TextureImageMemory");
}

void FVK::Internal::Texture::createImageView(const vk::Device device) {

	vk::ImageSubresourceRange range = {
		.aspectMask = vk::ImageAspectFlagBits::eColor,
		.baseMipLevel = 0,
		.levelCount = 1,
		.baseArrayLayer = 0,
		.layerCount = 1,
	};

	vk::ImageViewCreateInfo viewInfo{
		.image = this->info.image,
		.viewType = vk::ImageViewType::e2D,
		.format = vk::Format::eR8G8B8A8Srgb,
		.subresourceRange = range,
	};

	auto result = device.createImageView(viewInfo);

	if (result.result != vk::Result::eSuccess)
		Exception::throwFailedToCreate("Failed to create TextureImageView");

	this->info.imageView = result.value;
}

void FVK::Internal::Texture::transitionImageLayout(const Data::TextureData& data, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout) {
	vk::CommandBuffer commandBuffer = Buffer::beginSingleTimeCommands(data.get<FvkType::LogicalDevice>().device, data.get<FvkType::CommandPool>().commandPool);


	vk::ImageSubresourceRange range = {
		.aspectMask = vk::ImageAspectFlagBits::eColor,
		.baseMipLevel = 0,
		.levelCount = 1,
		.baseArrayLayer = 0,
		.layerCount = 1,
	};
	vk::ImageMemoryBarrier barrier = {
		.oldLayout = oldLayout,
		.newLayout = newLayout,
		.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.image = this->info.image,
		.subresourceRange = range
	};

	vk::PipelineStageFlags sourceStage;
	vk::PipelineStageFlags destinationStage;

	if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal) {
		barrier.srcAccessMask = vk::AccessFlagBits();
		barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

		sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
		destinationStage = vk::PipelineStageFlagBits::eTransfer;
	}
	else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
		barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
		barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

		sourceStage = vk::PipelineStageFlagBits::eTransfer;
		destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
	}
	else if (oldLayout == vk::ImageLayout::ePresentSrcKHR && newLayout == vk::ImageLayout::eTransferSrcOptimal) {
		barrier.srcAccessMask = vk::AccessFlagBits::eMemoryRead;
		barrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;

		sourceStage = vk::PipelineStageFlagBits::eTransfer;
		destinationStage = vk::PipelineStageFlagBits::eTransfer;
	}
	else {
		throw std::invalid_argument("");
	}


	commandBuffer.pipelineBarrier(
		sourceStage,
		destinationStage,
		vk::DependencyFlags(),
		nullptr,
		nullptr,
		barrier
	);

	Buffer::endSingleTimeCommands(commandBuffer, data.get<FvkType::LogicalDevice>().device, data.get<FvkType::CommandPool>().commandPool, data.get<FvkType::Queue>().queue);
}

void FVK::Internal::Texture::copyBufferToImage(const Data::TextureData& data, vk::Buffer buffer, uint32_t width, uint32_t height) {
	vk::CommandBuffer commandBuffer = Buffer::beginSingleTimeCommands(data.get<FvkType::LogicalDevice>().device, data.get<FvkType::CommandPool>().commandPool);

	vk::ImageSubresourceLayers layers = {
		.aspectMask = vk::ImageAspectFlagBits::eColor,
		.mipLevel = 0,
		.baseArrayLayer = 0,
		.layerCount = 1,
	};

	vk::BufferImageCopy region = {
		.bufferOffset = 0,
		.bufferRowLength = 0,
		.bufferImageHeight = 0,
		.imageSubresource = layers,
		.imageOffset = vk::Offset3D(0,0,0),
		.imageExtent = vk::Extent3D(width, height, 1),
	};

	commandBuffer.copyBufferToImage(buffer, this->info.image, vk::ImageLayout::eTransferDstOptimal, 1, &region);

	Buffer::endSingleTimeCommands(commandBuffer, data.get<FvkType::LogicalDevice>().device, data.get<FvkType::CommandPool>().commandPool, data.get<FvkType::Queue>().queue);
}


