#pragma once

#include "../Common/include.h"

namespace FVK::Internal {

	class Swapchain final {
	public:
		struct CreateInfo : public vk::SwapchainCreateInfoKHR {
		public:
			using vk::SwapchainCreateInfoKHR::flags;
			using vk::SwapchainCreateInfoKHR::minImageCount;
			using vk::SwapchainCreateInfoKHR::imageFormat;
			using vk::SwapchainCreateInfoKHR::imageColorSpace;
			using vk::SwapchainCreateInfoKHR::imageExtent;
			using vk::SwapchainCreateInfoKHR::imageArrayLayers;
			using vk::SwapchainCreateInfoKHR::imageUsage;
			using vk::SwapchainCreateInfoKHR::imageSharingMode;
			using vk::SwapchainCreateInfoKHR::queueFamilyIndexCount;
			using vk::SwapchainCreateInfoKHR::pQueueFamilyIndices;
			using vk::SwapchainCreateInfoKHR::preTransform;
			using vk::SwapchainCreateInfoKHR::compositeAlpha;
			using vk::SwapchainCreateInfoKHR::presentMode;
			using vk::SwapchainCreateInfoKHR::clipped;
			using vk::SwapchainCreateInfoKHR::oldSwapchain;
		private:
			using vk::SwapchainCreateInfoKHR::sType;
			using vk::SwapchainCreateInfoKHR::pNext;
			using vk::SwapchainCreateInfoKHR::surface;
		private:
			friend class Swapchain;
		};

		struct Parameter {
			CreateInfo* pInfo = nullptr;
			std::optional<Key::LogicalDeviceVariantKey> logicalDeviceKey = std::nullopt;
		};

	public:
		/*
		Exception:
			FailedToCreate
		*/
		//strong
		explicit Swapchain(ManagerPassKey, const Data::SwapchainData& data, Parameter& parameter);
		
		~Swapchain() = default;
		FluidumUtils_Class_Default_CopyMove(Swapchain);

	private:
		/*
		Exception:
			FailedToCreate
		*/
		//strong
		void create(const Data::SwapchainData& data, Parameter& parameter);

	public:
		//no-throw
		void destroy() noexcept;

	public:
		//no-throw
		[[nodiscard]] const Data::SwapchainInfo& get() const noexcept;

	private:
		//no-throw
		void setInfo(const Parameter& parameter) noexcept;

	public:
		[[nodiscard]] static bool isSurfaceFormatSupport(const vk::Format format, const vk::ColorSpaceKHR colorSpace, const std::vector<vk::SurfaceFormatKHR>& availableFormats);
		[[nodiscard]] static bool isPresentModeSupport(const vk::PresentModeKHR presentMode, const std::vector<vk::PresentModeKHR>& availablePresentModes);
		[[nodiscard]] static vk::Extent2D getCorrectSwapchainExtent(GLFWwindow* window, const vk::SurfaceCapabilitiesKHR& capabilities);

	private:
		Data::SwapchainInfo info{};

	};

}