#include "instance.h"

FVK::Internal::Instance::Instance(ManagerPassKey, const Data::InstanceData& data, const Parameter& parameter) {
	this->create(data, parameter);
}

FVK::Internal::Instance::Instance(ManagerPassKey, const Data::InstanceData& data, const MessengerParameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::Instance::create(const Data::InstanceData& data,const Parameter& parameter) {
	auto applicationVersion = VK_MAKE_VERSION(parameter.appInfo.appVersion.major, parameter.appInfo.appVersion.minor, parameter.appInfo.appVersion.patch);
	constexpr auto engineVersion = VK_MAKE_VERSION(EngineVersion.major, EngineVersion.minor, EngineVersion.patch);

	//api‚Ìversion
	auto apiVersion = convertVulkanApiVersion(parameter.appInfo.vulkanApiVersion);

	vk::ApplicationInfo appInfo = {
		.pApplicationName = parameter.appInfo.applicationName,
		.applicationVersion = applicationVersion,
		.pEngineName = EngineName,
		.engineVersion = engineVersion,
		.apiVersion = apiVersion
	};

	auto extensions = getRequiredExtensions();

	vk::InstanceCreateInfo instanceCreateInfo = {
		.pNext = nullptr,
		.pApplicationInfo = &appInfo,
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = nullptr,
		.enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
		.ppEnabledExtensionNames = extensions.data()
	};

	this->createInstance(instanceCreateInfo);
}

void FVK::Internal::Instance::create(const Data::InstanceData& data, const MessengerParameter& parameter) {
	auto applicationVersion = VK_MAKE_VERSION(parameter.appInfo.appVersion.major, parameter.appInfo.appVersion.minor, parameter.appInfo.appVersion.patch);
	constexpr auto engineVersion = VK_MAKE_VERSION(EngineVersion.major, EngineVersion.minor, EngineVersion.patch);

	//api‚Ìversion
	auto apiVersion = convertVulkanApiVersion(parameter.appInfo.vulkanApiVersion);

	vk::ApplicationInfo appInfo = {
		.pApplicationName = parameter.appInfo.applicationName,
		.applicationVersion = applicationVersion,
		.pEngineName = EngineName,
		.engineVersion = engineVersion,
		.apiVersion = apiVersion
	};

	auto extensions = getRequiredExtensions();
	const auto layerNames = makeValidationLayerNames(parameter.messengerParameter.validationLayer);
	vk::InstanceCreateInfo instanceCreateInfo = {
		.pApplicationInfo = &appInfo,
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = layerNames.data(),
		.enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
		.ppEnabledExtensionNames = extensions.data()
	};

	if (parameter.messengerParameter.validationLayer == Messenger::ValidationLayer::VK_LAYER_KHRONOS_validation)
		instanceCreateInfo.enabledLayerCount = 1;

	this->createInstance(instanceCreateInfo);

	this->info.isMessengerCreated = true;
}

const FVK::Internal::Data::InstanceInfo& FVK::Internal::Instance::get() const noexcept {
	assert(this->info.instance);
	return this->info;
}

void FVK::Internal::Instance::destroy() {
	assert(this->info.instance);
	info.instance.destroy();
}

unsigned int FVK::Internal::Instance::convertVulkanApiVersion(const VulkanApiVersion vulkanApiVersion) const noexcept {
	if (vulkanApiVersion == VulkanApiVersion::version_1_1)
		return VK_API_VERSION_1_1;
	else if (vulkanApiVersion == VulkanApiVersion::version_1_2)
		return  VK_API_VERSION_1_2;
	return VK_API_VERSION_1_0;
}

std::vector<const char*> FVK::Internal::Instance::makeValidationLayerNames(const Messenger::ValidationLayer type) const noexcept {
	if (type == Messenger::ValidationLayer::VK_LAYER_KHRONOS_validation)
		return { "VK_LAYER_KHRONOS_validation" };

	return {};
}

std::vector<const char*> FVK::Internal::Instance::Instance::getRequiredExtensions() const {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
	extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	info.instance.operator VkInstance();
	return extensions;
}

void FVK::Internal::Instance::Instance::createInstance(const vk::InstanceCreateInfo& info) {
	auto result = vk::createInstance(info);
	if (result.result != vk::Result::eSuccess)
		Exception::throwFailedToCreate("Failed to create Instance");

	this->info.instance = result.value;
}



