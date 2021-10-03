#pragma once

#include "../Common/include.h"

namespace FVK::Internal {

	class DescriptorPool final {
	public:

		struct CreateInfo : public vk::DescriptorPoolCreateInfo {
		public:
			using vk::DescriptorPoolCreateInfo::flags;
			using vk::DescriptorPoolCreateInfo::maxSets;
			using vk::DescriptorPoolCreateInfo::poolSizeCount;
			using vk::DescriptorPoolCreateInfo::pPoolSizes;
		private:
			using vk::DescriptorPoolCreateInfo::sType;
			using vk::DescriptorPoolCreateInfo::pNext;
		};

		struct Parameter {
			CreateInfo* pInfo = nullptr;
			std::optional<Key::LogicalDeviceVariantKey> logicalDeviceKey = std::nullopt;
		};

	public:
		explicit DescriptorPool(ManagerPassKey, const Data::DescriptorPoolData& data, const Parameter& parameter);
		~DescriptorPool() = default;
		FluidumUtils_Class_Default_CopyMove(DescriptorPool)

	private:
		void create(const Data::DescriptorPoolData& data, const Parameter& parameter);
	public:
		const Data::DescriptorPoolInfo& get() const noexcept;
		void destroy();

	private:
		Data::DescriptorPoolInfo info = {};
	};
}