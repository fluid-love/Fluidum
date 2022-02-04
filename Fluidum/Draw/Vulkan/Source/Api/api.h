/*
ThreadSafe

Key:Assign a string key to each item.
	Characters beginning with __ are reserved.

There is a possibility that a system_error exception will be thrown from the std::mutex.

The details of each function are described in the documentation.
*/

#pragma once

#include "using.h"
#include "assist.h"

#ifndef FluidumVK_API
#define FluidumVK_API
#else
#error Already Defined
#endif

//initialize terminate
namespace FVK {

	/*
	Exception:
		std::exception
		FailedToCreate
	*/
	//basic
	FluidumVK_API void initialize();

	FluidumVK_API [[nodiscard]] bool isFluidumVKInitialized() noexcept;

	/*
	Exception:
		NotInitialized
		FailedToDestroy
	*/
	//basic
	FluidumVK_API void terminate();

}

//Debug
namespace FVK {

	/*
	Exception:
		std::system_error
	*/
	//strong
	FluidumVK_API void setMessengerCallbackFunction(MessengerCallbackType callback);

}

//Window
namespace FVK {

	//create window
	//strong
	template<Internal::IsWindowParameter Parameter>
	FluidumVK_API void createWindow(const char* key, const Parameter& parameter) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		Api::createBase<FvkType::Window>(key, parameter);
	}

	//destroy window
	//strong
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyWindow(const WindowKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		Api::destroyBase<FvkType::Window>(key);
	}

	//Loop through the func argument until glfwWindowShouldClose == true.
	template<Internal::Key::IsKeyType T, typename... Args>
	FluidumVK_API void loopUntilWindowShouldClose(const WindowKey<T>& key, void(*func)(Args...), Args&&...args) {
		using namespace Internal;
		GLFWwindow* window;
		{
			LockGuard lock(GMutex);
			Api::checkManagerEmpty();
			const Data::WindowInfo& info = GManager->refInfo<FvkType::Window>(key);
			window = info.window;
		}
		while (!glfwWindowShouldClose(window)) {
			func(std::forward<Args>(args)...);
		}
	}

	/*
	Exception:
		NotInitialized
		NotFound
		Unexpected
	*/
	//strong

	//If the size of the FrameBuffer is changed, return the variable that is true and set it to false.
	template<Internal::Key::IsKeyType T>
	FluidumVK_API [[nodiscard]] bool isFrameBufferResized(const WindowKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();

		const Data::WindowInfo& info = GManager->refInfo<FvkType::Window>(key);
		bool result = info.framebufferResized;
		info.framebufferResized = false;
		return result;
	}

	template<Internal::Key::IsKeyType T>
	FluidumVK_API [[nodiscard]] std::pair<IF32, IF32> getWindowSize(const WindowKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		static_assert(Internal::Key::IsKey<decltype(key)>);
		const Window& item = GManager->refItem<FvkType::Window>(key);
		return item.windowSize();//no-throw
	}

	template<Internal::Key::IsKeyType T>
	FluidumVK_API [[nodiscard]] std::pair<IF32, IF32> getWindowPos(const WindowKey<T>& key) {
		using namespace Internal;
		int x{}, y{};
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const Data::WindowInfo& info = GManager->refInfo<FvkType::Window>(key);
		glfwGetWindowPos(info.window, &x, &y);
		return { static_cast<IF32>(x), static_cast<IF32>(y) };
	}

	FluidumVK_API [[nodiscard]] inline std::pair<IF32, IF32> getFulscreenWindowSize() {
		using namespace Internal;
		Api::checkManagerEmpty();
		return Internal::Window::fullscreenSize();
	}

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void setWindowSize(const WindowKey<T>& key, const IF32 width, const IF32 height) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const Data::WindowInfo& info = GManager->refInfo<FvkType::Window>(key);
		glfwSetWindowSize(info.window, static_cast<int>(width), static_cast<int>(height));
	}

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void setWindowPos(const WindowKey<T>& key, const IF32 x, const IF32 y) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const Data::WindowInfo& info = GManager->refInfo<FvkType::Window>(key);
		glfwSetWindowPos(info.window, static_cast<int>(x), static_cast<int>(y));
	}

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void setWindowSizeLimitsMin(const WindowKey<T>& key, const std::optional<IF32> width, const std::optional<IF32> height) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const Data::WindowInfo& info = GManager->refInfo<FvkType::Window>(key);
		assert(width.has_value() || height.has_value());
		if (width && height)
			glfwSetWindowSizeLimits_(info.window, static_cast<int>(*width), static_cast<int>(*height), GLFW_DONT_CARE, GLFW_DONT_CARE);
		else {//width || height
			if (width)
				glfwSetWindowSizeLimits_(info.window, static_cast<int>(*width), static_cast<int>(info.sizeMinlimits.second), GLFW_DONT_CARE, GLFW_DONT_CARE);
			else //height
				glfwSetWindowSizeLimits_(info.window, static_cast<int>(info.sizeMinlimits.first), static_cast<int>(*height), GLFW_DONT_CARE, GLFW_DONT_CARE);
		}
	}

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void setWindowSizeLimitsMax(const WindowKey<T>& key, const std::optional<IF32> width, const std::optional<IF32> height) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const Data::WindowInfo& info = GManager->refInfo<FvkType::Window>(key);
		assert(width.has_value() || height.has_value());
		if (width && height)
			glfwSetWindowSizeLimits_(info.window, GLFW_DONT_CARE, GLFW_DONT_CARE, static_cast<int>(*width), static_cast<int>(*height));
		else {//width || height
			if (width)
				glfwSetWindowSizeLimits_(info.window, GLFW_DONT_CARE, GLFW_DONT_CARE, static_cast<int>(*width), static_cast<int>(info.sizeMinLimits.second));
			else //height
				glfwSetWindowSizeLimits_(info.window, GLFW_DONT_CARE, GLFW_DONT_CARE, static_cast<int>(info.sizeMinLimits.first), static_cast<int>(*height));
		}
	}

	template<Internal::Key::IsKeyType T>
	FluidumVK_API [[nodiscard]] std::pair<IF32, IF32> getWindowSizeLimitsMin(const WindowKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const Data::WindowInfo& info = GManager->refInfo<FvkType::Window>(key);
		return { static_cast<IF32>(info.sizeMaxLimits.first), static_cast<IF32>(info.sizeMaxLimits.second) };
	}
	
	template<Internal::Key::IsKeyType T>
	FluidumVK_API [[nodiscard]] std::pair<IF32, IF32> getWindowSizeLimitsMax(const WindowKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const Data::WindowInfo& info = GManager->refInfo<FvkType::Window>(key);
		return { static_cast<IF32>(info.sizeMinLimits.first), static_cast<IF32>(info.sizeMinLimits.second) };
	}

	//minimize maximize
	/*
	Exception:
		NotInitialized
		NotFound
	*/
	//strong
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void minimizeWindow(const WindowKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const Data::WindowInfo& info = GManager->refInfo<FvkType::Window>(key);
		glfwIconifyWindow(info.window);
	}

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void maximizeWindow(const WindowKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const Data::WindowInfo& info = GManager->refInfo<FvkType::Window>(key);
		glfwMaximizeWindow(info.window);
	}

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void fullscreenWindow(const WindowKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const Window& item = GManager->refItem<FvkType::Window>(key);
		item.fullscreen();
	}

	template<Internal::Key::IsKeyType T>
	FluidumVK_API [[nodiscard]] bool isWindowMaximized(const WindowKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const Data::WindowInfo& info = GManager->refInfo<FvkType::Window>(key);
		return info.maximized;
	}

	template<Internal::Key::IsKeyType T>
	FluidumVK_API [[nodiscard]] bool isWindowFocused(const WindowKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const Data::WindowInfo& info = GManager->refInfo<FvkType::Window>(key);
		return info.focused;
	}

}

