#pragma once

#include "../Common/FVKinclude.h"

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
		explicit CommandPool(ManagerPassKey, const Data::CommandPoolData& data, const Parameter& parameter);
		~CommandPool() = default;
		FluidumUtils_Class_Default_CopyMove(CommandPool)

	private:
		void create(const Data::CommandPoolData& data, const Parameter& parameter);
	public:
		const Data::CommandPoolInfo& get() const noexcept;
		void destroy();

	private:
		Data::CommandPoolInfo info = {};
	};
}
