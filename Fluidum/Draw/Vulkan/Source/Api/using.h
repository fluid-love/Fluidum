#pragma once

#include "../Manager/FVKmanager.h"

//Key
namespace FVK {
	using Internal::Key::StringKeyType;
	using Internal::Key::CharKeyType;
	using Internal::Key::IndexKeyType;
	using Internal::Key::SelectKeyType;
}

//Window
namespace FVK {
	using WindowParameter = Internal::Window::NormalParameter;
	using WindowFullScreenParameter = Internal::Window::FullScreenParameter;
	using WindowMaximizedParameter = Internal::Window::MaximizedParameter;

	using Internal::Key::WindowKey;
}

//Instance Messanger
namespace FVK {
	using InstanceParameter = Internal::Instance::Parameter;
	using InstanceMessangerParameter = Internal::Instance::MessangerParameter;
	using MessangerParameter = Internal::Messanger::Parameter;
	using Message = Internal::Messanger::Message;
	using ApplicationInfo = Internal::Instance::ApplicationInfo;
	using VulkanApiVersion = Internal::Instance::VulkanApiVersion;

	using Internal::Key::InstanceKey;
}

//Surface
namespace FVK {
	using SurfaceParameter = Internal::Surface::Parameter;

	using Internal::Key::SurfaceKey;
	using SurfaceConnectionKeys = Internal::Key::RequiredDataKeys<Internal::FvkType::Surface>;
}

//PhysicalDevice
namespace FVK {
	using PhysicalDeviceExtension = Internal::PhysicalDevice::Extension;
	using PhysicalDeviceFeature = Internal::PhysicalDevice::Feature;
	using PhysicalDeviceParameter = Internal::PhysicalDevice::Parameter;

	using PhysicalDeviceSwapchainParameter = Internal::PhysicalDevice::SwapchainParameter;

	using Internal::Key::PhysicalDeviceKey;
	using PhysicalDeviceConnectionKeys = Internal::Key::RequiredDataKeys<Internal::FvkType::PhysicalDevice>;
	using PhysicalDeviceSwapchainConnectionKeys = Internal::Key::RequiredDataKeys<Internal::FvkType::PhysicalDevice_Swapchain>;

}

//LogicalDevice
namespace FVK {
	using LogicalDeviceParameter = Internal::LogicalDevice::Parameter;

	using Internal::Key::LogicalDeviceKey;
	using LogicalDeviceConnectionKeys = Internal::Key::RequiredDataKeys<Internal::FvkType::LogicalDevice>;

}

//Queue
namespace FVK {
	using QueueGetInfo = Internal::Queue::GetInfo;
	using QueueParameter = Internal::Queue::Parameter;

	using Internal::Key::QueueKey;
	using Internal::Key::QueueKeys;
}


//Swapchain
namespace FVK {
	using SwapchainCreateInfo = Internal::Swapchain::CreateInfo;
	using SwapchainParameter = Internal::Swapchain::Parameter;

	using Internal::Key::SwapchainKey;
	using SwapchainConnectionKeys = Internal::Key::RequiredDataKeys<Internal::FvkType::Swapchain>;
}

//RenderPass
namespace FVK {
	using RenderPassCreateInfo = Internal::RenderPass::CreateInfo;
	using RenderPassParameter = Internal::RenderPass::Parameter;

	using Internal::Key::RenderPassKey;
	using RenderPassConnectionKeys = Internal::Key::RequiredDataKeys<Internal::FvkType::RenderPass>;


}

//DescriptorSetLayout
namespace FVK {
	using DescriptorSetLayoutCreateInfo = Internal::DescriptorSetLayout::CreateInfo;
	using DescriptorSetLayoutParameter = Internal::DescriptorSetLayout::Parameter;

	using Internal::Key::DescriptorSetLayoutKey;
	using Internal::Key::DescriptorSetLayoutKeys;
	using DescriptorSetLayoutConnectionKeys = Internal::Key::RequiredDataKeys<Internal::FvkType::DescriptorSetLayout>;
}

