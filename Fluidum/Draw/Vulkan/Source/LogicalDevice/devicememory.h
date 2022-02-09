#pragma once

#include "../Common/include.h"

namespace FVK::Internal {

	class DeviceMemory final {
	public:
		using AllocateInfo = vk::MemoryAllocateInfo;

		struct Parameter {
			AllocateInfo* pInfo = nullptr;
			std::optional<Key::LogicalDeviceVariantKey> logicalDeviceKey = std::nullopt;
		};

	public:
		/*
		Exception:
			FailedToCreate
		*/
		//strong
		DeviceMemory(ManagerPassKey, const Data::DeviceMemoryData& data, const Parameter& parameter);

		~DeviceMemory() noexcept = default;
		FluidumUtils_Class_Default_CopyMove(DeviceMemory);

	public:
		/*
		Exception:
			FailedToCreate
		*/
		//strong
		void create(const Data::DeviceMemoryData& data, const Parameter& parameter);

	public:
		//no-thrwo
		void destroy() noexcept;

	public:
		//no-thrwo
		[[nodiscard]] const Data::DeviceMemoryInfo& get() const noexcept;

	private:
		Data::DeviceMemoryInfo info{};

	};

}
