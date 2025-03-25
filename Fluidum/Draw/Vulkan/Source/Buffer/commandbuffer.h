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
		/*
		Exception:
			FailedToDestroy
		*/
		//strong
		explicit CommandBuffer(ManagerPassKey, const Data::CommandBufferData& data, Parameter& parameter);

		~CommandBuffer() noexcept = default;
		FluidumUtils_Class_Default_CopyMove(CommandBuffer);

	private:
		/*
		Exception:
			FailedToDestroy
		*/
		//strong
		void allocate(const Data::CommandBufferData& data, Parameter& parameter);

	public:
		//no-throw
		void destroy() noexcept;
		
	public:
		//no-throw
		[[nodiscard]] const Data::CommandBufferInfo& get() const noexcept;
		
	private:
		Data::CommandBufferInfo info = {};

	};

}
