#pragma once

#include "../Common/include.h"

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
		/*
		Exception:
			FailedToCreate
		*/
		//strong
		explicit DescriptorSetLayout(ManagerPassKey, const Data::DescriptorSetLayoutData data, const Parameter& parameter);
		
		~DescriptorSetLayout() noexcept = default;
		FluidumUtils_Class_Default_CopyMove(DescriptorSetLayout)

	private:
		/*
		Exception:
			FailedToCreate
		*/
		//strong
		void create(const Data::DescriptorSetLayoutData data, const Parameter& parameter);

	public:
		//no-throw
		void destroy() noexcept;

	public:
		//no-throw
		const Data::DescriptorSetLayoutInfo& get() const noexcept;

	private:
		Data::DescriptorSetLayoutInfo info{};

	};

}