#pragma once

#include "../Common/include.h"

namespace FVK::Internal {

	class Queue final {
	public:
		struct GetInfo {
			UI32 queueFamilyIndex;
			UI32 queueIndex;
		};

		struct Parameter {
			GetInfo* pInfo = nullptr;
			std::optional<Key::LogicalDeviceVariantKey> logicalDevice = std::nullopt;
		};

	public:
		//no-throw
		explicit Queue(ManagerPassKey, const Data::QueueData& data, const Parameter& parameter) noexcept;

		~Queue() = default;
		FluidumUtils_Class_Default_CopyMove(Queue)

	private:
		//no-throw
		void getQueue(const Data::QueueData& data, const Parameter& parameter) noexcept;

	public:
		//no-throw
		//There is nothing to destroy
		void destroy() const noexcept;

	public:
		//no-throw
		[[nodiscard]] const Data::QueueInfo& get() const noexcept;

	private:
		Data::QueueInfo info{};
		static_assert(std::is_nothrow_constructible_v<Data::QueueInfo>);

	};

}
