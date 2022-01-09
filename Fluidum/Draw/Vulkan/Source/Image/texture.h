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

		~Texture() noexcept = default;
		FluidumUtils_Class_Default_CopyMove(Texture);

	private:
		void create(const Data::TextureData& data, const Parameter& parameter);

	public:
		//no-throw
		void destroy() noexcept;

	public:
		//no-throw
		[[nodiscard]] const Data::TextureInfo& get() const noexcept;

	private:
		/*
		Exception:
			FailedToCreate
		*/
		//basic
		/*
		When an exception is thrown,
		the image and imageview are invalidated,
		and the bind is released.
		*/
		//Create an image and allocate memory in imagememory. Then, bind the two together.
		void createImage(const vk::PhysicalDevice physicalDevice, const vk::Device device, uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties);

		/*
		Exception:
			FailedToCreate
		*/
		//strong
		void createImageView(const vk::Device device);

		/*
		Exception:
			FailedToCreate
		*/
		//basic
		void transitionImageLayout(const Data::TextureData& data, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout);
		
		/*
		Exception:
			FailedToCreate
		*/
		//strong
		void copyBufferToImage(const Data::TextureData& data, const vk::Buffer buffer, const UI32 width, const UI32 height);

	private://data
		Data::TextureInfo info{};

	};

}
