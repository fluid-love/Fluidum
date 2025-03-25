#pragma once

#include "../Common/include.h"

namespace FVK::Internal {

	class Messenger final {
	public:
		enum class ValidationLayer : UT {
			None,
			VK_LAYER_KHRONOS_validation //Anything else is deprecated. https://vulkan.lunarg.com/doc/view/1.1.114.0/windows/validation_layers.html
		};

		struct Message {
			VkDebugUtilsMessageSeverityFlagsEXT messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			VkDebugUtilsMessageTypeFlagsEXT messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		};

		struct Parameter {
			ValidationLayer validationLayer = ValidationLayer::VK_LAYER_KHRONOS_validation;
			PFN_vkDebugUtilsMessengerCallbackEXT callback = messengerCallback;
			Message message{};
		};

		static inline VKAPI_ATTR VkBool32 VKAPI_CALL messengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
			std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
			return VK_FALSE;
		}

	public:
		/*
		Exception:
			std::exception
			FailedToCreate
		*/
		//strong
		explicit Messenger(ManagerPassKey, const Data::MessengerData& data, const Parameter& parameter);
		
		~Messenger() = default;
		FluidumUtils_Class_Default_CopyMove(Messenger);

	private:
		/*
		Exception:
			std::exception
			FailedToCreate
		*/
		//strong
		void create(const Data::MessengerData& data, const Parameter& parameter);

	public:
		//no-throw
		[[nodiscard]] const Data::MessengerInfo& get() const noexcept;

	public:
		//no-throw
		void destroy() noexcept;

	private:
		/*
		Exception:
			std::exception
			FailedToCreate
		*/
		//strong
		[[nodiscard]] bool checkValidationLayerSupport(const Parameter& parameter) const;
		
		[[nodiscard]] VkResult createDebugUtilsMessengerEXT(const Data::MessengerData& data, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
		
		/*
		Exception:
			std::exception
		*/
		//strong
		[[nodiscard]] std::vector<const char*> makeValidationLayerNames(const Parameter& parameter)const noexcept;
				
		//no-throw
		void fillInfo(const Data::MessengerData& data) noexcept;
	private://data
		Data::MessengerInfo info = {};
	};

}
