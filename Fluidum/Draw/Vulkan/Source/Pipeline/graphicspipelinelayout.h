#pragma once

#include "../Common/FVKinclude.h"

namespace FVK::Internal {

	class GraphicsPipelineLayout final {
	public:
		struct CreateInfo : public vk::PipelineLayoutCreateInfo {
		public:
			//pSetLayouts‚Ì‘ã‚í‚è
			Key::DescriptorSetLayoutVariantVectorKey descriptorSetLayoutKeys = {};
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
			CreateInfo* pInfo;
			std::optional<Key::LogicalDeviceVariantKey> logicalDevice = std::nullopt;
		};
	public:
		explicit GraphicsPipelineLayout(ManagerPassKey, const Data::GraphicsPipelineLayoutData& data, Parameter& parameter);
		~GraphicsPipelineLayout() = default;
		FluidumUtils_Class_Default_CopyMove(GraphicsPipelineLayout)

	private:
		void create(const Data::GraphicsPipelineLayoutData& data, Parameter& parameter);
	public:
		const Data::GraphicsPipelineLayoutInfo& get() const noexcept;
		void destroy();

	private:
		Data::GraphicsPipelineLayoutInfo info = {};
	};
}