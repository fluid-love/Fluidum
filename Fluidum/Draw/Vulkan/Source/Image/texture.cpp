#include "texture.h"
#include "../Buffer/buffer.h"
#include "../PhysicalDevice/physicaldevice.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../../External/stb/stb_image.h"

FVK::Internal::Texture::Texture(ManagerPassKey, const Data::TextureData& data, const Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::Texture::create(const Data::TextureData& data, const Parameter& parameter) {
	info.filePath = parameter.filePath;

	stbi_uc* pixels;
	I32 texWidth, texHeight, texChannels;
	vk::DeviceSize imageSize{};
	pixels = stbi_load(parameter.filePath, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	imageSize = texWidth * texHeight * 4;

	if (!pixels) {
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to open texture file({}).", parameter.filePath);
		Exception::throwFailedToCreate();
	}

	vk::Buffer stagingBuffer{};
	vk::DeviceMemory stagingBufferMemory{};

	try {
		//strong
		Buffer::createBuffer(
			data.get<FvkType::PhysicalDevice>().physicalDevice,
			data.get<FvkType::LogicalDevice>().device,
			imageSize,
			vk::BufferUsageFlagBits::eTransferSrc,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
			stagingBuffer,
			stagingBufferMemory
		);
	}
	catch (...) {
		stbi_image_free(pixels);
		std::rethrow_exception(std::current_exception());
	}

	try {
		void* vkdata;
		auto result = data.get<FvkType::LogicalDevice>().device.mapMemory(stagingBufferMemory, 0, imageSize);
		if (result.result != vk::Result::eSuccess) {
			GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to map Memory({}).", vk::to_string(result.result));
			Exception::throwFailedToCreate();
		}

		vkdata = result.value;
		memcpy(vkdata, pixels, static_cast<Size>(imageSize));
	}
	catch (...) {
		data.get<FvkType::LogicalDevice>().device.destroyBuffer(stagingBuffer);
		data.get<FvkType::LogicalDevice>().device.freeMemory(stagingBufferMemory);

		stbi_image_free(pixels);
		std::rethrow_exception(std::current_exception());
	}

	//no-throw
	data.get<FvkType::LogicalDevice>().device.unmapMemory(stagingBufferMemory);

	//free pixels
	stbi_image_free(pixels);

	try {
		//basic
		this->createImage(
			data.get<FvkType::PhysicalDevice>().physicalDevice,
			data.get<FvkType::LogicalDevice>().device,
			texWidth,
			texHeight,
			vk::Format::eR8G8B8A8Srgb,
			vk::ImageTiling::eOptimal,
			vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
			vk::MemoryPropertyFlagBits::eDeviceLocal
		);
	}
	catch (...) {
		data.get<FvkType::LogicalDevice>().device.destroyBuffer(stagingBuffer);
		data.get<FvkType::LogicalDevice>().device.freeMemory(stagingBufferMemory);

		std::rethrow_exception(std::current_exception());
	}

	try {
		//basic
		this->transitionImageLayout(
			data,
			vk::Format::eR8G8B8A8Srgb,
			vk::ImageLayout::eUndefined,
			vk::ImageLayout::eTransferDstOptimal
		);

		//strong
		this->copyBufferToImage(data, stagingBuffer, static_cast<UI32>(texWidth), static_cast<UI32>(texHeight));

		//basic
		this->transitionImageLayout(
			data,
			vk::Format::eR8G8B8A8Srgb,
			vk::ImageLayout::eTransferDstOptimal,
			vk::ImageLayout::eShaderReadOnlyOptimal
		);
	}
	catch (...) {
		data.get<FvkType::LogicalDevice>().device.destroyBuffer(stagingBuffer);
		data.get<FvkType::LogicalDevice>().device.freeMemory(stagingBufferMemory);

		std::rethrow_exception(std::current_exception());
	}


	//no-throw
	data.get<FvkType::LogicalDevice>().device.destroyBuffer(stagingBuffer);
	data.get<FvkType::LogicalDevice>().device.freeMemory(stagingBufferMemory);

	try {
		this->createImageView(data.get<FvkType::LogicalDevice>().device);
	}
	catch (...) {
		this->info.device.freeMemory(this->info.imageMemory);
		this->info.device.destroyImage(this->info.image);

		std::rethrow_exception(std::current_exception());
	}

	//no-throw
	this->info.device = data.get<FvkType::LogicalDevice>().device;
	static_assert(noexcept(info.device = data.get<FvkType::LogicalDevice>().device));
}

const FVK::Internal::Data::TextureInfo& FVK::Internal::Texture::get() const noexcept {
	assert(this->info.image && this->info.imageMemory && this->info.imageView);
	return this->info;
}

void FVK::Internal::Texture::destroy() noexcept {
	assert(this->info.image && this->info.imageMemory && this->info.imageView);
	this->info.device.destroyImageView(this->info.imageView);
	this->info.device.freeMemory(this->info.imageMemory);
	this->info.device.destroyImage(this->info.image);
}

void FVK::Internal::Texture::createImage(const vk::PhysicalDevice physicalDevice, const vk::Device device, uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties) {

	vk::Extent3D extent{
		.width = width,
		.height = height,
		.depth = 1,
	};

	vk::ImageCreateInfo imageInfo{
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

		if (result.result != vk::Result::eSuccess) {
			GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to create Image({}).", vk::to_string(result.result));
			Exception::throwFailedToCreate();
		}

		//no-throw
		this->info.image = result.value;
		static_assert(noexcept(info.image = result.value));
	}

	//no-throw
	vk::MemoryRequirements memRequirements = device.getImageMemoryRequirements(this->info.image);
	static_assert(std::is_nothrow_constructible_v<vk::MemoryRequirements, vk::MemoryRequirements&&>);

	//no-throw
	vk::PhysicalDeviceMemoryProperties memProperties = physicalDevice.getMemoryProperties();
	static_assert(std::is_nothrow_constructible_v<vk::PhysicalDeviceMemoryProperties, vk::PhysicalDeviceMemoryProperties&& >);

	vk::MemoryAllocateInfo allocInfo{
		.allocationSize = memRequirements.size,
	};

	try {
		//strong
		allocInfo.memoryTypeIndex = PhysicalDevice::findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);
	}
	catch (...) {
		//no-throw
		device.destroyImage(this->info.image);

		std::rethrow_exception(std::current_exception());
	}

	try {
		auto result = device.allocateMemory(allocInfo);

		if (result.result != vk::Result::eSuccess) {
			GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to allocate Memory({}).", vk::to_string(result.result));
			Exception::throwFailedToCreate();
		}

		//no-throw
		this->info.imageMemory = result.value;
		static_assert(noexcept(info.imageMemory = result.value));
	}
	catch (...) {
		//no-throw
		device.destroyImage(this->info.image);

		std::rethrow_exception(std::current_exception());
	}

	try {
		auto result = device.bindImageMemory(this->info.image, this->info.imageMemory, 0);
		if (result != vk::Result::eSuccess) {
			GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to bind ImageMemory({}).", vk::to_string(result));
			Exception::throwFailedToCreate();
		}
	}
	catch (...) {
		//no-throw
		device.freeMemory(this->info.imageMemory);
		device.destroyImage(this->info.image);

		std::rethrow_exception(std::current_exception());
	}

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
	if (result.result != vk::Result::eSuccess) {
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to create ImageView({}).", vk::to_string(result.result));
		Exception::throwFailedToCreate();
	}

	//no-throw
	this->info.imageView = result.value;
	static_assert(noexcept(info.imageView = result.value));

}

void FVK::Internal::Texture::transitionImageLayout(const Data::TextureData& data, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout) {
	const auto device = data.get<FvkType::LogicalDevice>().device;

	//strong
	vk::CommandBuffer commandBuffer = Buffer::beginSingleTimeCommands(
		device,
		data.get<FvkType::CommandPool>().commandPool
	);

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

	vk::PipelineStageFlags sourceStage{};
	vk::PipelineStageFlags destinationStage{};

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
		assert(false);
		try {
			//strong
			Buffer::endSingleTimeCommands(
				commandBuffer,
				device,
				data.get<FvkType::CommandPool>().commandPool,
				data.get<FvkType::Queue>().queue
			);
		}
		catch (...) {
			device.freeCommandBuffers(data.get<FvkType::CommandPool>().commandPool, 1, &commandBuffer);
			std::rethrow_exception(std::current_exception());
		}
		Exception::throwFailedToCreate();
	}

	//no-throw
	commandBuffer.pipelineBarrier(
		sourceStage,
		destinationStage,
		vk::DependencyFlags(),
		nullptr,
		nullptr,
		barrier
	);

	try {
		//strong
		Buffer::endSingleTimeCommands(
			commandBuffer,
			device,
			data.get<FvkType::CommandPool>().commandPool,
			data.get<FvkType::Queue>().queue
		);
	}
	catch (...) {
		device.freeCommandBuffers(data.get<FvkType::CommandPool>().commandPool, 1, &commandBuffer);
		std::rethrow_exception(std::current_exception());
	}

}

void FVK::Internal::Texture::copyBufferToImage(const Data::TextureData& data, const vk::Buffer buffer, const UI32 width, const UI32 height) {
	//strong
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

	//no-throw
	commandBuffer.copyBufferToImage(buffer, this->info.image, vk::ImageLayout::eTransferDstOptimal, 1, &region);

	try {
		//strong
		Buffer::endSingleTimeCommands(
			commandBuffer,
			data.get<FvkType::LogicalDevice>().device,
			data.get<FvkType::CommandPool>().commandPool, 
			data.get<FvkType::Queue>().queue
		);
	}
	catch (...) {
		data.get<FvkType::LogicalDevice>().device.freeCommandBuffers(data.get<FvkType::CommandPool>().commandPool, 1, &commandBuffer);
		std::rethrow_exception(std::current_exception());
	}
}