//Instance
namespace FVK {

	//strong
	FluidumVK_API void createInstance(const char* key, const InstanceParameter& parameter);
	FluidumVK_API void createInstance(const char* key, const InstanceMessengerParameter& parameter);

	//strong
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyInstance(const InstanceKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();

		//Messenger
		const StrKey instanceStrKey = Key::Converter::keyToStringKeyType(key);
		StrKey messengerStrKey = "__Messenger_";
		messengerStrKey += instanceStrKey;

		const Key::MessengerKey<const char*> messengerKey = { messengerStrKey.c_str() };
		const Data::InstanceInfo& info = GManager->refInfo<FvkType::Instance>(key);
		if (info.isMessengerCreated) {
			//strong
			Api::destroyBase<FvkType::Messenger>(messengerKey);
		}

		if (!GManager->isItemDestructible<FvkType::Instance>(key))
			std::terminate();

		try {
			//strong
			GManager->destroy<FvkType::Instance>(key);
		}
		catch (...) {
			//This will cause a memory leak. Therefore, terminate.
			GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Serious Error.");
			std::terminate();
		}
	}

}

//Surface
namespace FVK {

	FluidumVK_API void createSurface(const char* key, const SurfaceParameter& parameter);

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroySurface(const SurfaceKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		Api::destroyBase<FvkType::Surface>(key);
	}

}

