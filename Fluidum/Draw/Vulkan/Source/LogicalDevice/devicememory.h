#pragma once

#include "../Common/FVKinclude.h"

namespace FVK::Internal {

	class DeviceMemory final {
	public:
		using AllocateInfo = vk::MemoryAllocateInfo;

		struct Parameter {
			AllocateInfo* pInfo = nullptr;
			std::optional<Key::LogicalDeviceVariantKey> logicalDeviceKey = std::nullopt;
		};

	public:
		DeviceMemory(ManagerPassKey, const Data::DeviceMemoryData& data, const Parameter& parameter);
		~DeviceMemory() = default;
		FluidumUtils_Class_Default_CopyMove(DeviceMemory)

	public:
		void create(const Data::DeviceMemoryData& data, const Parameter& parameter);
		const Data::DeviceMemoryInfo& get() const noexcept;
		void destroy();

	private:
		Data::DeviceMemoryInfo info = {};
	};




}
