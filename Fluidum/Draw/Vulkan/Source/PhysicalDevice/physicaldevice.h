#pragma once

#include "../Common/include.h"

namespace FVK::Internal {

	class PhysicalDevice final {
	public:
		struct SwapChainSupportDetails final {
			vk::SurfaceCapabilitiesKHR capabilities{};
			std::vector<vk::SurfaceFormatKHR> formats{};
			std::vector<vk::PresentModeKHR> presentModes{};
		};

		struct QueueFamilyIndices final {
			std::optional<UI32> graphicsFamily = std::nullopt;
			std::optional<UI32> presentFamily = std::nullopt;

			[[nodiscard]] bool isComplete() const noexcept {
				return graphicsFamily.has_value() && presentFamily.has_value();
			}
		};

	public:
		enum class Extension : UT {
			KHR_16bit_storage,
			KHR_swapchain
		};

		static constexpr const char* ExtensionNames[2] = {
			VK_KHR_16BIT_STORAGE_EXTENSION_NAME,
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		enum class Feature : UT {
			SamplerAnisotropy
		};

		struct Parameter {
			std::optional<Key::InstanceVariantKey> instanceKey = std::nullopt;
		};

		//Search for devices that meet all of the following.
		struct SwapchainParameter {
			std::optional<Key::SurfaceVariantKey> surfaceKey = std::nullopt;
			std::optional<vk::QueueFlagBits> queue = std::nullopt;
			std::vector<Extension> extensions{};
			std::vector<Feature> features{};
		};

	public:
		/*
		Exception:
			std::exception
			FailedToCreate
			NotSupported   -> There is no physical device that meets the request.
		*/
		//basic
		explicit PhysicalDevice(ManagerPassKey, const Data::PhysicalDeviceData& data, const Parameter& parameter);
		explicit PhysicalDevice(ManagerPassKey, const Data::PhysicalDeviceSwapchainData& data, const SwapchainParameter& parameter);

		~PhysicalDevice() = default;
		FluidumUtils_Class_Default_CopyMove(PhysicalDevice);

	private:
		/*
		Exception:
			std::exception
			FailedToCreate
			NotSupported   -> There is no physical device that meets the request.
		*/
		//basic
		void pick(const Data::PhysicalDeviceData& data, const Parameter& parameter);
		void pick(const Data::PhysicalDeviceSwapchainData& data, const SwapchainParameter& parameter);

	public:
		//no-throw
		[[nodiscard]] const Data::PhysicalDeviceInfo& get() const noexcept;

	public:
		//no-throw
		//There is nothing to destroy
		void destroy() const noexcept;

	public:
		//no-throw
		[[nodiscard]] vk::Format findFormat(const std::vector<vk::Format>& candidates, const vk::ImageTiling tiling, const vk::FormatFeatureFlags features) const noexcept;

	private:
		/*
		Exception:
			std::exception
			FailedToCreate
		*/
		//strong
		[[nodiscard]] bool isDeviceSuitable(const Parameter& parameter, vk::PhysicalDevice physicalDevice, const vk::SurfaceKHR surface, const std::vector<const char*>& deviceExtensions);
		[[nodiscard]] bool isDeviceSuitable(const SwapchainParameter& parameter, vk::PhysicalDevice physicalDevice, const vk::SurfaceKHR surface, const std::vector<const char*>& deviceExtensions);
		[[nodiscard]] std::vector<vk::PhysicalDevice> enumeratePhysicalDevices(const vk::Instance instance) const;

		/*
		Exception:
			std::exception
		*/
		//strong
		[[nodiscard]] std::vector<const char*> convertExtensions(const std::vector<Extension>& extensions) const;

		/*
		Exception:
			std::exception
			FailedToCreate
		*/
		//strong
		[[nodiscard]] bool isQueueComplete(const vk::PhysicalDevice device, const vk::SurfaceKHR surface, const std::optional<vk::QueueFlagBits> flag);
		[[nodiscard]] bool checkDeviceExtensionSupport(const vk::PhysicalDevice device, const std::vector<const char*>& deviceExtensions) const;
		[[nodiscard]] QueueFamilyIndices findQueueFamilies(const vk::QueueFlagBits flag, const vk::PhysicalDevice device, const vk::SurfaceKHR surface) const;
		[[nodiscard]] SwapChainSupportDetails querySwapChainSupport(const vk::PhysicalDevice physicalDevice, const vk::SurfaceKHR surface) const;

		//no-throw
		[[nodiscard]] vk::SampleCountFlagBits getMaxUsableSampleCount(const vk::PhysicalDevice physicalDevice) const noexcept;

		//no-throw
		void fillInfo(const Data::PhysicalDeviceSwapchainData& data) noexcept;

	public:
		/*
		Exception:
			NotSupported
		*/
		//strong
		[[nodiscard]]static vk::Format findSupportedFormat(const vk::PhysicalDevice physicalDevice, const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features);
		
		/*
		Exception:
			NotSupported
		*/
		//strong
		[[nodiscard]] static UI32 findMemoryType(const vk::PhysicalDevice physicalDevice, const UI32 typeFilter, const vk::MemoryPropertyFlags properties);

	private:
		Data::PhysicalDeviceInfo info = {};

	};

}