//ShaderModule
namespace FVK {
	using ShaderModuleCreateInfo = Internal::ShaderModule::CreateInfo;
	using ShaderModuleParameter = Internal::ShaderModule::Parameter;

	using Internal::Key::ShaderModuleKey;
	using Internal::Key::ShaderModuleKeys;


	using ShaderModuleConnectionKeys = Internal::Key::RequiredDataKeys<Internal::FvkType::ShaderModule>;
}

//GraphicsPipelineLayout
namespace FVK {
	using GraphicsPipelineLayoutCreateInfo = Internal::GraphicsPipelineLayout::CreateInfo;
	using GraphicsPipelineLayoutParameter = Internal::GraphicsPipelineLayout::Parameter;

	using Internal::Key::GraphicsPipelineLayoutKey;
}

//GraphicsPipeline
namespace FVK {
	using PipelineShaderStageCreateInfo = Internal::GraphicsPipeline::PipelineShaderStageCreateInfo;

	using GraphicsPipelineCreateInfo = Internal::GraphicsPipeline::CreateInfo;
	using GraphicsPipelineParameter = Internal::GraphicsPipeline::Parameter;

	using Internal::Key::GraphicsPipelineKey;
}


//Image
namespace FVK {
	using ImageCreateInfo = Internal::Image::CreateInfo;
	using ImageParameter = Internal::Image::Parameter;

	using Internal::Key::ImageKey;
}

//ImageView
namespace FVK {
	using ImageViewCreateInfo = Internal::ImageView::CreateInfo;
	using ImageViewParameter = Internal::ImageView::Parameter;
	using SwapchainImageViewParameter = Internal::ImageView::SwapchainParameter;

	using Internal::Key::ImageViewKey;

}

//DeviceMemory
namespace FVK {
	using DeviceMemoryAllocateInfo = Internal::DeviceMemory::AllocateInfo;
	using DeviceMemoryParameter = Internal::DeviceMemory::Parameter;

	using Internal::Key::DeviceMemoryKey;
}

//CommandPool
namespace FVK {
	using CommandPoolCreateInfo = Internal::CommandPool::CreateInfo;
	using CommandPoolParameter = Internal::CommandPool::Parameter;

	using Internal::Key::CommandPoolKey;
}

//VertexBuffer
namespace FVK {
	using VertexBufferParameter = Internal::VertexBuffer::Parameter;

	using Internal::Key::VertexBufferKey;
}

//IndexBuffer
namespace FVK {
	using IndexBufferParameter = Internal::IndexBuffer::Parameter;

	using Internal::Key::IndexBufferKey;
}

//UniformBuffer
namespace FVK {
	using UBO = Internal::UniformBuffer::Object;
	//using DeviceMemoryAllocateInfo = Internal::DeviceMemory::AllocateInfo;
	using UniformBufferParameter = Internal::UniformBuffer::Parameter;

	using Internal::Key::UniformBufferKey;
}

//DescriptorPool
namespace FVK {
	using DescriptorPoolCreateInfo = Internal::DescriptorPool::CreateInfo;
	using DescriptorPoolParameter = Internal::DescriptorPool::Parameter;

	using Internal::Key::DescriptorPoolKey;
}

//DescriptorSet
namespace FVK {
	using DescriptorSetAllocateInfo = Internal::DescriptorSet::AllocateInfo;
	using DescriptorSetParameter = Internal::DescriptorSet::Parameter;

	using Internal::Key::DescriptorSetKey;
	using Internal::Key::DescriptorSetKeys;

	using UpdateDescriptorSetUniformBufferInfo = Internal::UpdateDescriptorSet::UniformBufferInfo;
	using UpdateDescriptorSetWriteInfo = Internal::UpdateDescriptorSet::Write;
	using UpdateDescriptorSetParameter = Internal::UpdateDescriptorSet::Parameter;


}

//CommandBuffer
namespace FVK {
	using CommandBufferAllocateInfo = Internal::CommandBuffer::AllocateInfo;
	using CommandBufferParameter = Internal::CommandBuffer::Parameter;

