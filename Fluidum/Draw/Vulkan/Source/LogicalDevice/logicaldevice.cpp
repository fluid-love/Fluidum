#include "logicaldevice.h"
#include "../PhysicalDevice/physicaldevice.h"

FVK::Internal::LogicalDevice::LogicalDevice(ManagerPassKey, const Data::LogicalDeviceData& data, const Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::LogicalDevice::create(const Data::LogicalDeviceData& data, const Parameter& parameter) {

	const auto& pData = data.get<FvkType::PhysicalDevice_Swapchain>();

	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos{};
	std::set<UI32> uniqueQueueFamilies = { pData.graphicsFamily, pData.presentFamily };

	float queuePriority = 1.0f;
	for (UI32 queueFamily : uniqueQueueFamilies) {
		vk::DeviceQueueCreateInfo queueCreateInfo = {
			.queueFamilyIndex = queueFamily,
			.queueCount = 1,
			.pQueuePriorities = &queuePriority
		};
		queueCreateInfos.push_back(queueCreateInfo);
	}

	vk::PhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	const vk::DeviceCreateInfo createInfo = {
		.queueCreateInfoCount = static_cast<UI32>(queueCreateInfos.size()),
		.pQueueCreateInfos = queueCreateInfos.data(),
		.enabledExtensionCount = static_cast<UI32>(pData.extensionNames.size()),
		.ppEnabledExtensionNames = pData.extensionNames.data(),
		.pEnabledFeatures = &deviceFeatures,
	};

	{
		auto result = pData.physicalDevice.createDevice(createInfo);
		if (result.result != vk::Result::eSuccess) {
			GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to create LogicalDevice({}).", vk::to_string(result.result));
			Exception::throwFailedToCreate();
		}
		this->info.device = result.value;
	}

	//no-throw
	this->fillData(data);
}

const FVK::Internal::Data::LogicalDeviceInfo& FVK::Internal::LogicalDevice::get() const noexcept {
	assert(this->info.device);
	return this->info;
}

void FVK::Internal::LogicalDevice::destroy() noexcept {
	assert(this->info.device);
	this->info.device.destroy();
}

void FVK::Internal::LogicalDevice::fillData(const Data::LogicalDeviceData& data) noexcept  {
	this->info.window = data.get<FvkType::PhysicalDevice>().window;
	this->info.instance = data.get<FvkType::PhysicalDevice>().instance;
	this->info.surface = data.get<FvkType::PhysicalDevice>().surface;
	this->info.physicalDevice = data.get<FvkType::PhysicalDevice>().physicalDevice;

	static_assert(noexcept(info.window = data.get<FvkType::PhysicalDevice>().window));
	static_assert(noexcept(info.instance = data.get<FvkType::PhysicalDevice>().instance));
	static_assert(noexcept(info.surface = data.get<FvkType::PhysicalDevice>().surface));
	static_assert(noexcept(info.physicalDevice = data.get<FvkType::PhysicalDevice>().physicalDevice));
}
