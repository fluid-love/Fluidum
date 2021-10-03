#include "swapchain.h"

FVK::Internal::Swapchain::Swapchain(const ManagerPassKey, const Data::SwapchainData& data, Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::Swapchain::create(const Data::SwapchainData& data, Parameter& parameter) {

	parameter.pInfo->surface = data.get<FvkType::LogicalDevice>().surface;

	auto result = data.get<FvkType::LogicalDevice>().device.createSwapchainKHR(*parameter.pInfo, nullptr);

	if (result.result != vk::Result::eSuccess)
		Exception::throwFailedToCreate("Failed to create Swapchain");
	this->info.swapchain = result.value;
	
	this->setInfo(parameter);

	this->info.device = data.get<FvkType::LogicalDevice>().device;
}

const FVK::Internal::Data::SwapchainInfo& FVK::Internal::Swapchain::get() const noexcept {
	assert(info.swapchain);
	return this->info;
}

void FVK::Internal::Swapchain::destroy() {
	assert(info.swapchain);
	this->info.device.destroySwapchainKHR(info.swapchain);
}

void FVK::Internal::Swapchain::setInfo(const Parameter& parameter) noexcept {
	info.format = parameter.pInfo->imageFormat;
	info.extent = parameter.pInfo->imageExtent;
	info.minImageCount = parameter.pInfo->minImageCount;
}

bool FVK::Internal::Swapchain::isSurfaceFormatSupport(const vk::Format format, const vk::ColorSpaceKHR colorSpace, const std::vector<vk::SurfaceFormatKHR>& availableFormats) {
	for (const auto& x : availableFormats) {
		if (x.format == format && x.colorSpace == colorSpace) {
			return true;
		}
	}

	return false;
}

bool FVK::Internal::Swapchain::isPresentModeSupport(const vk::PresentModeKHR presentMode, const std::vector<vk::PresentModeKHR>& availablePresentModes) {
	for (const auto& x : availablePresentModes) {
		if (x == presentMode) {
			return true;
		}
	}

	return false;
}

vk::Extent2D FVK::Internal::Swapchain::getCorrectSwapchainExtent(GLFWwindow* window, const vk::SurfaceCapabilitiesKHR& capabilities) {
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	vk::Extent2D actualExtent = {
		static_cast<uint32_t>(width),
		static_cast<uint32_t>(height)
	};

	actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
	actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

	return actualExtent;
}
