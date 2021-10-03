#include "physicaldevice.h"

bool FVK::Internal::PhysicalDevice::checkDeviceExtensionSupport(const vk::PhysicalDevice device, const std::vector<const char*>& deviceExtensions) {
	using namespace FVK::Internal;

	auto result = device.enumerateDeviceExtensionProperties();
	if (result.result != vk::Result::eSuccess)
		Exception::throwFailedToCreate("Extension Support");

	std::vector<vk::ExtensionProperties> availableExtensions = result.value;

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

FVK::Internal::PhysicalDevice::QueueFamilyIndices FVK::Internal::PhysicalDevice::findQueueFamilies(const vk::QueueFlagBits flag, const vk::PhysicalDevice device, const vk::SurfaceKHR surface) {
	using namespace FVK::Internal;

	PhysicalDevice::QueueFamilyIndices indices;

	std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

	for (int32_t i = 0; const auto & queueFamily : queueFamilies) {
		if ((queueFamily.queueFlags & flag) == flag) {
			indices.graphicsFamily = i;
		}

		vk::Bool32 presentSupport = false;
		auto result = device.getSurfaceSupportKHR(i, surface, &presentSupport);
		if (result != vk::Result::eSuccess)
			throw std::runtime_error("");

		if (presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.isComplete()) {
			break;
		}

		i++;
	}

	return indices;
}

FVK::Internal::PhysicalDevice::SwapChainSupportDetails FVK::Internal::PhysicalDevice::querySwapChainSupport(const vk::PhysicalDevice physicalDevice, const vk::SurfaceKHR surface) {
	using namespace FVK::Internal;

	SwapChainSupportDetails details;

	{
		auto result = physicalDevice.getSurfaceCapabilitiesKHR(surface);

		if (result.result != vk::Result::eSuccess)
			Exception::throwFailedToCreate("getSurfaceCapabilitiesKHR");

		details.capabilities = result.value;
	}
	{
		auto result = physicalDevice.getSurfaceFormatsKHR(surface);

		if (result.result != vk::Result::eSuccess)
			Exception::throwFailedToCreate("getSurfaceFormatsKHR");

		details.formats = result.value;
	}
	{
		auto result = physicalDevice.getSurfacePresentModesKHR(surface);

		if (result.result != vk::Result::eSuccess)
			Exception::throwFailedToCreate("getSurfacePresentModesKHR");

		details.presentModes = result.value;
	}

	return details;
}

vk::SampleCountFlagBits FVK::Internal::PhysicalDevice::getMaxUsableSampleCount(const vk::PhysicalDevice physicalDevice) {

	vk::PhysicalDeviceProperties physicalDeviceProperties = physicalDevice.getProperties();

	vk::SampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;

	if (counts & vk::SampleCountFlagBits::e64) { return vk::SampleCountFlagBits::e64; }
	if (counts & vk::SampleCountFlagBits::e32) { return vk::SampleCountFlagBits::e32; }
	if (counts & vk::SampleCountFlagBits::e16) { return vk::SampleCountFlagBits::e16; }
	if (counts & vk::SampleCountFlagBits::e8) { return vk::SampleCountFlagBits::e8; }
	if (counts & vk::SampleCountFlagBits::e4) { return vk::SampleCountFlagBits::e4; }
	if (counts & vk::SampleCountFlagBits::e2) { return vk::SampleCountFlagBits::e2; }

	return vk::SampleCountFlagBits::e1;
}

void FVK::Internal::PhysicalDevice::fillInfo(const Data::PhysicalDeviceSwapchainData& data) VULKAN_HPP_NOEXCEPT {
	this->info.window = data.get<FvkType::Surface>().window;
	this->info.instance = data.get<FvkType::Surface>().instance;
	this->info.surface = data.get<FvkType::Surface>().surface;
}

bool isFeaturesSupport(const std::vector<FVK::Internal::PhysicalDevice::Feature>& requireFeatures, const vk::PhysicalDeviceFeatures& supportedFeatures) {
	using enum FVK::Internal::PhysicalDevice::Feature;
	vk::Bool32 result = false;
	for (const auto& x : requireFeatures) {

		if (x == SamplerAnisotropy)
			result = supportedFeatures.samplerAnisotropy;

		if (result == VK_FALSE)
			return false;
	}

	return true;
}

FVK::Internal::PhysicalDevice::PhysicalDevice(ManagerPassKey, const Data::PhysicalDeviceData& data, const Parameter& parameter) {
	this->pick(data, parameter);
}

FVK::Internal::PhysicalDevice::PhysicalDevice(ManagerPassKey, const Data::PhysicalDeviceSwapchainData& data, const SwapchainParameter& parameter) {
	this->pick(data, parameter);
}

void FVK::Internal::PhysicalDevice::pick(const Data::PhysicalDeviceData& data, const Parameter& parameter) {

	//auto result = data.get<FvkType::Instance>().enumeratePhysicalDevices();

	//if (result.result != vk::Result::eSuccess)
	//	Exception::throwFailedToCreate("Find GPUs");

	//std::vector<vk::PhysicalDevice> devices = result.value;;

	//std::vector<const char*> extensions(parameter.extensions.size());
	//for (std::size_t i = 0, size = parameter.extensions.size(); i < size; i++) {
	//	extensions[i] = ExtensionNames[static_cast<UT>(parameter.extensions[i])];
	//	assert(static_cast<UT>(parameter.extensions[i]) < std::extent_v<decltype(ExtensionNames)>);
	//}

	//for (const auto& device : devices) {
	//	if (isDeviceSuitable(parameter, device, data.get<FvkType::Surface>(), extensions)) {
	//		physicalDevice = device;
	//		//this->savedParameter->msaaSamples = getMaxUsableSampleCount(physicalDevice);
	//		break;
	//	}
	//}

	//if (!physicalDevice)
	//	Exception::throwFailedToCreate(Exception::PhysicalDevice);
}

void FVK::Internal::PhysicalDevice::pick(const Data::PhysicalDeviceSwapchainData& data, const SwapchainParameter& parameter) {

	const auto devices = enumeratePhysicalDevices(data.get<FvkType::Surface>().instance);

	std::vector<const char*> extensions = convertExtensions(parameter.extensions);
	this->info.extensionNames = extensions;

	for (const auto& device : devices) {
		if (isDeviceSuitable(parameter, device, data.get<FvkType::Surface>().surface, extensions)) {
			this->info.physicalDevice = device;
			break;
		}
	}

	if (!this->info.physicalDevice)
		Exception::throwFailedToCreate("Failed to pick PhysicalDevice");

	this->fillInfo(data);
}

const FVK::Internal::Data::PhysicalDeviceInfo& FVK::Internal::PhysicalDevice::get() const noexcept {
	assert(this->info.physicalDevice);
	return this->info;
}

void FVK::Internal::PhysicalDevice::destroy() const noexcept {

}

vk::Format FVK::Internal::PhysicalDevice::findFormat(const std::vector<vk::Format>& candidates, const vk::ImageTiling tiling, const vk::FormatFeatureFlags features) const {
	for (vk::Format format : candidates) {

		vk::FormatProperties props = this->info.physicalDevice.getFormatProperties(format);

		if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features) {
			return format;
		}
		else if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}

	return vk::Format::eUndefined;
}

