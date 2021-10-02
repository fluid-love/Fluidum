#pragma once

#include "../Common/FVKinclude.h"

namespace FVK::Internal {

	class PhysicalDevice final {
	public:
		struct SwapChainSupportDetails final {
			vk::SurfaceCapabilitiesKHR capabilities;
			std::vector<vk::SurfaceFormatKHR> formats;
			std::vector<vk::PresentModeKHR> presentModes;
		};

		struct QueueFamilyIndices final {
			std::optional<uint32_t> graphicsFamily = std::nullopt;
			std::optional<uint32_t> presentFamily = std::nullopt;

			bool isComplete() {
				return graphicsFamily.has_value() && presentFamily.has_value();
			}
		};

	public:

		enum class Extension : UT {
			KHR_16bit_storage,
			KHR_swapchain
		};
		static constexpr inline const char* ExtensionNames[] = {
			VK_KHR_16BIT_STORAGE_EXTENSION_NAME,
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		enum class Feature : UT {
			SamplerAnisotropy
		};

		struct Parameter {
			std::optional<Key::InstanceVariantKey> instanceKey = std::nullopt;

		};

		//全てを満たすデバイスを検査する
		struct SwapchainParameter {
			std::optional<Key::SurfaceVariantKey> surfaceKey = std::nullopt;
			std::optional<vk::QueueFlagBits> queue = std::nullopt;
			std::vector<Extension> extensions = {};
			std::vector<Feature> features = {};
		};

	public:
		explicit PhysicalDevice(ManagerPassKey, const Data::PhysicalDeviceData& data, const Parameter& parameter);
		explicit PhysicalDevice(ManagerPassKey, const Data::PhysicalDeviceSwapchainData& data, const SwapchainParameter& parameter);

		~PhysicalDevice() = default;
		FluidumUtils_Class_Default_CopyMove(PhysicalDevice)

	private:
		void pick(const Data::PhysicalDeviceData& data, const Parameter& parameter);
		void pick(const Data::PhysicalDeviceSwapchainData& data, const SwapchainParameter& parameter);

	public:
		_NODISCARD const Data::PhysicalDeviceInfo& get() const noexcept;
		vk::Format findFormat(const std::vector<vk::Format>& candidates, const vk::ImageTiling tiling, const vk::FormatFeatureFlags features) const;
		void destroy() const noexcept;

	private:
		bool isDeviceSuitable(const Parameter& parameter, vk::PhysicalDevice physicalDevice, const vk::SurfaceKHR surface, const std::vector<const char*>& deviceExtensions);
		bool isDeviceSuitable(const SwapchainParameter& parameter, vk::PhysicalDevice physicalDevice, const vk::SurfaceKHR surface, const std::vector<const char*>& deviceExtensions);

		std::vector<vk::PhysicalDevice> enumeratePhysicalDevices(const vk::Instance instance) const;
		std::vector<const char*> convertExtensions(const std::vector<Extension>& extensions) const;

		bool isQueueComplete(const vk::PhysicalDevice device, const vk::SurfaceKHR surface, const std::optional<vk::QueueFlagBits> flag);

		bool checkDeviceExtensionSupport(const vk::PhysicalDevice device, const std::vector<const char*>& deviceExtensions);
		QueueFamilyIndices findQueueFamilies(const vk::QueueFlagBits flag, const vk::PhysicalDevice device, const vk::SurfaceKHR surface);
		SwapChainSupportDetails querySwapChainSupport(const vk::PhysicalDevice physicalDevice, const vk::SurfaceKHR surface);

		vk::SampleCountFlagBits getMaxUsableSampleCount(const vk::PhysicalDevice physicalDevice);

		void fillInfo(const Data::PhysicalDeviceSwapchainData& data) VULKAN_HPP_NOEXCEPT;
	public:
		static vk::Format findSupportedFormat(const vk::PhysicalDevice physicalDevice, const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features);
		_NODISCARD static uint32_t findMemoryType(const vk::PhysicalDevice physicalDevice, const uint32_t typeFilter, const vk::MemoryPropertyFlags properties);

	private:
		Data::PhysicalDeviceInfo info = {};

	};

}
