#include "FVKmessenger.h"

FVK::Internal::Messanger::Messanger(ManagerPassKey, const Data::MessengerData& data, const Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::Messanger::create(const Data::MessengerData& data, const Parameter& parameter) {
	if (!checkValidationLayerSupport(parameter))
		Exception::throwFailedToCreate("Failed to create Messanger. ValidationLayer NotSupported");

	VkDebugUtilsMessengerCreateInfoEXT debugInfo = {
		.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
		.messageSeverity = parameter.message.messageSeverity,
		.messageType = parameter.message.messageType,
		.pfnUserCallback = parameter.callback
	};

	if (createDebugUtilsMessengerEXT(data,&debugInfo, nullptr, &info.messanger) != VK_SUCCESS)
		Exception::throwFailedToCreate("Failed to create Messanger.");

	this->fillInfo(data);
}

bool FVK::Internal::Messanger::checkValidationLayerSupport(const Parameter& parameter) const {
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

VkResult FVK::Internal::Messanger::createDebugUtilsMessengerEXT(const Data::MessengerData& data, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(data.get<FvkType::Instance>().instance, "vkCreateDebugUtilsMessengerEXT");
	if (func) {
		return func(data.get<FvkType::Instance>().instance, pCreateInfo, pAllocator, pDebugMessenger);
	}

	return VK_ERROR_EXTENSION_NOT_PRESENT;
}

std::vector<const char*> FVK::Internal::Messanger::makeValidationLayerNames(const Parameter& parameter) const noexcept {
	if (parameter.validationLayer == ValidationLayer::VK_LAYER_KHRONOS_validation)
		return { "VK_LAYER_KHRONOS_validation" };

	return {};
}

void FVK::Internal::Messanger::fillInfo(const Data::MessengerData& data) VULKAN_HPP_NOEXCEPT {
	this->info.instance = data.get<FvkType::Instance>().instance;
}

void (*vkDestroyDebugUtilsMessenger)(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
void vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
	vkDestroyDebugUtilsMessenger(instance, debugMessenger, pAllocator);
}

const FVK::Internal::Data::MessangerInfo& FVK::Internal::Messanger::get() const noexcept {
	assert(this->info.messanger);
	return this->info;
}

void FVK::Internal::Messanger::destroy() {
	assert(this->info.messanger);

	vkDestroyDebugUtilsMessenger =
		(PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
			this->info.instance, "vkDestroyDebugUtilsMessengerEXT");
	vkDestroyDebugUtilsMessengerEXT(this->info.instance, info.messanger, nullptr);
}