	using Internal::Key::CommandBufferKey;
	using Internal::Key::CommandBufferKeys;

}

//FrameBuffer
namespace FVK {
	using FrameBufferCreateInfo = Internal::FrameBuffer::CreateInfo;
	using FrameBufferParameter = Internal::FrameBuffer::Parameter;

	using Internal::Key::FrameBufferKey;
	using Internal::Key::FrameBufferKeys;

}

//Semaphore
namespace FVK {
	using SemaphoreCreateInfo = Internal::Semaphore::CreateInfo;
	using SemaphoreParameter = Internal::Semaphore::Parameter;

	using Internal::Key::SemaphoreKeys;
	using Internal::Key::SemaphoreKey;

}

//Fence
namespace FVK {
	using FenceCreateInfo = Internal::Fence::CreateInfo;
	using FenceParameter = Internal::Fence::Parameter;

	using Internal::Key::FenceKey;
	using Internal::Key::FenceKeys;

}

//Sampler
namespace FVK {
	using SamplerCreateInfo = Internal::Sampler::CreateInfo;
	using SamplerParameter = Internal::Sampler::Parameter;

	using Internal::Key::SamplerKey;
	using Internal::Key::SamplerKeys;
}

//Texture
namespace FVK {
	using TextureParameter = Internal::Texture::Parameter;

	using Internal::Key::TextureKey;
	using Internal::Key::TextureKeys;
}

//Draw
namespace FVK {
	//using DrawCeateInfo = Internal::Sampler::CreateInfo;
	//using DrawParameter = Internal::Sampler::Parameter;

	using Internal::Key::DrawKey;
	using Internal::Key::DrawKeys;
}

//ImGui
namespace FVK {
	using ImGuiCreateInfo = Internal::FvkImGui::CreateInfo;
	using ImGuiGlyphRangesType = Internal::FvkImGui::GlyphRangesType;
	using ImGuiFontInfo = Internal::FvkImGui::FontInfo;
	using ImGuiParameter = Internal::FvkImGui::Parameter;

	using Internal::Key::ImGuiKey;
	using Internal::Key::ImGuiKeys;


	using ImGuiImageParameter = Internal::ImGuiImage::Parameter;

	using Internal::Key::ImGuiImageKey;
	using Internal::Key::ImGuiImageKeys;

}

//Command
namespace FVK {
	using BeginCommandBufferCommand = Internal::Command::BeginCommandBufferCommand;
	using EndCommandBufferCommand = Internal::Command::EndCommandBufferCommand;
	using BindVertexBuffersCommand = Internal::Command::BindVertexBuffersCommand;
	using BeginRenderPassCommand = Internal::Command::BeginRenderPassCommand;
	using BindDescriptorSetCommand = Internal::Command::BindDescriptorSetCommand;
	using BindGraphicsPipelineCommand = Internal::Command::BindGraphicsPipelineCommand;
	using DrawIndexedCommand = Internal::Command::DrawIndexedCommand;
	using BindIndexBufferCommand = Internal::Command::BindIndexBufferCommand;
	using EndRenderPassCommand = Internal::Command::EndRenderPassCommand;
	using SetScissorCommand = Internal::Command::SetScissorCommand;
	using SetViewportCommand = Internal::Command::SetViewportCommand;
	using ImGuiRenderDrawDataCommand = Internal::Command::ImGuiRenderDrawDataCommand;
	using BindImGuiImageCommand = Internal::Command::BindImGuiImageCommand;
	using UpdateVertexBufferCommand = Internal::Command::UpdateVertexBufferCommand;
	using UpdateIndexBufferCommand = Internal::Command::UpdateIndexBufferCommand;
	using TransitionImageLayoutCommand = Internal::Command::TransitionImageLayoutCommand;
	using QueueSubmitCommand = Internal::Command::QueueSubmitCommand;
	using NextCommand = Internal::Command::NextCommand;

	//template<Internal::Key::IsKeyType... T>
	//using AnyCommand = Internal::Command::AnyCommand<T...>;

	using Commands = Internal::Command::Commands;
}