bool FVK::Internal::PhysicalDevice::isDeviceSuitable(const Parameter& parameter, vk::PhysicalDevice physicalDevice, const vk::SurfaceKHR surface, const std::vector<const char*>& deviceExtensions) {
	//using namespace FVK::Internal;

	//QueueFamilyIndices indices;
	//bool isQueueComplete = false;

	////queueの指定をしない
	//if (!parameter.queue.flag.has_value()) {
	//	isQueueComplete = true;
	//}
	//else {//条件に合致するdeviceを探す
	//	indices = findQueueFamilies(parameter.queue.flag.value(), physicalDevice, surface);
	//	isQueueComplete = indices.isComplete();
	//}

	//bool isExtensionsSupported = checkDeviceExtensionSupport(physicalDevice, deviceExtensions);

	//bool isSwapChainAdequate = false;
	//if (isExtensionsSupported && parameter.swapChainSupport) {
	//	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface);
	//	isSwapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	//}
	//else {
	//	isSwapChainAdequate = true;
	//}

	//vk::PhysicalDeviceFeatures supportedFeatures = physicalDevice.getFeatures();
	//bool isFeaturesSup = isFeaturesSupport(parameter.features, supportedFeatures);

	//if (isQueueComplete && isExtensionsSupported && isSwapChainAdequate && isFeaturesSup) {
	//	return true;
	//}
	return false;
}

