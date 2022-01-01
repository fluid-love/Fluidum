#pragma once

#include "../Common/include.h"

namespace FVK::Internal {

	class LogicalDevice final {
	public:
		struct Parameter {
			std::optional<Key::PhysicalDeviceVariantKey> physicalDeviceKey = std::nullopt;
		};

	public:
		/*
		Exception:
			std::exception
			FailedToCreate
		*/
		//strong
		explicit LogicalDevice(ManagerPassKey, const Data::LogicalDeviceData& data, const Parameter& parameter);

		~LogicalDevice() = default;
		FluidumUtils_Class_Default_CopyMove(LogicalDevice)

	private:
		/*
		Exception:
			std::exception
			FailedToCreate
		*/
		//strong
		void create(const Data::LogicalDeviceData& data, const Parameter& parameter);

	public:
		//no-throw
		void destroy() noexcept;

	public:
		//no-throw
		const Data::LogicalDeviceInfo& get() const noexcept;

	private:
		//no-throw
		void fillData(const Data::LogicalDeviceData& data) noexcept;

	private:
		Data::LogicalDeviceInfo info{};

	};

}
