#pragma once

#include "../Common/include.h"

namespace FVK::Internal {

	class CommandPool final {
	public:
		struct CreateInfo : public vk::CommandPoolCreateInfo {
		public:
			using vk::CommandPoolCreateInfo::flags;
			using vk::CommandPoolCreateInfo::queueFamilyIndex;
		private:
			using vk::CommandPoolCreateInfo::sType;
			using vk::CommandPoolCreateInfo::pNext;
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
		explicit CommandPool(ManagerPassKey, const Data::CommandPoolData& data, const Parameter& parameter);

		~CommandPool() noexcept = default;
		FluidumUtils_Class_Default_CopyMove(CommandPool);

	private:
		/*
		Exception:
			FailedToCreate
		*/
		//strong
		void create(const Data::CommandPoolData& data, const Parameter& parameter);

	public:
		//no-throw
		void destroy() noexcept;

	public:
		//no-throw
		[[nodiscard]] const Data::CommandPoolInfo& get() const noexcept;

	private:
		Data::CommandPoolInfo info{};

	};

}
