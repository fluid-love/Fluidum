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

//utils
#include "../../../../Utils/include.h"

//vulkan
//If VULKAN_HPP_NO_SPACESHIP_OPERATOR is not defined, "noexcept" modifier will be lost.
#define VULKAN_HPP_NO_SPACESHIP_OPERATOR 
#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#define VULKAN_HPP_NO_SETTERS
#define VULKAN_HPP_NO_EXCEPTIONS

FluidumUtils_Debug_BeginDisableAllWarning
#include <vulkan/vulkan.hpp>
FluidumUtils_Debug_EndDisableAllWarning


namespace FVK::Internal {
	//underlying_type
	using UT = UIF16;
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

}

namespace FVK::Internal {
	template<Size N>
	using DataTypeArray = std::array<FvkType, N>;
}
