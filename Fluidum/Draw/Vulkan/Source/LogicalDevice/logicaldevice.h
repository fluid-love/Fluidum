#pragma once

#include "../Common/FVKinclude.h"

namespace FVK::Internal {

	class LogicalDevice final {
	public:
		struct Parameter {
			std::optional<Key::PhysicalDeviceVariantKey> physicalDeviceKey = std::nullopt;
		};

	public:
		explicit LogicalDevice(ManagerPassKey, const Data::LogicalDeviceData& data, const Parameter& parameter);
		~LogicalDevice() = default;
		FluidumUtils_Class_Default_CopyMove(LogicalDevice)

	private:
		void create(const Data::LogicalDeviceData& data, const Parameter& parameter);
	public:
		const Data::LogicalDeviceInfo& get() const noexcept;
		void destroy();

	private:
		void fillData(const Data::LogicalDeviceData& data) VULKAN_HPP_NOEXCEPT;

	private:
		Data::LogicalDeviceInfo info = {};
	};




}
