#pragma once

#include "include.h"

namespace FVK::Internal::Manager {

	template<typename T>
	concept IsWindowParameter = FU::Concept::IsInstanceOfNonType<std::remove_const_t<T>, Window::Parameter>;

	template<typename T>
	concept IsParameter =
		IsWindowParameter<T> ||
		std::same_as<T, const Instance::Instance::Parameter> ||
		std::same_as<T, const Instance::Instance::MessengerParameter> ||
		std::same_as<T, const Messenger::Parameter> ||
		std::same_as<T, const Surface::Parameter> ||
		std::same_as<T, const PhysicalDevice::Parameter> ||
		std::same_as<T, const PhysicalDevice::SwapchainParameter> ||
		std::same_as<T, const LogicalDevice::Parameter> ||
		std::same_as<T, const Queue::Parameter> ||
		std::same_as<T, Swapchain::Parameter> ||
		std::same_as<T, const RenderPass::Parameter> ||
		std::same_as<T, const DescriptorSetLayout::Parameter> ||
		std::same_as<T, const ShaderModule::Parameter> ||
		std::same_as<T, GraphicsPipelineLayout::Parameter> ||
		std::same_as<T, GraphicsPipeline::Parameter> ||
		std::same_as<T, const Image::Parameter> ||
		std::same_as<T, ImageView::Parameter> ||
		std::same_as<T, const ImageView::SwapchainParameter> ||
		std::same_as<T, const DeviceMemory::Parameter> ||
		std::same_as<T, const CommandPool::Parameter> ||
		std::same_as<T, const VertexBuffer::Parameter> ||
		std::same_as<T, const IndexBuffer::Parameter> ||
		std::same_as<T, const UniformBuffer::Parameter> ||
		std::same_as<T, const DescriptorPool::Parameter> ||
		std::same_as<T, DescriptorSet::Parameter> ||
		std::same_as<T, CommandBuffer::Parameter> ||
		std::same_as<T, FrameBuffer::Parameter> ||
		std::same_as<T, const Semaphore::Parameter> ||
		std::same_as<T, const Fence::Parameter> ||
		std::same_as<T, const Sampler::Parameter>||
		std::same_as<T, const Texture::Parameter>||
		std::same_as<T, const FvkImGui::Parameter>||
		std::same_as<T, const ImGuiImage::Parameter>
		;

}