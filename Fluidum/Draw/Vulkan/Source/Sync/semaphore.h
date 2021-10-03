#pragma once

#include "../Common/include.h"

namespace FVK::Internal {

	class Semaphore final {
	public:
		struct CreateInfo : public vk::SemaphoreCreateInfo {
		public:
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
		explicit Semaphore(ManagerPassKey,const Data::SemaphoreData& data, const Parameter& parameter);
		~Semaphore() = default;
		FluidumUtils_Class_Default_CopyMove(Semaphore)

	private:
		void create(const Data::SemaphoreData& data, const Parameter& parameter);
	public:
		_NODISCARD const Data::SemaphoreInfo& get() const noexcept;
		void destroy();

	private:
		Data::SemaphoreInfo info = {};

	};




}
