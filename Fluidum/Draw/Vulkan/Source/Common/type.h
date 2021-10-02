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

#include <FluidumUtilsDebug/FUdebug.h>
#include <FluidumUtilsClass/FUclass.h>
#include <FluidumUtilsConcept/FUconcept.h>
#include <FluidumUtilsTuple/FUtuple.h>
#include <FluidumUtilsFile/FUfile.h>
#include <FluidumUtilsLog/FUlog.h>

//vulkan
#define VULKAN_HPP_NO_SPACESHIP_OPERATOR
#define VULKAN_HPP_NO_EXCEPTIONS
#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#define VULKAN_HPP_NO_SETTERS //setterÇè¡Ç∑Å@ç°ÇÃvulkanSDKÇ≈ÇÕëŒâûÇµÇƒÇ¢Ç»Ç¢

FluidumUtils_Debug_BeginDisableAllWarning //C26812ÇÕè¡Ç¶Ç»Ç¢
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
		Messanger,
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


		//OffscreenFrameBuffer,



		//ViewPort,

		//SwapchainImageView,


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
