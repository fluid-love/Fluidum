#pragma once

#include "../Common/include.h"
#include "../Buffer/uniformbuffer.h"

namespace FVK::Internal {

	class UpdateDescriptorSet final {
	public:
		struct UniformBufferInfo : public vk::DescriptorBufferInfo {
		public:
			std::optional<Key::UniformBufferVariantKey> uniformBufferKey = std::nullopt;
			using vk::DescriptorBufferInfo::offset;
			using vk::DescriptorBufferInfo::range;
		private:
			using vk::DescriptorBufferInfo::buffer;
		private:
			friend class UpdateDescriptorSet;
		};

		struct Write : public vk::WriteDescriptorSet {
		public:
			std::optional<Key::DescriptorSetVariantKey> descriptorSetKey = std::nullopt;
			using vk::WriteDescriptorSet::dstBinding;
			using vk::WriteDescriptorSet::dstArrayElement;
			using vk::WriteDescriptorSet::descriptorCount;
			using vk::WriteDescriptorSet::descriptorType;
			using vk::WriteDescriptorSet::pImageInfo;
			UniformBufferInfo* pUniformBufferInfo = nullptr;
			using vk::WriteDescriptorSet::pTexelBufferView;
		private:
			using vk::WriteDescriptorSet::sType;
			using vk::WriteDescriptorSet::pNext;
			using vk::WriteDescriptorSet::dstSet;
			using vk::WriteDescriptorSet::pBufferInfo;
		private:
			friend class UpdateDescriptorSet;
		};

		struct Parameter {
			Write* pInfo = nullptr;
			std::optional<Key::LogicalDeviceVariantKey> logicalDeviceKey = std::nullopt;
		};
	public:
		vk::Device device = nullptr;
		vk::Buffer buffer = nullptr;
		vk::DescriptorSet descriptorSet = nullptr;
	public:
		void updateUniformBuffer(Parameter& info);
	};


}