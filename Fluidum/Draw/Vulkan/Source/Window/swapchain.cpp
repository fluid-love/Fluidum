#include "swapchain.h"

FVK::Internal::Swapchain::Swapchain(const ManagerPassKey, const Data::SwapchainData& data, Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::Swapchain::create(const Data::SwapchainData& data, Parameter& parameter) {
	parameter.pInfo->surface = data.get<FvkType::LogicalDevice>().surface;

	auto result = data.get<FvkType::LogicalDevice>().device.createSwapchainKHR(*parameter.pInfo, nullptr);

	if (result.result != vk::Result::eSuccess) {
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to create Swapchain({}).", vk::to_string(result.result));
		Exception::throwFailedToCreate();
	}

	this->info.swapchain = result.value;

	//no-throw
	this->setInfo(parameter);

	//no-throw
	this->info.device = data.get<FvkType::LogicalDevice>().device;
	static_assert(noexcept(info.device = data.get<FvkType::LogicalDevice>().device));

}

const FVK::Internal::Data::SwapchainInfo& FVK::Internal::Swapchain::get() const noexcept {
	assert(info.swapchain);
	return this->info;
}

void FVK::Internal::Swapchain::destroy() noexcept {
	assert(info.swapchain);
	this->info.device.destroySwapchainKHR(info.swapchain);
}

void FVK::Internal::Swapchain::setInfo(const Parameter& parameter) noexcept {
	this->info.format = parameter.pInfo->imageFormat;
	this->info.extent = parameter.pInfo->imageExtent;
	this->info.minImageCount = parameter.pInfo->minImageCount;

	static_assert(noexcept(info.format = parameter.pInfo->imageFormat));
	static_assert(noexcept(info.extent = parameter.pInfo->imageExtent));
	static_assert(noexcept(info.minImageCount = parameter.pInfo->minImageCount));
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

vk::Extent2D FVK::Internal::Swapchain::getCorrectSwapchainExtent(WindowHandle window, const vk::SurfaceCapabilitiesKHR& capabilities) {
	if (capabilities.currentExtent.width != std::numeric_limits<UI32>::max()) {
		return capabilities.currentExtent;
	}

	int width, height;

#ifdef FluidumUtils_Type_OS_Windows
	RECT rect{};
	BOOL result = SystemParametersInfo(SPI_SETWORKAREA, NULL, &rect, NULL);
	if (result == FALSE) {
		Exception::throwUnexpected();
	}
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

#endif
	vk::Extent2D actualExtent = {
		static_cast<UI32>(width),
		static_cast<UI32>(height)
	};

	actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
	actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

	return actualExtent;
}