bool FVK::Internal::PhysicalDevice::isDeviceSuitable(const SwapchainParameter& parameter, vk::PhysicalDevice physicalDevice, const vk::SurfaceKHR surface, const std::vector<const char*>& deviceExtensions) {
	using namespace FVK::Internal;

	//要求を満たすqueueがあるか
	bool isQueueComplete = this->isQueueComplete(physicalDevice, surface, parameter.queue);
	if (!isQueueComplete)
		return false;

	//要求を満たす拡張をサポートするか
	bool isExtensionsSupported = checkDeviceExtensionSupport(physicalDevice, deviceExtensions);
	if (!isExtensionsSupported)
		return false;

	//swapchain
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface);
	bool isSwapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	if (!isSwapChainAdequate)
		return false;
	this->info.capabilities = swapChainSupport.capabilities;
	this->info.formats = std::move(swapChainSupport.formats);
	this->info.presentModes = std::move(swapChainSupport.presentModes);


	//Features
	vk::PhysicalDeviceFeatures supportedFeatures = physicalDevice.getFeatures();
	bool isFeaturesSup = isFeaturesSupport(parameter.features, supportedFeatures);
	if (!isFeaturesSup)
		return false;

	//msaasamples
	auto msaaSamples = getMaxUsableSampleCount(physicalDevice);
	this->info.usableMaxMsaaSamples = msaaSamples;

	return true;
}

std::vector<vk::PhysicalDevice> FVK::Internal::PhysicalDevice::enumeratePhysicalDevices(const vk::Instance instance) const {
	auto result = instance.enumeratePhysicalDevices();

	if (result.result != vk::Result::eSuccess)
		Exception::throwFailedToCreate("Find GPUs");

	return result.value;
}

std::vector<const char*> FVK::Internal::PhysicalDevice::convertExtensions(const std::vector<Extension>& extensions) const {
	std::vector<const char*> result(extensions.size());
	for (std::size_t i = 0, size = extensions.size(); i < size; i++) {
		result[i] = ExtensionNames[static_cast<UT>(extensions[i])];
		assert(static_cast<UT>(extensions[i]) < std::extent_v<decltype(ExtensionNames)>);
	}
	return result;
}

bool FVK::Internal::PhysicalDevice::isQueueComplete(const vk::PhysicalDevice device, const vk::SurfaceKHR surface, const std::optional<vk::QueueFlagBits> flag) {
	QueueFamilyIndices indices;
	//queueの要求がなければtrue
	if (!flag.has_value()) {
		return true;
	}
	//条件に合致するdeviceを探す
	indices = findQueueFamilies(flag.value(), device, surface);
	if (indices.isComplete()) {
		this->info.graphicsFamily = indices.graphicsFamily.value();
		this->info.presentFamily = indices.presentFamily.value();
		return true;
	}
	return false;
}

vk::Format FVK::Internal::PhysicalDevice::findSupportedFormat(const vk::PhysicalDevice physicalDevice, const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features) {
	for (vk::Format format : candidates) {
		vk::FormatProperties props;

		physicalDevice.getFormatProperties(format, &props);

		if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features) {
			return format;
		}
		else if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}

	Exception::throwNotSupported("Failed to find supported format");
}

uint32_t FVK::Internal::PhysicalDevice::findMemoryType(const vk::PhysicalDevice physicalDevice, const uint32_t typeFilter, const vk::MemoryPropertyFlags properties) {

	vk::PhysicalDeviceMemoryProperties memProperties = physicalDevice.getMemoryProperties();

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	Exception::throwNotSupported("failed to find memory type");
}
