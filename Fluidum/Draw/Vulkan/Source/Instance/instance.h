#pragma once

#include "../Common/include.h"
#include "messenger.h"

namespace FVK::Internal {

	class Instance final {
	public:
		struct ApplicationVersion {
			const UIF16 major = 1;
			const UIF16 minor = 0;
			const UIF16 patch = 0;
		};

		//vulkan api version 
		enum class VulkanApiVersion : UT {
			version_1_0,
			version_1_1,
			version_1_2
		};

		struct ApplicationInfo {
			const char* applicationName = "App";
			ApplicationVersion appVersion{}; //version of application.
			VulkanApiVersion vulkanApiVersion = VulkanApiVersion::version_1_2;
		};

		struct Parameter {
			ApplicationInfo appInfo = {};
		};

	public:
		//Messenger
		struct MessengerParameter {
			Messenger::Parameter messengerParameter{};
			ApplicationInfo appInfo{};
		};

	public:
		/*
		Exception:
			std::exception
			FailedToCreate
		*/
		//strong
		explicit Instance(ManagerPassKey, const Data::InstanceData& data, const Parameter& parameter);
		explicit Instance(ManagerPassKey, const Data::InstanceData& data, const MessengerParameter& parameter);

		~Instance() noexcept = default;
		FluidumUtils_Class_Default_CopyMove(Instance)

	private:
		void create(const Data::InstanceData& data, const Parameter& parameter);
		void create(const Data::InstanceData& data, const MessengerParameter& parameter);

	public:
		//no-throw
		void destroy() const noexcept;
	
	public:
		//no-throw
		[[nodiscard]] const Data::InstanceInfo& get() const noexcept;

	private:
		//no-throw
		[[nodiscard]] unsigned int convertVulkanApiVersion(const VulkanApiVersion vulkanApiVersion) const noexcept;
		
		/*
		Exception:
			std::exception
		*/
		//strong
		[[nodiscard]] std::vector<const char*> makeValidationLayerNames(const Messenger::ValidationLayer type) const;
		[[nodiscard]] std::vector<const char*> getRequiredExtensions() const;
		
		/*
		Exception:
			std::exception
			FailedToCreate
		*/
		//strong
		void createInstance(const vk::InstanceCreateInfo& info);

	private://data
		Data::InstanceInfo info = {};

	};

}