//PhysicalDevice
namespace FVK {

	//Find a physical device that meets the requirements.
	FluidumVK_API void pickPhysicalDevice(const char* key, const PhysicalDeviceParameter& parameter);

	//Find physical devices that support Swapchain.
	FluidumVK_API void pickPhysicalDevice(const char* key, const PhysicalDeviceSwapchainParameter& parameter);

	template<Internal::Key::IsKeyType T>
	FluidumVK_API [[nodiscard]] vk::SampleCountFlagBits getMaxUsableSampleCount(const PhysicalDeviceKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const Data::PhysicalDeviceInfo& data = GManager->refInfo<FvkType::PhysicalDevice>(key);
		return data.usableMaxMsaaSamples;
	}

	template<Internal::Key::IsKeyType T>
	FluidumVK_API [[nodiscard]] std::vector<vk::SurfaceFormatKHR> getSupportedSurfaceFormats(const PhysicalDeviceKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const Data::PhysicalDeviceInfo& data = GManager->refInfo<FvkType::PhysicalDevice>(key);
		return data.formats;
	}

	template<Internal::Key::IsKeyType T>
	FluidumVK_API [[nodiscard]] bool isSurfaceSupportedFormat(const PhysicalDeviceKey<T>& key, const vk::Format format, const vk::ColorSpaceKHR colorSpace) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const Data::PhysicalDeviceInfo& data = GManager->refInfo<FvkType::PhysicalDevice>(key);
		return Swapchain::isSurfaceFormatSupport(format, colorSpace, data.formats);
	}

	template<Internal::Key::IsKeyType T>
	FluidumVK_API [[nodiscard]] vk::Format findSupportedFormat(const PhysicalDeviceKey<T>& key, const std::vector<vk::Format>& candidates, const vk::ImageTiling tiling, const vk::FormatFeatureFlags features) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const Data::PhysicalDeviceInfo& info = GManager->refInfo<FvkType::PhysicalDevice>(key);
		return PhysicalDevice::findSupportedFormat(info.physicalDevice, candidates, tiling, features);
	}

	template<Internal::Key::IsKeyType T>
	FluidumVK_API [[nodiscard]] bool isPresentModeSupport(const PhysicalDeviceKey<T>& key, const vk::PresentModeKHR presentMode) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const Data::PhysicalDeviceInfo& data = GManager->refInfo<FvkType::PhysicalDevice>(key);
		return Swapchain::isPresentModeSupport(presentMode, data.presentModes);
	}

	template<Internal::Key::IsKeyType T>
	FluidumVK_API [[nodiscard]] vk::SurfaceCapabilitiesKHR getSurfaceCapabilities(const PhysicalDeviceKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const Data::PhysicalDeviceInfo& data = GManager->refInfo<FvkType::PhysicalDevice>(key);
		return data.capabilities;
	}

	template<Internal::Key::IsKeyType T>
	FluidumVK_API [[nodiscard]] vk::PhysicalDeviceProperties getPhysicalDeviceProperties(const PhysicalDeviceKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const Data::PhysicalDeviceInfo& data = GManager->refInfo<FvkType::PhysicalDevice>(key);
		return data.physicalDevice.getProperties();
	}

	template<Internal::Key::IsKeyType T>
	FluidumVK_API [[nodiscard]] UI32 getGraphicsQueueFmily(const PhysicalDeviceKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const Data::PhysicalDeviceInfo& data = GManager->refInfo<FvkType::PhysicalDevice>(key);
		return data.graphicsFamily;
	}

	template<Internal::Key::IsKeyType T>
	FluidumVK_API [[nodiscard]] UI32 getPresentQueueFmily(const PhysicalDeviceKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const Data::PhysicalDeviceInfo& data = GManager->refInfo<FvkType::PhysicalDevice>(key);
		return data.presentFamily;
	}

	template<Internal::Key::IsKeyType T1>
	FluidumVK_API [[nodiscard]] UI32 findMemoryType(
		const PhysicalDeviceKey<T1>& key,
		const UI32 typeFilter,
		const vk::MemoryPropertyFlags properties
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const Data::PhysicalDeviceInfo& info = GManager->refInfo<FvkType::PhysicalDevice>(key);
		return PhysicalDevice::findMemoryType(info.physicalDevice, typeFilter, properties);
	}

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyPhysicalDevice(const PhysicalDeviceKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		Api::destroyBase<FvkType::PhysicalDevice>(key);
	}

}

