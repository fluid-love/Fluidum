#include "logicaldevice.h"
#include "../PhysicalDevice/physicaldevice.h"

FVK::Internal::LogicalDevice::LogicalDevice(ManagerPassKey, const Data::LogicalDeviceData& data, const Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::LogicalDevice::create(const Data::LogicalDeviceData& data, const Parameter& parameter) {

	const auto& pData = data.get<FvkType::PhysicalDevice_Swapchain>();

	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { pData.graphicsFamily, pData.presentFamily };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		vk::DeviceQueueCreateInfo queueCreateInfo = {
			.queueFamilyIndex = queueFamily,
			.queueCount = 1,
			.pQueuePriorities = &queuePriority
		};
		queueCreateInfos.push_back(queueCreateInfo);
	}

	vk::PhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	vk::DeviceCreateInfo createInfo = {
		.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
		.pQueueCreateInfos = queueCreateInfos.data(),
		.enabledExtensionCount = static_cast<uint32_t>(pData.extensionNames.size()),
		.ppEnabledExtensionNames = pData.extensionNames.data(),
		.pEnabledFeatures = &deviceFeatures,
	};

	{
		auto result = pData.physicalDevice.createDevice(createInfo);
		if (result.result != vk::Result::eSuccess)
			Exception::throwFailedToCreate("Failed to create LogicalDevice");
		this->info.device = result.value;
	}

	this->fillData(data);
}

const FVK::Internal::Data::LogicalDeviceInfo& FVK::Internal::LogicalDevice::get() const noexcept {
	assert(this->info.device);
	return this->info;
}

void FVK::Internal::LogicalDevice::destroy() {
	assert(this->info.device);
	info.device.destroy();
}

void FVK::Internal::LogicalDevice::fillData(const Data::LogicalDeviceData& data) VULKAN_HPP_NOEXCEPT  {
	this->info.window = data.get<FvkType::PhysicalDevice>().window;
	this->info.instance = data.get<FvkType::PhysicalDevice>().instance;
	this->info.surface = data.get<FvkType::PhysicalDevice>().surface;
	this->info.physicalDevice = data.get<FvkType::PhysicalDevice>().physicalDevice;
}
