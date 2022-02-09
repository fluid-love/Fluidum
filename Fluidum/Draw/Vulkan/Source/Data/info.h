#pragma once

#include "../Common/fvktype.h"

namespace FVK::Internal::Data {

	template<FvkType Type>
	struct Info final {
		FluidumUtils_Class_Delete_ConDestructor(Info);
	};

	template<>
	struct Info<FvkType::Window> final {
		WindowHandle window = nullptr;
		bool framebufferResized = false;
		bool fullscreen{};
		bool focused{};
		std::pair<I32, I32> sizeMinLimits{};//width, height
		std::pair<I32, I32> sizeMaxLimits{};//width, height
	};

	template<>
	struct Info<FvkType::Instance> final {
		vk::Instance instance = nullptr;
		bool isMessengerCreated = false;
	};

	template<>
	struct Info<FvkType::Messenger> final {
		vk::Instance instance = nullptr;
		VkDebugUtilsMessengerEXT messenger = nullptr;
	};

	template<>
	struct Info<FvkType::Surface> final {
		WindowHandle window = nullptr;
		VkInstance instance = nullptr;
		vk::SurfaceKHR surface = nullptr;
	};

	template<>
	struct Info<FvkType::PhysicalDevice> final {
		vk::PhysicalDevice physicalDevice = nullptr;
		uint32_t graphicsFamily{};
		uint32_t presentFamily{};
		std::vector<const char*> extensionNames{};
		vk::SurfaceCapabilitiesKHR capabilities{};
		std::vector<vk::SurfaceFormatKHR> formats{};
		std::vector<vk::PresentModeKHR> presentModes{};
		vk::SampleCountFlagBits usableMaxMsaaSamples{};

		WindowHandle window = nullptr;
		vk::Instance instance = nullptr;
		vk::SurfaceKHR surface = nullptr;
	};

	template<>
	struct Info<FvkType::LogicalDevice> final {
		WindowHandle window = nullptr;
		vk::Instance instance = nullptr;
		vk::SurfaceKHR surface = nullptr;
		vk::PhysicalDevice physicalDevice = nullptr;
		vk::Device device = nullptr;
	};

	template<>
	struct Info<FvkType::Queue> final {
		vk::Queue queue = nullptr;
	};

	template<>
	struct Info<FvkType::Swapchain> final {
		vk::Device device = nullptr;
		vk::SwapchainKHR swapchain = nullptr;
		vk::Format format = {};
		vk::ColorSpaceKHR colorSpace = {};
		vk::Extent2D extent = {};
		UI32 minImageCount = 0;
	};

	template<>
	struct Info<FvkType::RenderPass> final {
		vk::Device device = nullptr;
		vk::RenderPass renderPass = nullptr;
	};

	template<>
	struct Info<FvkType::DescriptorSetLayout> final {
		vk::Device device = nullptr;
		vk::DescriptorSetLayout descriptorSetLayout = nullptr;
	};

	template<>
	struct Info<FvkType::ShaderModule> final {
		vk::Device device = nullptr;
		vk::ShaderModule shaderModule = nullptr;
	};

	template<>
	struct Info<FvkType::GraphicsPipelineLayout> final {
		vk::Device device = nullptr;
		vk::PipelineLayout graphicsPipelineLayout = nullptr;
	};

	template<>
	struct Info<FvkType::GraphicsPipeline> final {
		vk::Device device = nullptr;
		vk::Pipeline graphicsPipeline = nullptr;
	};

	template<>
	struct Info<FvkType::Image> final {
		vk::Device device = nullptr;
		vk::Image image = nullptr;
	};

	template<>
	struct Info<FvkType::ImageView> final {
		vk::Device device = nullptr;
		vk::ImageView imageView = nullptr;
	};

	template<>
	struct Info<FvkType::DeviceMemory> final {
		vk::Device device = nullptr;
		vk::DeviceMemory deviceMemory = nullptr;
	};

	template<>
	struct Info<FvkType::CommandPool> final {
		vk::Device device = nullptr;
		vk::CommandPool commandPool = nullptr;
	};

	template<>
	struct Info<FvkType::VertexBuffer> final {
		vk::Device device = nullptr;
		vk::Buffer vertexBuffer = nullptr;
		vk::DeviceMemory vertexBufferMemory = nullptr;
		vk::DeviceSize vertexBufferSize = 0;
	};

	template<>
	struct Info<FvkType::IndexBuffer> final {
		vk::Device device = nullptr;
		vk::Buffer indexBuffer = nullptr;
		vk::DeviceMemory indexBufferMemory = nullptr;
		vk::DeviceSize indexBufferSize = 0;
	};