//LogicalDevice
namespace FVK {

	//Enables all the extensions requested by PhysicalDevice.
	FluidumVK_API void createLogicalDevice(const char* key, const LogicalDeviceParameter& parameter);

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyLogicalDevice(const LogicalDeviceKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		Api::destroyBase<FvkType::LogicalDevice>(key);
	}

}

//Queue
namespace FVK {

	FluidumVK_API void getQueue(const char* key, const QueueParameter& parameter);

}

//Swapchain
namespace FVK {

	FluidumVK_API void createSwapchain(const char* key, SwapchainParameter& parameter);

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroySwapchain(const SwapchainKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		Api::destroyBase<FvkType::Swapchain>(key);
	}

	template<Internal::Key::IsKeyType T1, Internal::Key::IsKeyType T2>
	FluidumVK_API [[nodiscard]] vk::Extent2D getCorrectSwapchainExtent(
		const WindowKey<T1>& window,
		const PhysicalDeviceKey<T2>& physicalDevice
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const Data::PhysicalDeviceInfo& physicalDeviceInfo = GManager->refInfo<FvkType::PhysicalDevice>(physicalDevice);
		const Data::WindowInfo& windowInfo = GManager->refInfo<FvkType::Window>(window);
		return Swapchain::getCorrectSwapchainExtent(windowInfo.window, physicalDeviceInfo.capabilities);
	}

	template<Internal::Key::IsKeyType T1>
	FluidumVK_API [[nodiscard]] vk::Format getSwapchainImageFormat(
		const SwapchainKey<T1>& swapchain
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const Data::SwapchainInfo& info = GManager->refInfo<FvkType::Swapchain>(swapchain);
		return info.format;
	}

	template<Internal::Key::IsKeyType T1>
	FluidumVK_API [[nodiscard]] vk::Extent2D getSwapchainExtent(
		const SwapchainKey<T1>& swapchain
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const Data::SwapchainInfo& info = GManager->refInfo<FvkType::Swapchain>(swapchain);
		return info.extent;
	}

