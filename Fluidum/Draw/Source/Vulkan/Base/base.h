#pragma once

#include "../../Common/include.h"

//èâä˙âªéûÇ…ï€óLÇ∑ÇÈkey
namespace FDR::Internal {

	constexpr inline FVK::WindowKey<const char*> BaseWindowKey{ "_BW" };
	constexpr inline FVK::InstanceKey<const char*> BaseInstanceKey{ "_BI" };
	constexpr inline FVK::SurfaceKey<const char*> BaseSurfaceKey{ "_BSF" };
	constexpr inline FVK::SwapchainKey<const char*> BaseSwapchainKey{ "_BSC" };
	constexpr inline FVK::PhysicalDeviceKey<const char*> BasePhysicalDeviceKey{ "_BPD" };
	constexpr inline FVK::RenderPassKey<const char*> BaseRenderPassKey{ "_BRP" };
	constexpr inline FVK::LogicalDeviceKey<const char*> BaseDeviceKey{ "_BLD" };
	constexpr inline FVK::DescriptorSetLayoutKey<const char*> BaseDescriptorSetLayoutKey{ "_BDSL" };
	constexpr inline FVK::CommandPoolKey<const char*> BaseCommandPoolKey{ "_BCP" };

	constexpr inline FVK::DescriptorPoolKey<const char*> BaseDescriptorPoolKey{ "_BDP" };

	//color
	constexpr inline FVK::ImageKey<const char*> BaseColorImageKey{ "_BIGC" };
	constexpr inline FVK::DeviceMemoryKey<const char*> BaseColorImageMemoryKey{ "_BIGCDM" };
	constexpr inline FVK::ImageViewKey<const char*> BaseColorImageViewKey{ "_BIGVC" };

	//depth
	constexpr inline FVK::ImageKey<const char*> BaseDepthImageKey{ "_BIGD" };
	constexpr inline FVK::DeviceMemoryKey<const char*> BaseDepthImageMemoryKey{ "_BIGDDM" };
	constexpr inline FVK::ImageViewKey<const char*> BaseDepthImageViewKey{ "_BIGVD" };

	//swapchain
	inline std::vector<FVK::ImageViewKey<std::string>> BaseSwapchainImageViewKeys{};

	constexpr inline FVK::ShaderModuleKey<const char*> BaseVertShaderModuleKey{ "_BVSM" };
	constexpr inline FVK::ShaderModuleKey<const char*> BaseFragShaderModuleKey{ "_BFSM" };

	constexpr inline FVK::GraphicsPipelineLayoutKey<const char*> BaseGraphicsPipelineLayoutKey{ "_BGPL" };
	constexpr inline FVK::GraphicsPipelineKey<const char*> BaseGraphicsPipelineKey{ "_BGP" };

	constexpr inline FVK::QueueKey<const char*> BaseGraphicsQueueKey{ "_BGQ" };
	constexpr inline FVK::QueueKey<const char*> BasePresentQueueKey{ "_BPQ" };

	constexpr inline FVK::CommandBufferKey<const char*> BaseCommandBuffersKey{ "_BCBS" };
	inline std::vector<FVK::FrameBufferKey<std::string>> BaseFrameBufferKeys{};

	inline std::vector<FVK::SemaphoreKey<std::string>> BaseImageAvailableSemaphoreKeys{};
	inline std::vector<FVK::SemaphoreKey<std::string>> BaseRenderFinishedSemaphoreKeys{};
	inline std::vector<FVK::FenceKey<std::string>> BaseInFlightFenceKeys{};


	//ImGui
	constexpr inline FVK::DescriptorPoolKey<const char*> BaseImGuiDescriptorPoolKey{ "_BIGDP" };
	constexpr inline FVK::ImGuiKey<const char*> BaseImGuiKey{ "_BIMGUI" };

	//sampler
	constexpr inline FVK::SamplerKey<const char*> BaseSamplerKey{ "_BSMP" };


	struct Vertex {
		glm::vec3 pos;
		glm::vec4 color;
	};

	constexpr inline int32_t MaxFramesInFlight = 2;

	using Vertices = std::vector<Vertex>;
	using Indices = std::vector<uint32_t>;


}

//Main Commands
namespace FDR::Internal {
	inline FVK::Commands MainCommands{};
}