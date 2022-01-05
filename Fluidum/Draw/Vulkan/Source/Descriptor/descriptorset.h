#pragma once

#include "updatedescriptorset.h"

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
		/*
		Exception:
			std::exception
			FailedToCreate
		*/
		//basic
		/*
		If an exception is thrown, 
		the contents of the private variable in parameter may change.
		However, it is safe.
		*/
		DescriptorSet(ManagerPassKey, const Data::DescriptorSetData& data, Parameter& parameter);

		~DescriptorSet() = default;
		FluidumUtils_Class_Default_CopyMove(DescriptorSet)

	private:
		/*
		Exception:
			std::exception
			FailedToCreate
		*/
		//basic
		void allocate(const Data::DescriptorSetData& data, Parameter& parameter);

	public:
		/*
		Exception:
			FailedToDestroy
		*/		
		//strong
		void destroy() noexcept;

	public:
		//no-throw
		[[nodiscard]] const Data::DescriptorSetInfo& get() const noexcept;

	private:
		Data::DescriptorSetInfo info{};

	};

}