#pragma once

#include "../Common/FVKinclude.h"

namespace FVK::Internal {

	class Queue final {
	public:
		struct GetInfo {
			uint32_t queueFamilyIndex;
			uint32_t queueIndex;
		};
		struct Parameter {
			GetInfo* pInfo = nullptr;
			std::optional<Key::LogicalDeviceVariantKey> logicalDevice = std::nullopt;
		};

	public:
		explicit Queue(ManagerPassKey, const Data::QueueData& data, const Parameter& parameter);
		~Queue() = default;
		FluidumUtils_Class_Default_CopyMove(Queue)

	private:
		void getQueue(const Data::QueueData& data, const Parameter& parameter);
	public:
		const Data::QueueInfo& get() const noexcept;
		void destroy() const noexcept;

	private:
		Data::QueueInfo info = {};
	};

}
