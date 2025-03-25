#pragma once

#include "variant.h"
#include "converter.h"

namespace FVK::Internal::Key {

	template<IsKeyType T>
	using WindowKey = Key<FvkType::Window, T>;
	template<IsKeyType T>
	using WindowKeys = std::vector<WindowKey<T>>;

	template<IsKeyType T>
	using InstanceKey = Key<FvkType::Instance, T>;
	template<IsKeyType T>
	using InstanceKeys = std::vector<InstanceKey<T>>;

	template<IsKeyType T>
	using MessengerKey = Key<FvkType::Messenger, T>;
	template<IsKeyType T>
	using MessageKeys = std::vector<MessengerKey<T>>;

	template<IsKeyType T>
	using SurfaceKey = Key<FvkType::Surface, T>;
	template<IsKeyType T>
	using SurfaceKeys = std::vector<SurfaceKey<T>>;

	template<IsKeyType T>
	using PhysicalDeviceKey = Key<FvkType::PhysicalDevice, T>;
	template<IsKeyType T>
	using PhysicalDeviceKeys = std::vector<PhysicalDeviceKey<T>>;

	template<IsKeyType T>
	using LogicalDeviceKey = Key<FvkType::LogicalDevice, T>;
	template<IsKeyType T>
	using LogicalDeviceKeys = std::vector<LogicalDeviceKey<T>>;

	template<IsKeyType T>
	using QueueKey = Key<FvkType::Queue, T>;
	template<IsKeyType T>
	using QueueKeys = std::vector<QueueKey<T>>;

	template<IsKeyType T>
	using SwapchainKey = Key<FvkType::Swapchain, T>;
	template<IsKeyType T>
	using SwapchainKeys = std::vector<SwapchainKey<T>>;

	template<IsKeyType T>
	using RenderPassKey = Key<FvkType::RenderPass, T>;
	template<IsKeyType T>
	using RenderPassKeys = std::vector<RenderPassKey<T>>;

	template<IsKeyType T>
	using DescriptorSetLayoutKey = Key<FvkType::DescriptorSetLayout, T>;
	template<IsKeyType T>
	using DescriptorSetLayoutKeys = std::vector<DescriptorSetLayoutKey<T>>;

	template<IsKeyType T>
	using ShaderModuleKey = Key<FvkType::ShaderModule, T>;
	template<IsKeyType T>
	using ShaderModuleKeys = std::vector<ShaderModuleKey<T>>;

	template<IsKeyType T>
	using GraphicsPipelineLayoutKey = Key<FvkType::GraphicsPipelineLayout, T>;
	template<IsKeyType T>
	using GraphicsPipelineLayoutKeys = std::vector<GraphicsPipelineLayoutKey<T>>;

	template<IsKeyType T>
	using GraphicsPipelineKey = Key<FvkType::GraphicsPipeline, T>;
	template<IsKeyType T>
	using GraphicsPipelines = std::vector<GraphicsPipelineKey<T>>;

	template<IsKeyType T>
	using ImageKey = Key<FvkType::Image, T>;
	template<IsKeyType T>
	using ImageKeys = std::vector<ImageKey<T>>;

	template<IsKeyType T>
	using ImageViewKey = Key<FvkType::ImageView, T>;
	template<IsKeyType T>
	using ImageViewKeys = std::vector<ImageViewKey<T>>;

	template<IsKeyType T>
	using DeviceMemoryKey = Key<FvkType::DeviceMemory, T>;
	template<IsKeyType T>
	using DeviceMemoryKeys = std::vector<DeviceMemoryKey<T>>;

	template<IsKeyType T>
	using CommandPoolKey = Key<FvkType::CommandPool, T>;
	template<IsKeyType T>
	using CommandPoolKeys = std::vector<CommandPoolKey<T>>;

	template<IsKeyType T>
	using VertexBufferKey = Key<FvkType::VertexBuffer, T>;
	template<IsKeyType T>
	using VertexBufferKeys = std::vector<VertexBufferKey<T>>;

	template<IsKeyType T>
	using IndexBufferKey = Key<FvkType::IndexBuffer, T>;
	template<IsKeyType T>
	using IndexBufferKeys = std::vector<IndexBufferKey<T>>;

	template<IsKeyType T>
	using UniformBufferKey = Key<FvkType::UniformBuffer, T>;
	template<IsKeyType T>
	using UniformBufferKeys = std::vector<UniformBufferKey<T>>;

	template<IsKeyType T>
	using DescriptorPoolKey = Key<FvkType::DescriptorPool, T>;
	template<IsKeyType T>
	using DescriptorPoolKeys = std::vector<DescriptorPoolKey<T>>;

	template<IsKeyType T>
	using DescriptorSetKey = Key<FvkType::DescriptorSet, T>;
	template<IsKeyType T>
	using DescriptorSetKeys = std::vector<DescriptorSetKey<T>>;

	template<IsKeyType T>
	using CommandBufferKey = Key<FvkType::CommandBuffer, T>;
	template<IsKeyType T>
	using CommandBufferKeys = std::vector<CommandBufferKey<T>>;

	template<IsKeyType T>
	using FrameBufferKey = Key<FvkType::FrameBuffer, T>;
	template<IsKeyType T>
	using FrameBufferKeys = std::vector<FrameBufferKey<T>>;

	template<IsKeyType T>
	using SemaphoreKey = Key<FvkType::Semaphore, T>;
	template<IsKeyType T>
	using SemaphoreKeys = std::vector<SemaphoreKey<T>>;

