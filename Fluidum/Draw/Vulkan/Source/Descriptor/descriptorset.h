#pragma once

#include "FVKupdatedescriptorset.h"

namespace FVK::Internal {

	class DescriptorSet final {
	public:

		struct AllocateInfo : public vk::DescriptorSetAllocateInfo {
		public:
			std::optional<Key::DescriptorPoolVariantKey> descriptorPoolKey = std::nullopt;
			using vk::DescriptorSetAllocateInfo::descriptorSetCount;
			std::optional<Key::DescriptorSetLayoutVariantVectorKey> decsriptorSetLayoutKeys = std::nullopt;
		private:
			using vk::DescriptorSetAllocateInfo::sType;
			using vk::DescriptorSetAllocateInfo::pNext;
			using vk::DescriptorSetAllocateInfo::descriptorPool;
			using vk::DescriptorSetAllocateInfo::pSetLayouts;
		private:
			friend DescriptorSet;
		};

		struct Parameter {
			AllocateInfo* pInfo = nullptr;
			std::optional<Key::LogicalDeviceVariantKey> logicalDeviceKey = std::nullopt;
		};

	public:
		DescriptorSet(ManagerPassKey, const Data::DescriptorSetData& data, Parameter& parameter);
		~DescriptorSet() = default;
		FluidumUtils_Class_Default_CopyMove(DescriptorSet)

	private:
		void allocate(const Data::DescriptorSetData& data, Parameter& parameter);
	public:
		const Data::DescriptorSetInfo& get() const noexcept;
		void destroy();


	private:
		Data::DescriptorSetInfo info = {};
	};
}