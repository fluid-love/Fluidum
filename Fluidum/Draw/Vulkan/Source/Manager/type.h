#pragma once

#include "include.h"

namespace FVK::Internal::Manager {

	using Windows = std::vector<std::unique_ptr<Window>>;//resized callback
	using Instances = std::vector<std::unique_ptr<Instance>>;
	using Messengers = std::vector<std::unique_ptr<Messenger>>;
	using Surfaces = std::vector<std::unique_ptr<Surface>>;
	using PhysicalDevices = std::vector<std::unique_ptr<PhysicalDevice>>;
	using LogicalDevices = std::vector<std::unique_ptr<LogicalDevice>>;
	using Queues = std::vector<std::unique_ptr<Queue>>;
	using Swapchains = std::vector<std::unique_ptr<Swapchain>>;
	using RenderPasses = std::vector<std::unique_ptr<RenderPass>>;
	using DescriptorSetLayouts = std::vector<std::unique_ptr<DescriptorSetLayout>>;
	using ShaderModules = std::vector<std::unique_ptr<ShaderModule>>;
	using GraphicsPipelineLayouts = std::vector<std::unique_ptr<GraphicsPipelineLayout>>;
	using GraphicsPipelines = std::vector<std::unique_ptr<GraphicsPipeline>>;
	using Images = std::vector<std::unique_ptr<Image>>;
	using ImageViews = std::vector<std::unique_ptr<ImageView>>;
	using DeviceMemories = std::vector<std::unique_ptr<DeviceMemory>>;
	using CommandPools = std::vector<std::unique_ptr<CommandPool>>;
	using VertexBuffers = std::vector<std::unique_ptr<VertexBuffer>>;
	using IndexBuffers = std::vector<std::unique_ptr<IndexBuffer>>;
	using UniformBuffers = std::vector<std::unique_ptr<UniformBuffer>>;
	using DescriptorPools = std::vector<std::unique_ptr<DescriptorPool>>;
	using DescriptorSets = std::vector<std::unique_ptr<DescriptorSet>>;
	using CommandBuffers = std::vector<std::unique_ptr<CommandBuffer>>;
	using FrameBuffers = std::vector<std::unique_ptr<FrameBuffer>>;
	using Semaphores = std::vector<std::unique_ptr<Semaphore>>;
	using Fences = std::vector<std::unique_ptr<Fence>>;
	using Samplers = std::vector<std::unique_ptr<Sampler>>;
	using Textures = std::vector<std::unique_ptr<Texture>>;
	using ImGuis = std::vector<std::unique_ptr<FvkImGui>>;
	using ImGuiImages = std::vector<std::unique_ptr<ImGuiImage>>;

}

namespace FVK::Internal::Manager {

	//used in class Manager.
	using DataTuple = std::tuple <
		Windows,
		Instances,
		Messengers,
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
		ImGuis,

		ImGuiImages
	> ;

	//Correspondence to DataTuple.
	//pair::first->FvkType | pair::second->index
	using CorrespondenceType = std::array<std::pair<FvkType, uint16_t>, 40>;
	constexpr inline CorrespondenceType Correspondence = {
		std::make_pair(FvkType::Window, 0),
		std::make_pair(FvkType::Instance, 1),
		std::make_pair(FvkType::Messenger, 2),
		std::make_pair(FvkType::Surface, 3),
		std::make_pair(FvkType::PhysicalDevice, 4),
		std::make_pair(FvkType::PhysicalDevice_Swapchain, 4),
		std::make_pair(FvkType::LogicalDevice, 5),
		std::make_pair(FvkType::Queue, 6),
		std::make_pair(FvkType::Queue_Vector, 6),
		std::make_pair(FvkType::Swapchain, 7),

		std::make_pair(FvkType::RenderPass, 8),
		std::make_pair(FvkType::DescriptorSetLayout, 9),
		std::make_pair(FvkType::DescriptorSetLayout_Vector, 9),
		std::make_pair(FvkType::ShaderModule, 10),
		std::make_pair(FvkType::ShaderModule_Vector, 10),
		std::make_pair(FvkType::GraphicsPipelineLayout, 11),
		std::make_pair(FvkType::GraphicsPipeline, 12),
		std::make_pair(FvkType::Image, 13),
		std::make_pair(FvkType::ImageView, 14),
		std::make_pair(FvkType::ImageView_Swapchain, 14),

		std::make_pair(FvkType::ImageView_Vector, 14),
		std::make_pair(FvkType::DeviceMemory, 15),
		std::make_pair(FvkType::CommandPool, 16),
		std::make_pair(FvkType::VertexBuffer, 17),
		std::make_pair(FvkType::IndexBuffer, 18),
		std::make_pair(FvkType::UniformBuffer, 19),
		std::make_pair(FvkType::DescriptorPool, 20),
		std::make_pair(FvkType::DescriptorSet, 21),
		std::make_pair(FvkType::CommandBuffer, 22),
		std::make_pair(FvkType::FrameBuffer, 23),

		std::make_pair(FvkType::Semaphore, 24),
		std::make_pair(FvkType::Semaphore_Vector, 24),
		std::make_pair(FvkType::Fence, 25),
		std::make_pair(FvkType::Fence_Vector, 25),
		std::make_pair(FvkType::Sampler, 26),
		std::make_pair(FvkType::Texture, 27),
		std::make_pair(FvkType::ImGui, 28),
		std::make_pair(FvkType::ImGuiImage, 29)

	};

	//Get Index from Type.
	template<FvkType Type>
	consteval CorrespondenceType::value_type::second_type CorrespondenceAt() {
		auto func = [&](auto& x) {return x.first == Type; };
		auto itr = std::find_if(Correspondence.cbegin(), Correspondence.cend(), func);
		assert(itr != Correspondence.cend());
		return itr->second;
	}

	template<typename T>
	consteval FvkType ItemTypeToFvkType() {
		if constexpr (std::same_as<T, Window>)
			return FvkType::Window;
		else if constexpr (std::same_as<T, Instance>)
			return FvkType::Instance;
		else if constexpr (std::same_as<T, Messenger>)
			return FvkType::Messenger;
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
		else if constexpr (std::same_as<T, FvkImGui>)
			return FvkType::ImGui;
		else if constexpr (std::same_as<T, ImGuiImage>)
			return FvkType::ImGuiImage;
		else
			FU::Concept::DelayAssert_T<T>;
	}

}

namespace FVK::Internal {

	//Whether we need to waitidle or not.
	template<typename Info>
	concept IsRequireWaitIdle = requires(Info info) {
		info.device;
	};

}