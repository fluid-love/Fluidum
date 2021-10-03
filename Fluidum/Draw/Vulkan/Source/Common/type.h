#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <memory>
#include <algorithm>
#include <map>
#include <variant>
#include <type_traits>
#include <optional>
#include <set>
#include <fstream>
#include <functional>
#include <any>
#include <condition_variable>
#include <sstream>

#include "../../../../Utils/include.h"

//vulkan
#define VULKAN_HPP_NO_SPACESHIP_OPERATOR
#define VULKAN_HPP_NO_EXCEPTIONS
#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#define VULKAN_HPP_NO_SETTERS

FluidumUtils_Debug_BeginDisableAllWarning 
#include <vulkan/vulkan.hpp>
FluidumUtils_Debug_EndDisableAllWarning


namespace FVK::Internal {
	//underlying_type
	using UT = uint16_t;
}

namespace FVK::Internal {

	enum class FvkType : UT {
		None,
		Glfw,
		Window,
		Instance,
		Messenger,
		Surface,
		PhysicalDevice,
		PhysicalDevice_Swapchain,
		LogicalDevice,
		Queue,
		Queue_Vector,
		Swapchain,
		RenderPass,
		DescriptorSetLayout,
		DescriptorSetLayout_Vector,
		ShaderModule,
		ShaderModule_Vector,
		GraphicsPipeline,
		GraphicsPipelineLayout,
		Image,
		ImageView,
		ImageView_Swapchain,
		ImageView_Vector,
		DeviceMemory,
		CommandPool,
		VertexBuffer,
		IndexBuffer,
		UniformBuffer,
		DescriptorPool,
		DescriptorSet,
		CommandBuffer,
		FrameBuffer,
		Semaphore,
		Semaphore_Vector,
		Fence,
		Fence_Vector,
		Sampler,
		Texture,
		Draw,
		ImGui,
		ImGuiImage,

		COUNT,


	};

	enum class CommandType : UT {
		None = static_cast<UT>(FvkType::COUNT),
		BeginCommandBuffer,
		EndCommandBuffer,
		BindVertexBuffers,
		BindIndexBuffer,
		DrawIndexed,
		BindDescriptorSet,
		BindGraphicsPipeline,
		BeginRenderPass,
		EndRenderPass,
		SetScissor,
		SetViewport,
		ImGuiRenderDrawData,
		BindImGuiImage,
		UpdateVertexBuffer,
		UpdateIndexBuffer,
		TransitionImageLayout,
		QueueSubmit,
		Next,
		Any,
		Commands,

		COUNT
	};


	template<std::size_t Size>
	using DataTypeArray = std::array<FvkType, Size>;

}
