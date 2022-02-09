#pragma once

#include "../Common/include.h"

namespace FVK::Internal {

	class Semaphore final {
	public:
		struct CreateInfo : public vk::SemaphoreCreateInfo {
		private:
			using vk::SemaphoreCreateInfo::sType;
			using vk::SemaphoreCreateInfo::pNext;
			using vk::SemaphoreCreateInfo::flags;
		};

		struct Parameter {
			CreateInfo* pInfo = nullptr;
			std::optional<Key::LogicalDeviceVariantKey> logicalDeviceKey = std::nullopt;
		};

	public:
		/*
		Exception:
			FailedToCreate
		*/
		//strong
		explicit Semaphore(ManagerPassKey, const Data::SemaphoreData& data, const Parameter& parameter);

		~Semaphore() noexcept = default;
		FluidumUtils_Class_Default_CopyMove(Semaphore);

	private:
		/*
		Exception:
			FailedToCreate
		*/
		//strong
		void create(const Data::SemaphoreData& data, const Parameter& parameter);

	public:
		//no-throw
		void destroy() noexcept;

	public:
		//no-throw
		[[nodiscard]] const Data::SemaphoreInfo& get() const noexcept;

	private:
		Data::SemaphoreInfo info{};

	};

}
