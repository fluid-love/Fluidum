#include "physicaldevice.h"

bool FVK::Internal::PhysicalDevice::checkDeviceExtensionSupport(const vk::PhysicalDevice device, const std::vector<const char*>& deviceExtensions) const {
	using namespace FVK::Internal;

	auto result = device.enumerateDeviceExtensionProperties();
	if (result.result != vk::Result::eSuccess) {
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to enumerate DeviceExtensionProperties.", vk::to_string(result.result));
		Exception::throwFailedToCreate();
	}

	const std::vector<vk::ExtensionProperties> availableExtensions = std::move(result.value);

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

FVK::Internal::PhysicalDevice::QueueFamilyIndices FVK::Internal::PhysicalDevice::findQueueFamilies(const vk::QueueFlagBits flag, const vk::PhysicalDevice device, const vk::SurfaceKHR surface) const {
	using namespace FVK::Internal;

	PhysicalDevice::QueueFamilyIndices indices{};

	std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

	for (Size i = 0; const auto & queueFamily : queueFamilies) {
		if ((queueFamily.queueFlags & flag) == flag) {
			indices.graphicsFamily = static_cast<UI32>(i);
		}

		vk::Bool32 presentSupport = false;
		auto result = device.getSurfaceSupportKHR(static_cast<UI32>(i), surface, &presentSupport);
		if (result != vk::Result::eSuccess) {
			GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to get SurfaceSupportKHR({}).", vk::to_string(result));
			Exception::throwFailedToCreate();
		}

		if (presentSupport) {
			indices.presentFamily = static_cast<UI32>(i);
		}

		//no-throw
		if (indices.isComplete()) {//presentFamily and graphicsFamily
			break;
		}

		i++;
	}

	return indices;
}

FVK::Internal::PhysicalDevice::SwapChainSupportDetails FVK::Internal::PhysicalDevice::querySwapChainSupport(const vk::PhysicalDevice physicalDevice, const vk::SurfaceKHR surface) const {
	using namespace FVK::Internal;

	SwapChainSupportDetails details{};

	{
		auto result = physicalDevice.getSurfaceCapabilitiesKHR(surface);

		if (result.result != vk::Result::eSuccess) {
			GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to get SurfaceCapabilities({}).", vk::to_string(result.result));
			Exception::throwFailedToCreate();
		}

		details.capabilities = std::move(result.value);
	}
	{
		auto result = physicalDevice.getSurfaceFormatsKHR(surface);

		if (result.result != vk::Result::eSuccess) {
			GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to get SurfaceFormats({}).", vk::to_string(result.result));
			Exception::throwFailedToCreate();
		}

		details.formats = std::move(result.value);
	}
	{
		auto result = physicalDevice.getSurfacePresentModesKHR(surface);

		if (result.result != vk::Result::eSuccess) {
			GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to get SurfacePresentModes({}).", vk::to_string(result.result));
			Exception::throwFailedToCreate();
		}

		details.presentModes = std::move(result.value);
	}

	return details;
}

vk::SampleCountFlagBits FVK::Internal::PhysicalDevice::getMaxUsableSampleCount(const vk::PhysicalDevice physicalDevice) const noexcept {

	vk::PhysicalDeviceProperties physicalDeviceProperties = physicalDevice.getProperties();
	static_assert(std::is_nothrow_constructible_v<vk::PhysicalDeviceProperties, vk::PhysicalDeviceProperties&&>);

	vk::SampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;

	if (counts & vk::SampleCountFlagBits::e64) { return vk::SampleCountFlagBits::e64; }
	if (counts & vk::SampleCountFlagBits::e32) { return vk::SampleCountFlagBits::e32; }
	if (counts & vk::SampleCountFlagBits::e16) { return vk::SampleCountFlagBits::e16; }
	if (counts & vk::SampleCountFlagBits::e8) { return vk::SampleCountFlagBits::e8; }
	if (counts & vk::SampleCountFlagBits::e4) { return vk::SampleCountFlagBits::e4; }
	if (counts & vk::SampleCountFlagBits::e2) { return vk::SampleCountFlagBits::e2; }

	return vk::SampleCountFlagBits::e1;
}

void FVK::Internal::PhysicalDevice::fillInfo(const Data::PhysicalDeviceSwapchainData& data) noexcept {
	this->info.window = data.get<FvkType::Surface>().window;
	this->info.instance = data.get<FvkType::Surface>().instance;
	this->info.surface = data.get<FvkType::Surface>().surface;

	static_assert(noexcept(this->info.window = data.get<FvkType::Surface>().window));
	static_assert(noexcept(this->info.instance = data.get<FvkType::Surface>().instance));
	static_assert(noexcept(this->info.surface = data.get<FvkType::Surface>().surface));
}

[[nodiscard]] bool isFeaturesSupport(const std::vector<FVK::Internal::PhysicalDevice::Feature>& requireFeatures, const vk::PhysicalDeviceFeatures& supportedFeatures) {
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
	assert(false);
	//future
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

	//strong
	const auto devices = this->enumeratePhysicalDevices(data.get<FvkType::Surface>().instance);

	//strong
	std::vector<const char*> extensions = this->convertExtensions(parameter.extensions);
	this->info.extensionNames = extensions;

	//strong
	for (const auto& device : devices) {
		//Select the appropriate physical device found in the first place.
		if (isDeviceSuitable(parameter, device, data.get<FvkType::Surface>().surface, extensions)) {

			//find
			this->info.physicalDevice = device;
			break;
		}
	}

	if (!this->info.physicalDevice) {
		//not found
		{
			auto lock = GMessenger.getLock();
			GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to pick PhysicalDevice.");
			GMessenger.add<FU::Log::Type::Info>(__FILE__, __LINE__, "No available physical devices are found. It is not available on this computer.");
		}
		Exception::throwNotSupported();
	}
	else {
		//no-throw
		const auto msaaSamples = this->getMaxUsableSampleCount(this->info.physicalDevice);
		this->info.usableMaxMsaaSamples = msaaSamples;
	}

	//no-throw
	this->fillInfo(data);
}

const FVK::Internal::Data::PhysicalDeviceInfo& FVK::Internal::PhysicalDevice::get() const noexcept {
	assert(this->info.physicalDevice);
	return this->info;
}

void FVK::Internal::PhysicalDevice::destroy() const noexcept {
	//There is nothing to destroy, but the function is there for the sake of consistency.
}

vk::Format FVK::Internal::PhysicalDevice::findFormat(const std::vector<vk::Format>& candidates, const vk::ImageTiling tiling, const vk::FormatFeatureFlags features) const noexcept {
	for (vk::Format format : candidates) {

		//no-throw
		vk::FormatProperties props = this->info.physicalDevice.getFormatProperties(format);


		if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features) {
			return format;
		}
		else if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features) {
			return format;
		}

		static_assert(noexcept(props.linearTilingFeatures & features));
		static_assert(noexcept(props.linearTilingFeatures == features));

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

	//Whether or not there is a queue that satisfies the request.
	{
		const bool isQueueComplete = this->isQueueComplete(physicalDevice, surface, parameter.queue);
		if (!isQueueComplete)
			return false;
	}

	//Whether to support extensions that meet the requirements.
	{
		const bool isExtensionsSupported = this->checkDeviceExtensionSupport(physicalDevice, deviceExtensions);
		if (!isExtensionsSupported)
			return false;
	}

	//features{
	{
		//no-throw
		vk::PhysicalDeviceFeatures supportedFeatures = physicalDevice.getFeatures();

		const bool isFeaturesSup = isFeaturesSupport(parameter.features, supportedFeatures);
		if (!isFeaturesSup)
			return false;
	}

	//swapchain
	{
		SwapChainSupportDetails swapChainSupport = this->querySwapChainSupport(physicalDevice, surface);
		const bool isSwapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		if (!isSwapChainAdequate)
			return false;

		//no-throw
		this->info.capabilities = swapChainSupport.capabilities;
		static_assert(noexcept(this->info.capabilities = swapChainSupport.capabilities));

		//no-throw
		this->info.formats = std::move(swapChainSupport.formats);
		this->info.presentModes = std::move(swapChainSupport.presentModes);
	}

	return true;
}

