#pragma once

#include "../Common/include.h"
#include "info.h"

namespace FVK::Internal::Data {

	template<FvkType Type>
	using CorrespondenceType =
		std::conditional_t<
		Type == FvkType::Queue_Vector ||
		Type == FvkType::DescriptorSetLayout_Vector ||
		Type == FvkType::ShaderModule_Vector ||
		Type == FvkType::ImageView_Vector ||
		Type == FvkType::Semaphore_Vector ||
		Type == FvkType::Fence_Vector,
		const std::vector<std::reference_wrapper<const Info<ToObjectType<Type>()>>>,
		const std::reference_wrapper<const Info<ToObjectType<Type>()>>
		>;

	template<FvkType Type>
	using CorrespondenceVulkanObject =
		std::conditional_t<Type == FvkType::LogicalDevice, vk::Device,
		std::conditional_t<Type == FvkType::Queue, vk::Queue,
		std::conditional_t<Type == FvkType::Swapchain, vk::SwapchainKHR,
		std::conditional_t<Type == FvkType::DeviceMemory, vk::DeviceMemory,
		std::conditional_t<Type == FvkType::UniformBuffer, std::pair<vk::Buffer, vk::DeviceMemory>,
		std::conditional_t<Type == FvkType::CommandBuffer, std::vector<vk::CommandBuffer>,
		std::conditional_t<Type == FvkType::Semaphore_Vector, std::vector<vk::Semaphore>,
		std::conditional_t<Type == FvkType::Fence_Vector, std::vector<vk::Fence>,

		void
		>>>>>>>>;

	//template<FvkType Type>
	//using CorrespondenceType =
	//	std::conditional_t<Type == FvkType::Window, const std::reference_wrapper<const WindowInfo>,
	//	std::conditional_t<Type == FvkType::Instance, const std::reference_wrapper<const InstanceInfo>,
	//	std::conditional_t<Type == FvkType::Messanger, const std::reference_wrapper<const MessangerInfo>,
	//	std::conditional_t<Type == FvkType::Surface, const std::reference_wrapper<const SurfaceInfo>,
	//	std::conditional_t<Type == FvkType::PhysicalDevice, const std::reference_wrapper<const PhysicalDeviceInfo>,
	//	std::conditional_t<Type == FvkType::PhysicalDevice_Swapchain, const std::reference_wrapper<const PhysicalDeviceInfo>,
	//	std::conditional_t<Type == FvkType::LogicalDevice, const std::reference_wrapper<const LogicalDeviceInfo>,
	//	std::conditional_t<Type == FvkType::Swapchain, const std::reference_wrapper<const SwapchainInfo>,
	//	std::conditional_t<Type == FvkType::RenderPass, const std::reference_wrapper<const RenderPassInfo>,
	//	std::conditional_t<Type == FvkType::DescriptorSetLayout, const std::reference_wrapper<const DescriptorSetLayoutInfo>,

	//	std::conditional_t<Type == FvkType::DescriptorSetLayout_Vector, const std::vector<std::reference_wrapper<const DescriptorSetLayoutInfo>>,
	//	std::conditional_t<Type == FvkType::ShaderModule, const std::reference_wrapper<const ShaderModuleInfo>,
	//	std::conditional_t<Type == FvkType::ShaderModule_Vector, const std::vector<std::reference_wrapper<const ShaderModuleInfo>>,
	//	std::conditional_t<Type == FvkType::GraphicsPipelineLayout, const std::reference_wrapper<const GraphicsPipelineLayoutInfo>,
	//	std::conditional_t<Type == FvkType::GraphicsPipeline, const std::reference_wrapper<const GraphicsPipelineInfo>,
	//	std::conditional_t<Type == FvkType::Image, const std::reference_wrapper<const ImageInfo>,
	//	std::conditional_t<Type == FvkType::ImageView, const std::reference_wrapper<const ImageViewInfo>,
	//	std::conditional_t<Type == FvkType::ImageView_Vector, const std::vector<std::reference_wrapper<const ImageViewInfo>>,
	//	std::conditional_t<Type == FvkType::DeviceMemory, const std::reference_wrapper<const DeviceMemoryInfo>,
	//	std::conditional_t<Type == FvkType::CommandPool, const std::reference_wrapper<const CommandPoolInfo>,

	//	std::conditional_t<Type == FvkType::VertexBuffer, const std::reference_wrapper<const VertexBufferInfo>,
	//	std::conditional_t<Type == FvkType::IndexBuffer, const std::reference_wrapper<const IndexBufferInfo>,
	//	std::conditional_t<Type == FvkType::UniformBuffer, const std::reference_wrapper<const UniformBufferInfo>,
	//	std::conditional_t<Type == FvkType::DescriptorPool, const std::reference_wrapper<const DescriptorPoolInfo>,
	//	std::conditional_t<Type == FvkType::DescriptorSet, const std::reference_wrapper<const DescriptorSetInfo>,
	//	std::conditional_t<Type == FvkType::CommandBuffer, const std::reference_wrapper<const CommandBufferInfo>,
	//	std::conditional_t<Type == FvkType::FrameBuffer, const std::reference_wrapper<const FrameBufferInfo>,
	//	std::conditional_t<Type == FvkType::Semaphore, const std::reference_wrapper<const SemaphoreInfo>,
	//	std::conditional_t<Type == FvkType::Fence, const std::reference_wrapper<const FenceInfo>,
	//	std::conditional_t<Type == FvkType::Sampler, const std::reference_wrapper<const SamplerInfo>,

	//	std::conditional_t<Type == FvkType::Draw, const std::reference_wrapper<const DrawInfo>,

	//	void
	//	>>>>>>>>>>
	//	>>>>>>>>>>
	//	>>>>>>>>>>
	//	> ;


	template<auto TypeArray>
	requires(FU::Concept::IsStdArrayParticularType<decltype(TypeArray), FvkType>)
		using DataTuple = FU::Tuple::ArrayToTupleType<TypeArray, CorrespondenceType>;

}
