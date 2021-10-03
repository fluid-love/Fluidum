#pragma once

#include "../Common/include.h"

namespace FVK::Internal {

	class CommandBuffer final {
	public:
		struct AllocateInfo : public vk::CommandBufferAllocateInfo {
		public:
			std::optional<Key::CommandPoolVariantKey> commandPoolKey = std::nullopt;
			using vk::CommandBufferAllocateInfo::level;
			using vk::CommandBufferAllocateInfo::commandBufferCount;
		private:
			using vk::CommandBufferAllocateInfo::sType;
			using vk::CommandBufferAllocateInfo::pNext;
			using vk::CommandBufferAllocateInfo::commandPool;
		private:
			friend CommandBuffer;
		};

		struct Parameter {
			AllocateInfo* pInfo = nullptr;
			std::optional<Key::LogicalDeviceVariantKey> logicalDeviceKey = std::nullopt;
		};

	public:
		explicit CommandBuffer(ManagerPassKey, const Data::CommandBufferData& data, Parameter& parameter);
		~CommandBuffer() = default;
		FluidumUtils_Class_Default_CopyMove(CommandBuffer)

	private:
		void allocate(const Data::CommandBufferData& data, Parameter& parameter);
	public:
		const Data::CommandBufferInfo& get() const noexcept;
		void destroy();

	private:
		Data::CommandBufferInfo info = {};
	};
}