std::vector<vk::PhysicalDevice> FVK::Internal::PhysicalDevice::enumeratePhysicalDevices(const vk::Instance instance) const {
	auto result = instance.enumeratePhysicalDevices();

	if (result.result != vk::Result::eSuccess) {
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to enumerate PhysicalDevices({}).", vk::to_string(result.result));
		Exception::throwFailedToCreate();
	}

	return result.value;
}

std::vector<const char*> FVK::Internal::PhysicalDevice::convertExtensions(const std::vector<Extension>& extensions) const {
	std::vector<const char*> result(extensions.size());
	for (Size i = 0, size = extensions.size(); i < size; i++) {
		result[i] = ExtensionNames[static_cast<UT>(extensions[i])];
		assert(static_cast<UT>(extensions[i]) < std::extent_v<decltype(ExtensionNames)>);
	}
	return result;
}

bool FVK::Internal::PhysicalDevice::isQueueComplete(const vk::PhysicalDevice device, const vk::SurfaceKHR surface, const std::optional<vk::QueueFlagBits> flag) {

	if (!flag.has_value()) {
		return true;
	}

	//Find devices that match the criteria.
	QueueFamilyIndices indices = findQueueFamilies(flag.value(), device, surface);

	//no-throw
	const bool complete = indices.isComplete();

	if (complete) { //good

		//no-throw 
		/*
		There is no possibility that the exception bad_optional_access will be thrown,
		as confirmed by the isComplete function.
		*/
		this->info.graphicsFamily = indices.graphicsFamily.value();
		this->info.presentFamily = indices.presentFamily.value();
		return true;
	}

	return false;
}

vk::Format FVK::Internal::PhysicalDevice::findSupportedFormat(const vk::PhysicalDevice physicalDevice, const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features) {
	for (vk::Format format : candidates) {
		vk::FormatProperties props{};
		static_assert(std::is_nothrow_constructible_v<vk::FormatProperties>);

		//no-throw
		physicalDevice.getFormatProperties(format, &props);

		//no-throw
		if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features) {
			return format;
		}
		else if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}

	Exception::throwNotSupported();
}

FVK::UI32 FVK::Internal::PhysicalDevice::findMemoryType(const vk::PhysicalDevice physicalDevice, const UI32 typeFilter, const vk::MemoryPropertyFlags properties) {

	//no-throw
	vk::PhysicalDeviceMemoryProperties memProperties = physicalDevice.getMemoryProperties();
	static_assert(std::is_nothrow_constructible_v<vk::PhysicalDeviceMemoryProperties, vk::PhysicalDeviceMemoryProperties&&>);

	//no-throw
	for (UI32 i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	Exception::throwNotSupported();
}
