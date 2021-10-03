#include "messenger.h"

FVK::Internal::Messenger::Messenger(ManagerPassKey, const Data::MessengerData& data, const Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::Messenger::create(const Data::MessengerData& data, const Parameter& parameter) {
	if (!checkValidationLayerSupport(parameter))
		Exception::throwFailedToCreate("Failed to create Messenger. ValidationLayer NotSupported");

	VkDebugUtilsMessengerCreateInfoEXT debugInfo = {
		.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
		.messageSeverity = parameter.message.messageSeverity,
		.messageType = parameter.message.messageType,
		.pfnUserCallback = parameter.callback
	};

	if (createDebugUtilsMessengerEXT(data,&debugInfo, nullptr, &info.messenger) != VK_SUCCESS)
		Exception::throwFailedToCreate("Failed to create Messenger.");

	this->fillInfo(data);
}

bool FVK::Internal::Messenger::checkValidationLayerSupport(const Parameter& parameter) const {
	auto result = vk::enumerateInstanceLayerProperties();
	std::vector<vk::LayerProperties> availableLayers = result.value;
	const auto names = this->makeValidationLayerNames(parameter);
	for (const char* layerName : names) {
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

VkResult FVK::Internal::Messenger::createDebugUtilsMessengerEXT(const Data::MessengerData& data, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(data.get<FvkType::Instance>().instance, "vkCreateDebugUtilsMessengerEXT");
	if (func) {
		return func(data.get<FvkType::Instance>().instance, pCreateInfo, pAllocator, pDebugMessenger);
	}

	return VK_ERROR_EXTENSION_NOT_PRESENT;
}

std::vector<const char*> FVK::Internal::Messenger::makeValidationLayerNames(const Parameter& parameter) const noexcept {
	if (parameter.validationLayer == ValidationLayer::VK_LAYER_KHRONOS_validation)
		return { "VK_LAYER_KHRONOS_validation" };

	return {};
}

void FVK::Internal::Messenger::fillInfo(const Data::MessengerData& data) VULKAN_HPP_NOEXCEPT {
	this->info.instance = data.get<FvkType::Instance>().instance;
}

void (*vkDestroyDebugUtilsMessenger)(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
void vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
	vkDestroyDebugUtilsMessenger(instance, debugMessenger, pAllocator);
}

const FVK::Internal::Data::MessengerInfo& FVK::Internal::Messenger::get() const noexcept {
	assert(this->info.messenger);
	return this->info;
}

void FVK::Internal::Messenger::destroy() {
	assert(this->info.messenger);

	vkDestroyDebugUtilsMessenger =
		(PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
			this->info.instance, "vkDestroyDebugUtilsMessengerEXT");
	vkDestroyDebugUtilsMessengerEXT(this->info.instance, info.messenger, nullptr);
}

