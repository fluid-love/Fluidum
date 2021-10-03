#pragma once

#include "../Common/include.h"
#include "messenger.h"

namespace FVK::Internal {

	class Instance final {
	public:

		struct ApplicationVersion {
			const uint16_t major = 1;
			const uint16_t minor = 0;
			const uint16_t patch = 0;
		};

		//vulkan api version 
		//バージョンを超えた機能を利用しようとするとメッセージが飛ぶ
		enum class VulkanApiVersion : UT {
			version_1_0,
			version_1_1,
			version_1_2
		};

		struct ApplicationInfo {
			const char* applicationName = "App";
			ApplicationVersion appVersion{};//アプリのバージョン
			VulkanApiVersion vulkanApiVersion = VulkanApiVersion::version_1_2;//要求するvulkanのversion
		};

		struct Parameter {
			ApplicationInfo appInfo = {};
		};

		//Messenger
		struct MessengerParameter {
			Messenger::Parameter messengerParameter{};
			ApplicationInfo appInfo = {};
		};

	public:
		explicit Instance(ManagerPassKey, const Data::InstanceData& data, const Parameter& parameter);
		explicit Instance(ManagerPassKey, const Data::InstanceData& data, const MessengerParameter& parameter);

		~Instance() = default;
		FluidumUtils_Class_Default_CopyMove(Instance)

	private:
		void create(const Data::InstanceData& data, const Parameter& parameter);
		void create(const Data::InstanceData& data, const MessengerParameter& parameter);

	public:
		_NODISCARD const Data::InstanceInfo& get() const noexcept;
		void destroy();

	private:
		_NODISCARD unsigned int convertVulkanApiVersion(const VulkanApiVersion vulkanApiVersion) const noexcept;
		_NODISCARD std::vector<const char*> makeValidationLayerNames(const Messenger::ValidationLayer type) const noexcept;
		_NODISCARD std::vector<const char*> getRequiredExtensions() const;
		void createInstance(const vk::InstanceCreateInfo& info);

	private://data
		Data::InstanceInfo info = {};

	};



}