	template<Internal::Key::IsKeyType T1>
	FluidumVK_API [[nodiscard]] UI32 getSwapchainMinImageCount(
		const SwapchainKey<T1>& swapchain
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const Data::SwapchainInfo& info = GManager->refInfo<FvkType::Swapchain>(swapchain);
		return info.minImageCount;
	}

}

//RenderPass
namespace FVK {

	FluidumVK_API void createRenderPass(const char* key, const RenderPassParameter& parameter);

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyRenderPass(const RenderPassKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		Api::destroyBase<FvkType::RenderPass>(key);
	}

}

//DescriptorSetLayout
namespace FVK {

	FluidumVK_API void createDescriptorSetLayout(const char* key, const DescriptorSetLayoutParameter& parameter);

	FluidumVK_API void updateDescriptorSet(UpdateDescriptorSetParameter& parameter);

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyDescriptorSetLayout(const DescriptorSetLayoutKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		Api::destroyBase<FvkType::DescriptorSetLayout>(key);
	}

}

//ShaderModule
namespace FVK {

	FluidumVK_API void createShaderModule(const char* key, const ShaderModuleParameter& parameter);

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyShaderModule(const ShaderModuleKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		Api::destroyBase<FvkType::ShaderModule>(key);
	}

	FluidumVK_API [[nodiscard]] std::vector<char> readShaderFile(const char* filePath);

}

//GraphicsPipelineLayout
namespace FVK {

	FluidumVK_API void createGraphicsPipelineLayout(const char* key, GraphicsPipelineLayoutParameter& parameter);

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyGraphicsPipelineLayout(const GraphicsPipelineLayoutKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		Api::destroyBase<FvkType::GraphicsPipelineLayout>(key);
	}

}


//GraphicsPipeline
namespace FVK {

	FluidumVK_API void createGraphicsPipeline(const char* key, GraphicsPipelineParameter& parameter);

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyGraphicsPipeline(const GraphicsPipelineKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		Api::destroyBase<FvkType::GraphicsPipeline>(key);
	}

}

//Image
namespace FVK {

	FluidumVK_API void createImage(const char* key, const ImageParameter& parameter);

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyImage(const ImageKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		Api::destroyBase<FvkType::Image>(key);
	}

	template<Internal::Key::IsKeyType T1, Internal::Key::IsKeyType T2, Internal::Key::IsKeyType T3>
	FluidumVK_API [[nodiscard]] void bindImageMemory(
		const LogicalDeviceKey<T3>& logicalDeviceKey,
		const ImageKey<T1>& imageKey,
		const DeviceMemoryKey<T2>& deviceMemoryKey,
		const vk::DeviceSize offset = 0
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();

		const Data::LogicalDeviceInfo& logicalDeviceInfo = GManager->refInfo<FvkType::LogicalDevice>(logicalDeviceKey);
		const Data::ImageInfo& imageInfo = GManager->refInfo<FvkType::Image>(imageKey);
		const Data::DeviceMemoryInfo& deviceMemoryInfo = GManager->refInfo<FvkType::DeviceMemory>(deviceMemoryKey);

		auto result = logicalDeviceInfo.device.bindImageMemory(imageInfo.image, deviceMemoryInfo.deviceMemory, offset);

		if (result != vk::Result::eSuccess)
			::FVK::Internal::Exception::throwFailedToCreate();
	}

}

//ImageView
namespace FVK {

	FluidumVK_API void createImageView(const char* key, ImageViewParameter& parameter);
	FluidumVK_API void createImageView(const char* key, const SwapchainImageViewParameter& parameter);

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyImageView(const ImageViewKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		Api::destroyBase<FvkType::ImageView>(key);
	}

}

//DeviceMemory
namespace FVK {

	FluidumVK_API void allocateDeviceMemory(const char* key, const DeviceMemoryParameter& parameter);

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void freeDeviceMemory(const DeviceMemoryKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		Api::destroyBase<FvkType::DeviceMemory>(key);
	}

