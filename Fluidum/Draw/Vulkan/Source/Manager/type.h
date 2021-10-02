#pragma once

#include "FVKinclude.h"

namespace FVK::Internal {

	using Windows = std::vector<Window>;
	using Instances = std::vector<Instance>;
	using Messangers = std::vector<Messanger>;
	using Surfaces = std::vector<Surface>;
	using PhysicalDevices = std::vector<PhysicalDevice>;
	using LogicalDevices = std::vector<LogicalDevice>;
	using Queues = std::vector<Queue>;
	using Swapchains = std::vector<Swapchain>;
	using RenderPasses = std::vector<RenderPass>;
	using DescriptorSetLayouts = std::vector<DescriptorSetLayout>;
	using ShaderModules = std::vector<ShaderModule>;
	using GraphicsPipelineLayouts = std::vector<GraphicsPipelineLayout>;
	using GraphicsPipelines = std::vector<GraphicsPipeline>;
	using Images = std::vector<Image>;
	using ImageViews = std::vector<ImageView>;
	using DeviceMemories = std::vector<DeviceMemory>;
	using CommandPools = std::vector<CommandPool>;
	using VertexBuffers = std::vector<VertexBuffer>;
	using IndexBuffers = std::vector<IndexBuffer>;
	using UniformBuffers = std::vector<UniformBuffer>;
	using DescriptorPools = std::vector<DescriptorPool>;
	using DescriptorSets = std::vector<DescriptorSet>;
	using CommandBuffers = std::vector<CommandBuffer>;
	using FrameBuffers = std::vector<FrameBuffer>;
	using Semaphores = std::vector<Semaphore>;
	using Fences = std::vector<Fence>;
	using Samplers = std::vector<Sampler>;
	using Textures = std::vector<Texture>;
	using Draws = std::vector<Draw>;
	using ImGuis = std::vector<FvkImGui>;
	using ImGuiImages = std::vector<ImGuiImage>;

}

namespace FVK::Internal::Manager {

	//class Manager‚Ådata‚ðˆµ‚¤
	using DataTuple = std::tuple <
		Glfw,
		Windows,
		Instances,
		Messangers,
		Surfaces,
		PhysicalDevices,
		LogicalDevices,
		Queues,
		Swapchains,
		RenderPasses,

		DescriptorSetLayouts,
		ShaderModules,
		GraphicsPipelineLayouts,
		GraphicsPipelines,
		Images,
		ImageViews,
		DeviceMemories,
		CommandPools,
		VertexBuffers,
		IndexBuffers,

		UniformBuffers,
		DescriptorPools,
		DescriptorSets,
		CommandBuffers,
		FrameBuffers,
		Semaphores,
		Fences,
		Samplers,
		Textures,
		Draws,

		ImGuis,
		ImGuiImages
	> ;

	//DataTuple‚Æ‘Î‰ž‚³‚¹‚é
	//pair::first->FvkType‚É‘Î‰ž‚·‚épair::second->index
	using CorrespondenceType = std::array<std::pair<FvkType, uint16_t>, 40>;
	constexpr inline CorrespondenceType Correspondence = {
		std::make_pair(FvkType::Glfw, 0),
		std::make_pair(FvkType::Window, 1),
		std::make_pair(FvkType::Instance, 2),
		std::make_pair(FvkType::Messanger, 3),
		std::make_pair(FvkType::Surface, 4),
		std::make_pair(FvkType::PhysicalDevice, 5),
		std::make_pair(FvkType::PhysicalDevice_Swapchain, 5),
		std::make_pair(FvkType::LogicalDevice, 6),
		std::make_pair(FvkType::Queue, 7),
		std::make_pair(FvkType::Queue_Vector, 7),
		std::make_pair(FvkType::Swapchain, 8),

		std::make_pair(FvkType::RenderPass, 9),
		std::make_pair(FvkType::DescriptorSetLayout, 10),
		std::make_pair(FvkType::DescriptorSetLayout_Vector, 10),
		std::make_pair(FvkType::ShaderModule, 11),
		std::make_pair(FvkType::ShaderModule_Vector, 11),
		std::make_pair(FvkType::GraphicsPipelineLayout, 12),
		std::make_pair(FvkType::GraphicsPipeline, 13),
		std::make_pair(FvkType::Image, 14),
		std::make_pair(FvkType::ImageView, 15),
		std::make_pair(FvkType::ImageView_Swapchain, 15),

		std::make_pair(FvkType::ImageView_Vector, 15),
		std::make_pair(FvkType::DeviceMemory, 16),
		std::make_pair(FvkType::CommandPool, 17),
		std::make_pair(FvkType::VertexBuffer, 18),
		std::make_pair(FvkType::IndexBuffer, 19),
		std::make_pair(FvkType::UniformBuffer, 20),
		std::make_pair(FvkType::DescriptorPool, 21),
		std::make_pair(FvkType::DescriptorSet, 22),
		std::make_pair(FvkType::CommandBuffer, 23),
		std::make_pair(FvkType::FrameBuffer, 24),

		std::make_pair(FvkType::Semaphore, 25),
		std::make_pair(FvkType::Semaphore_Vector, 25),
		std::make_pair(FvkType::Fence, 26),
		std::make_pair(FvkType::Fence_Vector, 26),
		std::make_pair(FvkType::Sampler, 27),
		std::make_pair(FvkType::Texture, 28),
		std::make_pair(FvkType::Draw, 29),
		std::make_pair(FvkType::ImGui, 30),
		std::make_pair(FvkType::ImGuiImage, 31)


	};

