#include "instance.h"

FVK::Internal::Instance::Instance(ManagerPassKey, const Data::InstanceData& data, const Parameter& parameter) {
	this->create(data, parameter);
}

FVK::Internal::Instance::Instance(ManagerPassKey, const Data::InstanceData& data, const MessengerParameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::Instance::create(const Data::InstanceData& data, const Parameter& parameter) {
	//no-throw
	const auto applicationVersion = VK_MAKE_VERSION(parameter.appInfo.appVersion.major, parameter.appInfo.appVersion.minor, parameter.appInfo.appVersion.patch);
	constexpr auto engineVersion = VK_MAKE_VERSION(EngineVersion.major, EngineVersion.minor, EngineVersion.patch);
	const auto apiVersion = convertVulkanApiVersion(parameter.appInfo.vulkanApiVersion);

	const vk::ApplicationInfo appInfo{
		.pApplicationName = parameter.appInfo.applicationName,
		.applicationVersion = applicationVersion,
		.pEngineName = EngineName,
		.engineVersion = engineVersion,
		.apiVersion = apiVersion
	};

	auto extensions = getRequiredExtensions();

	const vk::InstanceCreateInfo instanceCreateInfo = {
		.pNext = nullptr,
		.pApplicationInfo = &appInfo,
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = nullptr,
		.enabledExtensionCount = static_cast<UI32>(extensions.size()),
		.ppEnabledExtensionNames = extensions.data()
	};

	this->createInstance(instanceCreateInfo);
}

namespace FVK::Internal {

	bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers) {
		auto result = vk::enumerateInstanceLayerProperties();
		if (result.result != vk::Result::eSuccess)
			return false;

		std::vector<vk::LayerProperties> availableLayers = std::move(result.value);

		for (const char* layerName : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		return true;
	}

}

void FVK::Internal::Instance::create(const Data::InstanceData& data, const MessengerParameter& parameter) {
	//no-throw
	const auto applicationVersion = VK_MAKE_VERSION(parameter.appInfo.appVersion.major, parameter.appInfo.appVersion.minor, parameter.appInfo.appVersion.patch);
	constexpr auto engineVersion = VK_MAKE_VERSION(EngineVersion.major, EngineVersion.minor, EngineVersion.patch);
	const auto apiVersion = convertVulkanApiVersion(parameter.appInfo.vulkanApiVersion);

	const vk::ApplicationInfo appInfo = {
		.pApplicationName = parameter.appInfo.applicationName,
		.applicationVersion = applicationVersion,
		.pEngineName = EngineName,
		.engineVersion = engineVersion,
		.apiVersion = apiVersion
	};

	const auto extensions = getRequiredExtensions();

	const auto layerNames = makeValidationLayerNames(parameter.messengerParameter.validationLayer);

	const bool layerSupported = checkValidationLayerSupport(layerNames);

	const vk::InstanceCreateInfo instanceCreateInfo = {
		.pApplicationInfo = &appInfo,
		.enabledLayerCount = layerSupported ? static_cast<UI32>(layerNames.size()) : 0,
		.ppEnabledLayerNames = layerSupported ? layerNames.data() : nullptr,
		.enabledExtensionCount = static_cast<UI32>(extensions.size()),
		.ppEnabledExtensionNames = extensions.data()
	};

	this->createInstance(instanceCreateInfo);

	this->info.isMessengerCreated = true;
}

const FVK::Internal::Data::InstanceInfo& FVK::Internal::Instance::get() const noexcept {
	assert(this->info.instance);
	return this->info;
}

void FVK::Internal::Instance::destroy() const noexcept {
	assert(this->info.instance);
	info.instance.destroy();
}

unsigned int FVK::Internal::Instance::convertVulkanApiVersion(const VulkanApiVersion vulkanApiVersion) const noexcept {
	if (vulkanApiVersion == VulkanApiVersion::version_1_1)
		return VK_API_VERSION_1_1;
	else if (vulkanApiVersion == VulkanApiVersion::version_1_2)
		return  VK_API_VERSION_1_2;
	assert(vulkanApiVersion == VulkanApiVersion::version_1_0);
	return VK_API_VERSION_1_0;
}

std::vector<const char*> FVK::Internal::Instance::makeValidationLayerNames(const Messenger::ValidationLayer type) const {
	if (type == Messenger::ValidationLayer::VK_LAYER_KHRONOS_validation)
		return { "VK_LAYER_KHRONOS_validation" };

	return {};
}

std::vector<const char*> FVK::Internal::Instance::Instance::getRequiredExtensions() const {

	std::vector<const char*> extensions{
		VK_KHR_SURFACE_EXTENSION_NAME,

#ifdef FluidumUtils_Type_OS_Windows
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif

		VK_EXT_DEBUG_UTILS_EXTENSION_NAME
	};

	return extensions;
}

void FVK::Internal::Instance::Instance::createInstance(const vk::InstanceCreateInfo& info) {
	auto result = vk::createInstance(info);
	if (result.result != vk::Result::eSuccess) {
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to create instance({}).", vk::to_string(result.result));
		Exception::throwFailedToCreate();
	}
	static_assert(noexcept(this->info.instance = result.value));
	this->info.instance = result.value;
}



