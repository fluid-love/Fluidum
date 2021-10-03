#pragma once

#include "../Common/include.h"

namespace FVK::Internal {

	class GraphicsPipeline final {
	public:

		struct PipelineShaderStageCreateInfo : public vk::PipelineShaderStageCreateInfo {
		public:
			using vk::PipelineShaderStageCreateInfo::flags;
			using vk::PipelineShaderStageCreateInfo::stage;
			std::optional<Key::ShaderModuleVariantKey> shaderModuleKey = std::nullopt;
			using vk::PipelineShaderStageCreateInfo::pName;
			using vk::PipelineShaderStageCreateInfo::pSpecializationInfo;
		private:
			using vk::PipelineShaderStageCreateInfo::module;
			using vk::PipelineShaderStageCreateInfo::sType;
			using vk::PipelineShaderStageCreateInfo::pNext;
		private:
			friend class GraphicsPipeline;
		};

		struct CreateInfo : public vk::GraphicsPipelineCreateInfo {
			using vk::GraphicsPipelineCreateInfo::flags;
			using vk::GraphicsPipelineCreateInfo::stageCount;
			PipelineShaderStageCreateInfo* pStageCreateInfos = nullptr;
			using vk::GraphicsPipelineCreateInfo::pVertexInputState;
			using vk::GraphicsPipelineCreateInfo::pInputAssemblyState;
			using vk::GraphicsPipelineCreateInfo::pTessellationState;
			using vk::GraphicsPipelineCreateInfo::pViewportState;
			using vk::GraphicsPipelineCreateInfo::pRasterizationState;
			using vk::GraphicsPipelineCreateInfo::pMultisampleState;
			using vk::GraphicsPipelineCreateInfo::pDepthStencilState;
			using vk::GraphicsPipelineCreateInfo::pColorBlendState;
			using vk::GraphicsPipelineCreateInfo::pDynamicState;
			std::optional<Key::GraphicsPipelineLayoutVariantKey> graphicsPipelineLayoutKey = std::nullopt;
			std::optional<Key::RenderPassVariantKey> renderPassKey = std::nullopt;
			using vk::GraphicsPipelineCreateInfo::subpass;
			using vk::GraphicsPipelineCreateInfo::basePipelineHandle;
			using vk::GraphicsPipelineCreateInfo::basePipelineIndex;
		private:
			using vk::GraphicsPipelineCreateInfo::sType;
			using vk::GraphicsPipelineCreateInfo::pNext;
			using vk::GraphicsPipelineCreateInfo::pStages;
			using vk::GraphicsPipelineCreateInfo::layout;
			using vk::GraphicsPipelineCreateInfo::renderPass;
		private:
			friend class GraphicsPipeline;
		};

		struct Parameter {
			CreateInfo* pInfo = nullptr;
			std::optional<Key::LogicalDeviceVariantKey> logicalDeviceKey = std::nullopt;
		};

	public:
		explicit GraphicsPipeline(ManagerPassKey, const Data::GraphicsPipelineData& data, Parameter& parameter);
		~GraphicsPipeline() = default;
		FluidumUtils_Class_Default_CopyMove(GraphicsPipeline)

	private:
		void create(const Data::GraphicsPipelineData& data, Parameter& parameter);
	public:
		const Data::GraphicsPipelineInfo& get() const noexcept;
		void destroy();

	private:
		void fillParameter(const Data::GraphicsPipelineData& data, Parameter& parameter);

	private:
		//FVK::PipelineShaderStageCreateInfoÇ∆vk::PipelineShaderStageCreateInfoÇÃÉÅÉÇÉäí≤êÆóp
		std::vector<vk::PipelineShaderStageCreateInfo> align = {};
		const vk::PipelineShaderStageCreateInfo* alignPipelineShaderStageCreateInfo(const Parameter& parameter,const std::size_t size);

	private:
		Data::GraphicsPipelineInfo info = {};
	};
}