	template<IsKeyType T>
	using FenceKey = Key<FvkType::Fence, T>;
	template<IsKeyType T>
	using FenceKeys = std::vector<FenceKey<T>>;

	template<IsKeyType T>
	using SamplerKey = Key<FvkType::Sampler, T>;
	template<IsKeyType T>
	using SamplerKeys = std::vector<SamplerKey<T>>;

	template<IsKeyType T>
	using TextureKey = Key<FvkType::Texture, T>;
	template<IsKeyType T>
	using TextureKeys = std::vector<TextureKey<T>>;

	template<IsKeyType T>
	using ImGuiKey = Key<FvkType::ImGui, T>;
	template<IsKeyType T>
	using ImGuiKeys = std::vector<ImGuiKey<T>>;

	template<IsKeyType T>
	using ImGuiImageKey = Key<FvkType::ImGuiImage, T>;
	template<IsKeyType T>
	using ImGuiImageKeys = std::vector<ImGuiImageKey<T>>;

}

namespace FVK::Internal::Key {

	using WindowVariantKey = VariantKey<WindowKey>;
	using WindowVariantVectorKey = VariantVectorKey<WindowKey>;

	using InstanceVariantKey = VariantKey<InstanceKey>;
	using InstanceVariantVectorKey = VariantVectorKey<InstanceKey>;

	using MessengerVariantKey = VariantKey<MessengerKey>;
	using MessengerVariantVectorKey = VariantVectorKey<MessengerKey>;

	using SurfaceVariantKey = VariantKey<SurfaceKey>;
	using SurfaceVariantVectorKey = VariantVectorKey<SurfaceKey>;

	using PhysicalDeviceVariantKey = VariantKey<PhysicalDeviceKey>;
	using PhysicalDeviceVariantVectorKey = VariantVectorKey<PhysicalDeviceKey>;

	using LogicalDeviceVariantKey = VariantKey<LogicalDeviceKey>;
	using LogicalDeviceVariantVectorKey = VariantVectorKey<LogicalDeviceKey>;

	using QueueVariantKey = VariantKey<QueueKey>;
	using QueueVariantVectorKey = VariantVectorKey<QueueKey>;

	using SwapchainVariantKey = VariantKey<SwapchainKey>;
	using SwapchainVariantVectorKey = VariantVectorKey<SwapchainKey>;

	using ShaderModuleVariantKey = VariantKey<ShaderModuleKey>;
	using ShaderModuleVariantVectorKey = VariantVectorKey<ShaderModuleKey>;

	using RenderPassVariantKey = VariantKey<RenderPassKey>;
	using RenderPassVariantVectorKey = VariantVectorKey<RenderPassKey>;
	using GraphicsPipelineLayoutVariantKey = VariantKey<GraphicsPipelineLayoutKey>;
	using GraphicsPipelineLayoutVariantVectorKey = VariantVectorKey<GraphicsPipelineLayoutKey>;

	using DescriptorSetLayoutVariantKey = VariantKey<DescriptorSetLayoutKey>;
	using DescriptorSetLayoutVariantVectorKey = VariantVectorKey<DescriptorSetLayoutKey>;

	using ImageVariantKey = VariantKey<ImageKey>;
	using ImageVariantVectorKey = VariantVectorKey<ImageKey>;

	using ImageViewVariantKey = VariantKey<ImageViewKey>;
	using ImageViewVariantVectorKey = VariantVectorKey<ImageViewKey>;

	using DeviceMemoryVariantKey = VariantKey<DeviceMemoryKey>;
	using DeviceMemoryVariantVectorKey = VariantVectorKey<DeviceMemoryKey>;

	using CommandPoolVariantKey = VariantKey<CommandPoolKey>;
	using CommandPoolVariantVectorKey = VariantVectorKey<CommandPoolKey>;

	using UniformBufferVariantKey = VariantKey<UniformBufferKey>;
	using UniformBufferVariantVectorKey = VariantVectorKey<UniformBufferKey>;

	using DescriptorPoolVariantKey = VariantKey<DescriptorPoolKey>;
	using DescriptorPoolVariantVectorKey = VariantVectorKey<DescriptorPoolKey>;

	using DescriptorSetVariantKey = VariantKey<DescriptorSetKey>;
	using DescriptorSetVariantVectorKey = VariantVectorKey<DescriptorSetKey>;

	using CommandBufferVariantKey = VariantKey<CommandBufferKey>;
	using CommandBufferVariantVectorKey = VariantVectorKey<CommandBufferKey>;

	using FrameBufferVariantKey = VariantKey<FrameBufferKey>;
	using FrameBufferVariantVectorKey = VariantVectorKey<FrameBufferKey>;

	using SemaphoreVariantKey = VariantKey<SemaphoreKey>;
	using SemaphoreVariantVectorKey = VariantVectorKey<SemaphoreKey>;

	using FenceVariantKey = VariantKey<FenceKey>;
	using FenceVariantVectorKey = VariantVectorKey<FenceKey>;

	using SamplerVariantKey = VariantKey<SamplerKey>;
	using SamplerVariantVectorKey = VariantVectorKey<SamplerKey>;

	using TextureVariantKey = VariantKey<TextureKey>;
	using TextureVariantVectorKey = VariantVectorKey<TextureKey>;

	using ImGuiVariantKey = VariantKey<ImGuiKey>;
	using ImGuiVariantVectorKey = VariantVectorKey<ImGuiKey>;

}