	template<Internal::Key::IsKeyType T1, Internal::Key::IsKeyType T2>
	FluidumVK_API [[nodiscard]] vk::MemoryRequirements getImageMemoryRequirements(
		const LogicalDeviceKey<T1>& logicalDeviceKey,
		const ImageKey<T2>& imageKey
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();

		const Data::LogicalDeviceInfo& logicalDeviceInfo = GManager->refInfo<FvkType::LogicalDevice>(logicalDeviceKey);
		const Data::ImageInfo& imageInfo = GManager->refInfo<FvkType::Image>(imageKey);

		return logicalDeviceInfo.device.getImageMemoryRequirements(imageInfo.image);
	}

}

//CommandPool
namespace FVK {

	FluidumVK_API void createCommandPool(const char* key, const CommandPoolParameter& parameter);

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyCommandPool(const CommandPoolKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		Api::destroyBase<FvkType::CommandPool>(key);
	}

}

//VertexBuffer
namespace FVK {

	FluidumVK_API void createVertexBuffer(const char* key, const VertexBufferParameter& parameter);

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyVertexBuffer(const VertexBufferKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		Api::destroyBase<FvkType::VertexBuffer>(key);
	}

}

//IndexBuffer
namespace FVK {

	FluidumVK_API void createIndexBuffer(const char* key, const IndexBufferParameter& parameter);

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyIndexBuffer(const IndexBufferKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		Api::destroyBase<FvkType::IndexBuffer>(key);
	}

}

//UniformBuffer
namespace FVK {

	FluidumVK_API void createUniformBuffer(const char* key, const UniformBufferParameter& parameter);

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyUniformBuffer(const UniformBufferKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		Api::destroyBase<FvkType::UniformBuffer>(key);
	}

}

//DescriptorPool
namespace FVK {

	FluidumVK_API void createDescriptorPool(const char* key, const DescriptorPoolParameter& parameter);

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyDescriptorPool(const DescriptorPoolKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		Api::destroyBase<FvkType::DescriptorPool>(key);
	}

}

//DescriptorSet
namespace FVK {

	FluidumVK_API void allocateDescriptorSet(const char* key, DescriptorSetParameter& parameter);

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void freeDescriptorSet(const DescriptorSetKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		Api::destroyBase<FvkType::DescriptorSet>(key);
	}

}

//CommandBuffer
namespace FVK {

	FluidumVK_API void allocateCommandBuffers(const char* key, CommandBufferParameter& parameter);

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void freeCommandBuffer(const CommandBufferKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		Api::destroyBase<FvkType::CommandBuffer>(key);
	}

}

//FrameBuffer
namespace FVK {

	FluidumVK_API void createFrameBuffer(const char* key, FrameBufferParameter& parameter);

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyFrameBuffer(const FrameBufferKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		Api::destroyBase<FvkType::FrameBuffer>(key);
	}

}

//Semaphore
namespace FVK {

	FluidumVK_API void createSemaphore(const char* key, const SemaphoreParameter& parameter);

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroySemaphore(const SemaphoreKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		Api::destroyBase<FvkType::Semaphore>(key);
	}

}

//Fence
namespace FVK {

	FluidumVK_API void createFence(const char* key, const FenceParameter& parameter);

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyFence(const FenceKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		Api::destroyBase<FvkType::Fence>(key);
	}

}

//Sampler
namespace FVK {

	FluidumVK_API void createSampler(const char* key, const SamplerParameter& parameter);

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroySampler(const SamplerKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		Api::destroyBase<FvkType::Sampler>(key);
	}

}

//Texture
namespace FVK {

	FluidumVK_API void createTexture(const char* key, const TextureParameter& parameter);

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyTexture(const TextureKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		Api::destroyBase<FvkType::Texture>(key);
	}

}

//ImGui
namespace FVK {

