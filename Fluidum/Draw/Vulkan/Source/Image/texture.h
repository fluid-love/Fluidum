#pragma once

#include "../Common/include.h"

namespace FVK::Internal {

	class Texture final {
	public:

		struct Parameter {
			const char* filePath = nullptr;
			std::optional<Key::PhysicalDeviceVariantKey> physicalDeviceKey = std::nullopt;
			std::optional<Key::LogicalDeviceVariantKey> logicalDeviceKey = std::nullopt;
			std::optional<Key::QueueVariantKey> graphicsQueueKey = std::nullopt;
			std::optional<Key::CommandPoolVariantKey> commandPoolKey = std::nullopt;
		};

	public:
		explicit Texture(ManagerPassKey, const Data::TextureData& data, const Parameter& parameter);
		~Texture() = default;
		FluidumUtils_Class_Default_CopyMove(Texture)

	private:
		void create(const Data::TextureData& data, const Parameter& parameter);
	public:
		_NODISCARD const Data::TextureInfo& get() const noexcept;

		void destroy();

	private:
		void  createImage(const vk::PhysicalDevice physicalDevice, const vk::Device device, uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties);
		void  createImageView(const vk::Device device);
		void  transitionImageLayout(const Data::TextureData& data, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout);
		void  copyBufferToImage(const Data::TextureData& data, vk::Buffer buffer, uint32_t width, uint32_t height);


	private://data
		Data::TextureInfo info = {};
	};



}