	template<>
	struct Info<FvkType::UniformBuffer> final {
		vk::Device device = nullptr;
		vk::Buffer uniformBuffer = nullptr;
		vk::DeviceMemory uniformBufferMemory = nullptr;
	};

	template<>
	struct Info<FvkType::DescriptorPool> final {
		vk::Device device = nullptr;
		vk::DescriptorPool descriptorPool = nullptr;
	};

	template<>
	struct Info<FvkType::DescriptorSet> final {
		vk::Device device = nullptr;
		vk::DescriptorPool descriptorPool = nullptr;
		vk::DescriptorSet descriptorSet = nullptr;
	};

	template<>
	struct Info<FvkType::CommandBuffer> final {
		vk::Device device = nullptr;
		vk::CommandPool commandPool = nullptr;
		std::vector<vk::CommandBuffer> commandBuffers = {};
	};

	template<>
	struct Info<FvkType::FrameBuffer> final {
		vk::Device device = nullptr;
		vk::Framebuffer frameBuffer = nullptr;
	};

	template<>
	struct Info<FvkType::Semaphore> final {
		vk::Device device = nullptr;
		vk::Semaphore semaphore = nullptr;
	};

	template<>
	struct Info<FvkType::Fence> final {
		vk::Device device = nullptr;
		vk::Fence fence = nullptr;
	};

	template<>
	struct Info<FvkType::Sampler> final {
		vk::Device device = nullptr;
		vk::Sampler sampler = nullptr;
	};

	template<>
	struct Info<FvkType::Texture> final {
		vk::Device device = nullptr;
		vk::Image image = nullptr;
		vk::DeviceMemory imageMemory = nullptr;
		vk::ImageView imageView = nullptr;
		std::string filePath = "";
	};

	template<>
	struct Info<FvkType::ImGui> final {
		vk::Device device = nullptr;
		vk::DescriptorPool descriptorPool = nullptr;
		std::vector<ImFont*> fonts{};
	};

	template<>
	struct Info<FvkType::ImGuiImage> final {
		vk::Device device = nullptr;
		vk::DescriptorSetLayout descriptorSetLayout = nullptr;
		vk::DescriptorPool descriptorPool = nullptr;
		VkDescriptorSet descriptorSet = nullptr;
		vk::Image image = nullptr;
	};

}

namespace FVK::Internal::Data {

	using WindowInfo = Info<FvkType::Window>;
	using InstanceInfo = Info<FvkType::Instance>;
	using MessengerInfo = Info<FvkType::Messenger>;
	using SurfaceInfo = Info<FvkType::Surface>;
	using PhysicalDeviceInfo = Info<FvkType::PhysicalDevice>;
	using LogicalDeviceInfo = Info<FvkType::LogicalDevice>;
	using QueueInfo = Info<FvkType::Queue>;
	using SwapchainInfo = Info<FvkType::Swapchain>;
	using RenderPassInfo = Info<FvkType::RenderPass>;
	using DescriptorSetLayoutInfo = Info<FvkType::DescriptorSetLayout>;
	using ShaderModuleInfo = Info<FvkType::ShaderModule>;
	using GraphicsPipelineLayoutInfo = Info<FvkType::GraphicsPipelineLayout>;
	using GraphicsPipelineInfo = Info<FvkType::GraphicsPipeline>;
	using ImageInfo = Info<FvkType::Image>;
	using ImageViewInfo = Info<FvkType::ImageView>;
	using DeviceMemoryInfo = Info<FvkType::DeviceMemory>;
	using CommandPoolInfo = Info<FvkType::CommandPool>;
	using VertexBufferInfo = Info<FvkType::VertexBuffer>;
	using IndexBufferInfo = Info<FvkType::IndexBuffer>;
	using UniformBufferInfo = Info<FvkType::UniformBuffer>;
	using DescriptorPoolInfo = Info<FvkType::DescriptorPool>;
	using DescriptorSetInfo = Info<FvkType::DescriptorSet>;
	using CommandBufferInfo = Info<FvkType::CommandBuffer>;
	using FrameBufferInfo = Info<FvkType::FrameBuffer>;
	using SemaphoreInfo = Info<FvkType::Semaphore>;
	using FenceInfo = Info<FvkType::Fence>;
	using SamplerInfo = Info<FvkType::Sampler>;
	using TextureInfo = Info<FvkType::Texture>;
	using ImGuiInfo = Info<FvkType::ImGui>;
	using ImGuiImageInfo = Info<FvkType::ImGuiImage>;

}
