#pragma once

#include "../Common/include.h"

namespace FVK::Internal {

	class GraphicsPipelineLayout final {
	public:
		struct CreateInfo : public vk::PipelineLayoutCreateInfo {
		public:
			Key::DescriptorSetLayoutVariantVectorKey descriptorSetLayoutKeys{};//pSetLayouts
			using vk::PipelineLayoutCreateInfo::setLayoutCount;
			using vk::PipelineLayoutCreateInfo::pushConstantRangeCount;
			using vk::PipelineLayoutCreateInfo::pPushConstantRanges;
		private:
			using vk::PipelineLayoutCreateInfo::pSetLayouts;
			using vk::PipelineLayoutCreateInfo::sType;
			using vk::PipelineLayoutCreateInfo::pNext;
			using vk::PipelineLayoutCreateInfo::flags;
		private:
			friend class GraphicsPipelineLayout;
		};

		struct Parameter {
			CreateInfo* pInfo = nullptr;
			std::optional<Key::LogicalDeviceVariantKey> logicalDevice = std::nullopt;
		};

	public:
		/*
		Exception:
			FailedToCreate
		*/
		//strong
		explicit GraphicsPipelineLayout(ManagerPassKey, const Data::GraphicsPipelineLayoutData& data, Parameter& parameter);
		
		~GraphicsPipelineLayout() noexcept = default;
		FluidumUtils_Class_Default_CopyMove(GraphicsPipelineLayout)

	private:
		/*
		Exception:
			FailedToCreate
		*/
		//strong
		void create(const Data::GraphicsPipelineLayoutData& data, Parameter& parameter);
	
	public:
		//no-throw
		void destroy() noexcept;

	public:
		//no-throw
		[[nodiscard]] const Data::GraphicsPipelineLayoutInfo& get() const noexcept;

	private:
		Data::GraphicsPipelineLayoutInfo info{};

	};

}