	FluidumVK_API void createImGui(const char* key, const ImGuiParameter& parameter);

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyImGui(const ImGuiKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		Api::destroyBase<FvkType::ImGui>(key);
	}

	FluidumVK_API [[nodiscard]] ImTextureID createImGuiImage(const char* key, const ImGuiImageParameter& parameter);

	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyImGuiImage(const ImGuiImageKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		Api::destroyBase<FvkType::ImGuiImage>(key);
	}

	template<Internal::Key::IsKeyType T>
	FluidumVK_API std::vector<ImFont*> getImGuiFonts(const ImGuiKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const Data::ImGuiInfo& info = GManager->refInfo<FvkType::ImGui>(key);
		return info.fonts;
	}

}

//Command
namespace FVK {

	template<Internal::Key::IsKeyType T1>
	FluidumVK_API [[nodiscard]] BeginCommandBufferCommand makeBeginCommandBufferCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const UI32 commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const auto connections = Key::BeginCommandBufferConnectionKeys(commandBuffer);
		return GManager->makeCommand<CommandType::BeginCommandBuffer>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType T1>
	FluidumVK_API [[nodiscard]] EndCommandBufferCommand makeEndCommandBufferCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const UI32 commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const auto connections = Key::EndCommandBufferConnectionKeys(commandBuffer);
		return GManager->makeCommand<CommandType::EndCommandBuffer>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType T1, Internal::Key::IsKeyType T2>
	FluidumVK_API [[nodiscard]] BindVertexBuffersCommand makeBindVertexBuffersCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const VertexBufferKey<T2>& vertexBuffer,
		const UI32 commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const auto connections = Key::BindVertexbuffersConnectionKeys(commandBuffer, vertexBuffer);
		return GManager->makeCommand<CommandType::BindVertexBuffers>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType  T1, Internal::Key::IsKeyType  T2, Internal::Key::IsKeyType  T3>
	FluidumVK_API [[nodiscard]] BeginRenderPassCommand makeBeginRenderPassCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const RenderPassKey<T2>& renderPass,
		const FrameBufferKey<T3>& frameBuffer,
		const UI32 commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const auto connections = Key::BeginRenderPassConnectionKeys(commandBuffer, renderPass, frameBuffer);
		return GManager->makeCommand<CommandType::BeginRenderPass>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType  T1, Internal::Key::IsKeyType  T2, Internal::Key::IsKeyType  T3>
	FluidumVK_API [[nodiscard]] BindDescriptorSetCommand makeBindDescriptorSetCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const GraphicsPipelineLayoutKey<T2>& graphicsPipelineLayout,
		const DescriptorSetKey<T3>& descriptorSet,
		const UI32 commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const auto connections = Key::BindDescriptorSetConnectionKeys(commandBuffer, graphicsPipelineLayout, descriptorSet);
		return GManager->makeCommand<CommandType::BindDescriptorSet>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType  T1, Internal::Key::IsKeyType  T2>
	FluidumVK_API [[nodiscard]] BindGraphicsPipelineCommand makeBindGraphicsPipelineCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const GraphicsPipelineKey<T2>& graphicsPipeline,
		const UI32 commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const auto connections = Key::BindGraphicsPipelineConnectionKeys(commandBuffer, graphicsPipeline);
		return GManager->makeCommand<CommandType::BindGraphicsPipeline>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType  T1, Internal::Key::IsKeyType  T2>
	FluidumVK_API [[nodiscard]] BindIndexBufferCommand makeBindIndexBufferCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const IndexBufferKey<T2>& indexBuffer,
		const UI32 commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const auto connections = Key::BindIndexBufferConnectionKeys(commandBuffer, indexBuffer);
		return GManager->makeCommand<CommandType::BindIndexBuffer>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType  T1>
	FluidumVK_API [[nodiscard]] DrawIndexedCommand makeDrawIndexedCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const UI32 commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const auto connections = Key::DrawIndexedConnectionKeys(commandBuffer);
		return GManager->makeCommand<CommandType::DrawIndexed>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType  T1>
	FluidumVK_API [[nodiscard]] EndRenderPassCommand makeEndRenderPassCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const UI32 commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const auto connections = Key::EndRenderPassConnectionKeys(commandBuffer);
		return GManager->makeCommand<CommandType::EndRenderPass>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType  T1>
	FluidumVK_API [[nodiscard]] ImGuiRenderDrawDataCommand makeImGuiRenderDrawDataCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const UI32 commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const auto connections = Key::ImGuiRenderDrawDataConnectionKeys(commandBuffer);
		return GManager->makeCommand<CommandType::ImGuiRenderDrawData>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType T1, Internal::Key::IsKeyType T2>
	FluidumVK_API [[nodiscard]] BindImGuiImageCommand makeBindImGuiImageCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const ImGuiImageKey<T2>& imguiImage,
		const UI32 commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const auto connections = Key::BindImGuiImageConnectionKeys(commandBuffer, imguiImage);
		return GManager->makeCommand<CommandType::BindImGuiImage>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType  T1>
	FluidumVK_API [[nodiscard]] SetScissorCommand makeSetScissorCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const UI32 commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const auto connections = Key::SetScissorConnectionKeys(commandBuffer);
		return GManager->makeCommand<CommandType::SetScissor>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType  T1>
	FluidumVK_API [[nodiscard]] SetViewportCommand makeSetViewportCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const UI32 commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const auto connections = Key::SetViewportConnectionKeys(commandBuffer);
		return GManager->makeCommand<CommandType::SetViewport>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType  T1, Internal::Key::IsKeyType  T2>
	FluidumVK_API [[nodiscard]] UpdateVertexBufferCommand makeUpdateVertexBufferCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const VertexBufferKey<T2>& vertexBuffer,
		const UI32 commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const auto connections = Key::UpdateVertexBufferConnectionKeys(commandBuffer, vertexBuffer);
		return GManager->makeCommand<CommandType::UpdateVertexBuffer>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType  T1, Internal::Key::IsKeyType  T2>
	FluidumVK_API [[nodiscard]] UpdateIndexBufferCommand makeUpdateIndexBufferCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const IndexBufferKey<T2>& indexBuffer,
		const UI32 commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const auto connections = Key::UpdateIndexBufferConnectionKeys(commandBuffer, indexBuffer);
		return GManager->makeCommand<CommandType::UpdateIndexBuffer>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType  T1, Internal::Key::IsKeyType  T2>
	FluidumVK_API [[nodiscard]] TransitionImageLayoutCommand makeTransitionImageLayout(
		const CommandBufferKey<T1>& commandBuffer,
		const ImageKey<T2>& image,
		const UI32 commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const auto connections = Key::TransitionImageLayoutConnectionKeys(commandBuffer, image);
		return GManager->makeCommand<CommandType::TransitionImageLayout>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType  T1, Internal::Key::IsKeyType  T2>
	FluidumVK_API [[nodiscard]] QueueSubmitCommand makeQueueSubmitCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const QueueKey<T2>& queue,
		const UI32 commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		const auto connections = Key::QueueSubmitConnectionKeys(commandBuffer, queue);
		return GManager->makeCommand<CommandType::QueueSubmit>(connections, commandBufferIndex);
	}

	FluidumVK_API [[nodiscard]] NextCommand makeNextCommand();

	template<Internal::Key::IsKey ...T>
	FluidumVK_API [[nodiscard]] auto makeAnyCommand(const T&... keys) {
		using namespace Internal;
		LockGuard lock(GMutex);
		Api::checkManagerEmpty();
		std::array<std::vector<IndexKey>, sizeof...(T)> indices
		{ Key::Converter::toIndexKeyVector(keys)... };

		return GManager->makeAnyCommand<T...>(indices);
	}

	void  exeCommands(Commands& commands);

}