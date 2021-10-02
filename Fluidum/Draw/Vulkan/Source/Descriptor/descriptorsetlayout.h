#pragma once

#include "../Common/FVKinclude.h"

namespace FVK::Internal {

	class DescriptorSetLayout final {
	public:

		struct CreateInfo : public vk::DescriptorSetLayoutCreateInfo {
		public:
			using vk::DescriptorSetLayoutCreateInfo::flags;
			using vk::DescriptorSetLayoutCreateInfo::bindingCount;
			using vk::DescriptorSetLayoutCreateInfo::pBindings;
		private:
			using vk::DescriptorSetLayoutCreateInfo::sType;
			using vk::DescriptorSetLayoutCreateInfo::pNext;
		};

		struct Parameter {
			CreateInfo* pInfo;
			std::optional<Key::LogicalDeviceVariantKey> logicalDeviceKey = std::nullopt;
		};

	public:
		explicit DescriptorSetLayout(ManagerPassKey, const Data::DescriptorSetLayoutData data, const Parameter& parameter);
		~DescriptorSetLayout() = default;
		FluidumUtils_Class_Default_CopyMove(DescriptorSetLayout)

	private:
		void create(const Data::DescriptorSetLayoutData data, const Parameter& parameter);
	public:
		const Data::DescriptorSetLayoutInfo& get() const noexcept;
		void destroy();

	private:
		Data::DescriptorSetLayoutInfo info;
	};
}