	//Type‚©‚çIndex‚ðŽæ“¾
	template<FvkType Type>
	consteval CorrespondenceType::value_type::second_type CorrespondenceAt() {
		auto func = [&](auto& x) {return x.first == Type; };
		auto itr = std::find_if(Correspondence.cbegin(), Correspondence.cend(), func);
		assert(itr != Correspondence.cend());
		return itr->second;
	}

	template<typename T>
	consteval FvkType ItemTypeToFvkType() {
		if constexpr (std::same_as<T, Glfw>)
			return FvkType::Glfw;
		else if constexpr (std::same_as<T, Window>)
			return FvkType::Window;
		else if constexpr (std::same_as<T, Instance>)
			return FvkType::Instance;
		else if constexpr (std::same_as<T, Messanger>)
			return FvkType::Messanger;
		else if constexpr (std::same_as<T, Surface>)
			return FvkType::Surface;
		else if constexpr (std::same_as<T, PhysicalDevice>)
			return FvkType::PhysicalDevice;
		else if constexpr (std::same_as<T, LogicalDevice>)
			return FvkType::LogicalDevice;
		else if constexpr (std::same_as<T, Queue>)
			return FvkType::Queue;
		else if constexpr (std::same_as<T, Swapchain>)
			return FvkType::Swapchain;
		else if constexpr (std::same_as<T, RenderPass>)
			return FvkType::RenderPass;
		else if constexpr (std::same_as<T, DescriptorSetLayout>)
			return FvkType::DescriptorSetLayout;
		else if constexpr (std::same_as<T, ShaderModule>)
			return FvkType::ShaderModule;
		else if constexpr (std::same_as<T, GraphicsPipelineLayout>)
			return FvkType::GraphicsPipelineLayout;
		else if constexpr (std::same_as<T, GraphicsPipeline>)
			return FvkType::GraphicsPipeline;
		else if constexpr (std::same_as<T, Image>)
			return FvkType::Image;
		else if constexpr (std::same_as<T, ImageView>)
			return FvkType::ImageView;
		else if constexpr (std::same_as<T, DeviceMemory>)
			return FvkType::DeviceMemory;
		else if constexpr (std::same_as<T, CommandPool>)
			return FvkType::CommandPool;
		else if constexpr (std::same_as<T, VertexBuffer>)
			return FvkType::VertexBuffer;
		else if constexpr (std::same_as<T, IndexBuffer>)
			return FvkType::IndexBuffer;
		else if constexpr (std::same_as<T, UniformBuffer>)
			return FvkType::UniformBuffer;
		else if constexpr (std::same_as<T, DescriptorPool>)
			return FvkType::DescriptorPool;
		else if constexpr (std::same_as<T, DescriptorSet>)
			return FvkType::DescriptorSet;
		else if constexpr (std::same_as<T, CommandBuffer>)
			return FvkType::CommandBuffer;
		else if constexpr (std::same_as<T, FrameBuffer>)
			return FvkType::FrameBuffer;
		else if constexpr (std::same_as<T, Semaphore>)
			return FvkType::Semaphore;
		else if constexpr (std::same_as<T, Fence>)
			return FvkType::Fence;
		else if constexpr (std::same_as<T, Sampler>)
			return FvkType::Sampler;
		else if constexpr (std::same_as<T, Texture>)
			return FvkType::Texture;
		else if constexpr (std::same_as<T, Draw>)
			return FvkType::Draw;
		else if constexpr (std::same_as<T, FvkImGui>)
			return FvkType::ImGui;
		else if constexpr (std::same_as<T, ImGuiImage>)
			return FvkType::ImGuiImage;

	}

}

namespace FVK::Internal {

	//device waitIdle‚Ì•K—v‚ª‚ ‚é‚©
	template<typename Info>
	concept IsRequireWaitIdle = requires(Info info) {
		info.device;
	};

}