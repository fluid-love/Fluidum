#include "initialize.h"

void window(const char* title) {
	FVK::WindowFullScreenParameter param{};
	param.title = title;
	FVK::createWindow(FDR::Internal::BaseWindowKey, param);
}

void instance() {
	FVK::ApplicationInfo appInfo = {
		.applicationName = "Fluidum",
		.appVersion = {1,0,0},
		.vulkanApiVersion = FVK::VulkanApiVersion::version_1_2
	};
	FVK::InstanceMessengerParameter param = {
		.messengerParameter = {},
		.appInfo = appInfo
	};

	FVK::createInstance(FDR::Internal::BaseInstanceKey, param);
}

void surface() {
	using namespace FDR::Internal;
	FVK::SurfaceParameter param;
	param.windowKey = BaseWindowKey;
	param.instanceKey = BaseInstanceKey;
	FVK::createSurface(BaseSurfaceKey, param);
}

void physicalDevice() {
	using namespace FDR::Internal;

	FVK::PhysicalDeviceSwapchainParameter param;
	param.surfaceKey = BaseSurfaceKey;
	param.queue = vk::QueueFlagBits::eGraphics;
	param.extensions = { FVK::PhysicalDeviceExtension::KHR_swapchain };
	param.features = { FVK::PhysicalDeviceFeature::SamplerAnisotropy };
	FVK::pickPhysicalDevice(BasePhysicalDeviceKey, param);
}

void logicalDevice() {
	using namespace FDR::Internal;

	FVK::LogicalDeviceParameter param;
	param.physicalDeviceKey = BasePhysicalDeviceKey;
	FVK::createLogicalDevice(BaseDeviceKey, param);
}

void swapchain() {
	using namespace FDR::Internal;

	vk::SurfaceFormatKHR surfaceFormat = {
		vk::Format::eR8G8B8A8Srgb,
		vk::ColorSpaceKHR::eVkColorspaceSrgbNonlinear
	};
	if (!FVK::isSurfaceSupportedFormat(BasePhysicalDeviceKey, surfaceFormat.format, surfaceFormat.colorSpace)) {
		surfaceFormat = FVK::getSupportedSurfaceFormats(BasePhysicalDeviceKey).at(0);
	}

	vk::PresentModeKHR presentMode = vk::PresentModeKHR::eMailbox;
	if (!FVK::isPresentModeSupport(BasePhysicalDeviceKey, presentMode)) {
		presentMode = vk::PresentModeKHR::eFifo;
	}

	const vk::Extent2D extent = FVK::getCorrectSwapchainExtent(BaseWindowKey, BasePhysicalDeviceKey);

	const auto capabilities = FVK::getSurfaceCapabilities(BasePhysicalDeviceKey);
	uint32_t imageCount = capabilities.minImageCount + 1;
	if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
		imageCount = capabilities.maxImageCount;
	}

	FVK::SwapchainCreateInfo createInfo;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc;

	uint32_t queueFamilyIndices[] = { FVK::getGraphicsQueueFmily(BasePhysicalDeviceKey),FVK::getPresentQueueFmily(BasePhysicalDeviceKey) };

	if (queueFamilyIndices[0] != queueFamilyIndices[1]) {
		createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = vk::SharingMode::eExclusive;
	}
	createInfo.preTransform = capabilities.currentTransform;
	createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	{
		FVK::SwapchainParameter param;
		param.pInfo = &createInfo;
		param.logicalDeviceKey = BaseDeviceKey;
		FVK::createSwapchain(BaseSwapchainKey, param);
	}



	{
		auto size = FVK::getSwapchainMinImageCount(BaseSwapchainKey);

		for (uint16_t i = 0; i < size; i++) {
			BaseSwapchainImageViewKeys.emplace_back(std::string("_BSCIV") + std::to_string(i));

			FVK::SwapchainImageViewParameter param;
			param.imageIndex = i;
			param.logicalDeviceKey = BaseDeviceKey;
			param.swapchainKey = BaseSwapchainKey;

			FVK::createImageView(BaseSwapchainImageViewKeys[i], param);
		}
	}
}

vk::Format  findDepthFormat() {
	using namespace FDR::Internal;

	return FVK::findSupportedFormat(
		BasePhysicalDeviceKey,
		{
		vk::Format::eD32Sfloat,
		vk::Format::eD32SfloatS8Uint,
		vk::Format::eD24UnormS8Uint
		}
		,
		vk::ImageTiling::eOptimal,
		vk::FormatFeatureFlagBits::eDepthStencilAttachment
	);
}

void renderpass() {
	using namespace FDR::Internal;

	const auto msaaSamples = FVK::getMaxUsableSampleCount(BasePhysicalDeviceKey);

	vk::AttachmentDescription colorAttachment = {
		.format = FVK::getSwapchainImageFormat(BaseSwapchainKey),
		.samples = msaaSamples,
		.loadOp = vk::AttachmentLoadOp::eClear,
		.storeOp = vk::AttachmentStoreOp::eStore,
		.stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
		.stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
		.initialLayout = vk::ImageLayout::eUndefined,
		.finalLayout = vk::ImageLayout::eColorAttachmentOptimal,
	};

	vk::AttachmentDescription depthAttachment = {
		.format = findDepthFormat(),
		.samples = msaaSamples,
		.loadOp = vk::AttachmentLoadOp::eClear,
		.storeOp = vk::AttachmentStoreOp::eDontCare,
		.stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
		.stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
		.initialLayout = vk::ImageLayout::eUndefined,
		.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal,
	};

	vk::AttachmentDescription colorAttachmentResolve = {
		.format = FVK::getSwapchainImageFormat(BaseSwapchainKey),
		.samples = vk::SampleCountFlagBits::e1,
		.loadOp = vk::AttachmentLoadOp::eDontCare,
		.storeOp = vk::AttachmentStoreOp::eStore,
		.stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
		.stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
		.initialLayout = vk::ImageLayout::eUndefined,
		.finalLayout = vk::ImageLayout::ePresentSrcKHR
	};

	vk::AttachmentReference colorAttachmentRef = {
		.attachment = 0,
		.layout = vk::ImageLayout::eColorAttachmentOptimal
	};

	vk::AttachmentReference depthAttachmentRef = {
		.attachment = 1,
		.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal
	};

	vk::AttachmentReference colorAttachmentResolveRef = {
		.attachment = 2,
		.layout = vk::ImageLayout::eColorAttachmentOptimal,
	};

	vk::SubpassDescription subpass = {
		.pipelineBindPoint = vk::PipelineBindPoint::eGraphics,
		.colorAttachmentCount = 1,
		.pColorAttachments = &colorAttachmentRef,
		.pResolveAttachments = &colorAttachmentResolveRef,
		.pDepthStencilAttachment = &depthAttachmentRef,
	};

	vk::SubpassDependency dependency = {
		.srcSubpass = VK_SUBPASS_EXTERNAL,
		.dstSubpass = 0,
		.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
		.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
		.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite
	};

	std::array<vk::AttachmentDescription, 3> attachments = { colorAttachment, depthAttachment, colorAttachmentResolve };

	FVK::RenderPassCreateInfo renderPassInfo;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	{
		FVK::RenderPassParameter param;
		param.pInfo = &renderPassInfo;
		param.logicalDeviceKey = BaseDeviceKey;
		FVK::createRenderPass(BaseRenderPassKey, param);
	}

}

void descriptorSetLayout() {
	using namespace FDR::Internal;

	vk::DescriptorSetLayoutBinding uboLayoutBinding = {
		.binding = 0,
		.descriptorType = vk::DescriptorType::eUniformBufferDynamic,
		.descriptorCount = 1,
		.stageFlags = vk::ShaderStageFlagBits::eVertex,
		.pImmutableSamplers = nullptr,
	};

	std::array<vk::DescriptorSetLayoutBinding, 1> bindings = { uboLayoutBinding/*, samplerLayoutBinding */ };

	FVK::DescriptorSetLayoutCreateInfo createInfo;
	createInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	createInfo.pBindings = bindings.data();

	{
		FVK::DescriptorSetLayoutParameter param;
		param.pInfo = &createInfo;
		param.logicalDeviceKey = BaseDeviceKey;
		FVK::createDescriptorSetLayout(BaseDescriptorSetLayoutKey, param);
	}

}

void shaderModule() {
	using namespace FDR::Internal;

	{
		std::vector<char> code = FVK::readShaderFile(VertShaderFilePath);
		FVK::ShaderModuleCreateInfo createInfo;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
		FVK::ShaderModuleParameter param;
		param.pInfo = &createInfo;
		param.logicalDeviceKey = BaseDeviceKey;
		FVK::createShaderModule(BaseVertShaderModuleKey, param);
	}
	{
		std::vector<char> code = FVK::readShaderFile(FragShaderFilePath);
		FVK::ShaderModuleCreateInfo createInfo;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
		FVK::ShaderModuleParameter param;
		param.pInfo = &createInfo;
		param.logicalDeviceKey = BaseDeviceKey;
		FVK::createShaderModule(BaseFragShaderModuleKey, param);
	}
}

void graphicsPipelineLayout() {
	using namespace FDR::Internal;

	vk::PipelineLayoutCreateInfo pipelineLayoutInfo = {};

	FVK::GraphicsPipelineLayoutCreateInfo createInfo;
	createInfo.setLayoutCount = 1;
	createInfo.descriptorSetLayoutKeys = FVK::DescriptorSetLayoutKeys<const char*>{ BaseDescriptorSetLayoutKey };

	FVK::GraphicsPipelineLayoutParameter param;
	param.pInfo = &createInfo;
	param.logicalDevice = BaseDeviceKey;

	FVK::createGraphicsPipelineLayout(BaseGraphicsPipelineLayoutKey, param);
}

void graphicsPipeline() {
	using namespace FDR::Internal;

	FVK::PipelineShaderStageCreateInfo vertShaderStageInfo;
	vertShaderStageInfo.shaderModuleKey = BaseVertShaderModuleKey;
	vertShaderStageInfo.stage = vk::ShaderStageFlagBits::eVertex;
	vertShaderStageInfo.pName = "main";

	FVK::PipelineShaderStageCreateInfo fragShaderStageInfo;
	fragShaderStageInfo.shaderModuleKey = BaseFragShaderModuleKey;
	fragShaderStageInfo.stage = vk::ShaderStageFlagBits::eFragment;
	fragShaderStageInfo.pName = "main";

	FVK::PipelineShaderStageCreateInfo shaderStages[2] = { vertShaderStageInfo, fragShaderStageInfo };


	std::vector<vk::VertexInputAttributeDescription> baseAttributeDescriptions =
	{
	{
		{
		.location = 0,
		.binding = 0,
		.format = vk::Format::eR32G32B32Sfloat,
		.offset = offsetof(Vertex, pos),
		}
		,
		{
		.location = 1,
		.binding = 0,
		.format = vk::Format::eR32G32B32A32Sfloat,
		.offset = offsetof(Vertex, color)
		}
	}
	};

	vk::VertexInputBindingDescription baseBindingDescripton = {
		.binding = 0,
		.stride = sizeof(Vertex),
		.inputRate = vk::VertexInputRate::eVertex
	};


	vk::PipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &baseBindingDescripton;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(baseAttributeDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions = baseAttributeDescriptions.data();


	vk::PipelineInputAssemblyStateCreateInfo inputAssembly = {
		.topology = vk::PrimitiveTopology::eLineList,
		.primitiveRestartEnable = VK_FALSE
	};

	const auto extent = FVK::getSwapchainExtent(BaseSwapchainKey);
	vk::Viewport viewport = {
		.x = 0.0f,
		.y = 0.0f,
		.width = static_cast<float>(extent.width),
		.height = static_cast<float>(extent.height),
		.minDepth = 0.0f,
		.maxDepth = 1.0f,
	};

	vk::Rect2D scissor = {
		.offset = { 0, 0 },
		.extent = extent
	};

	vk::PipelineViewportStateCreateInfo viewportState = {
		.viewportCount = 1,
		.pViewports = &viewport,
		.scissorCount = 1,
		.pScissors = &scissor
	};

	vk::PipelineRasterizationStateCreateInfo rasterizer = {
		.depthClampEnable = VK_FALSE,
		.rasterizerDiscardEnable = VK_FALSE,
		.polygonMode = vk::PolygonMode::eFill,
		.cullMode = vk::CullModeFlagBits::eBack,
		.frontFace = vk::FrontFace::eClockwise,
		.depthBiasEnable = VK_FALSE,
		.lineWidth = 1.0f
	};

	vk::PipelineMultisampleStateCreateInfo multisampling = {
		.rasterizationSamples = FVK::getMaxUsableSampleCount(BasePhysicalDeviceKey),
		.sampleShadingEnable = VK_FALSE,
	};

	vk::PipelineDepthStencilStateCreateInfo depthStencil = {
		.depthTestEnable = VK_TRUE,
		.depthWriteEnable = VK_TRUE,
		.depthCompareOp = vk::CompareOp::eLess,
		.depthBoundsTestEnable = VK_FALSE,
		.stencilTestEnable = VK_FALSE,
	};

	vk::PipelineColorBlendAttachmentState colorBlendAttachment = {
		.blendEnable = VK_TRUE,
		.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha,
		.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha,
		.colorBlendOp = vk::BlendOp::eAdd,
		.srcAlphaBlendFactor = vk::BlendFactor::eOne,
		.dstAlphaBlendFactor = vk::BlendFactor::eZero,
		.alphaBlendOp = vk::BlendOp::eAdd,
		.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA,
	};

	vk::PipelineColorBlendStateCreateInfo colorBlending = {
		.logicOpEnable = VK_FALSE,
		.logicOp = vk::LogicOp::eCopy,
		.attachmentCount = 1,
		.pAttachments = &colorBlendAttachment,
		.blendConstants = vk::ArrayWrapper1D<float,4>({0.0f,0.0f,0.0f,0.0f}),
	};

	std::vector<vk::DynamicState> dynamics = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };
	vk::PipelineDynamicStateCreateInfo dynamicsStates = {
		.dynamicStateCount = static_cast<uint32_t>(dynamics.size()),
		.pDynamicStates = dynamics.data(),
	};
	FVK::GraphicsPipelineCreateInfo pipelineInfo;
	pipelineInfo.flags = vk::PipelineCreateFlagBits::eAllowDerivatives;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStageCreateInfos = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicsStates;
	pipelineInfo.graphicsPipelineLayoutKey = BaseGraphicsPipelineLayoutKey;
	pipelineInfo.renderPassKey = BaseRenderPassKey;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = nullptr;
	pipelineInfo.basePipelineIndex = -1;

	FVK::GraphicsPipelineParameter param;
	param.pInfo = &pipelineInfo;
	param.logicalDeviceKey = BaseDeviceKey;
	FVK::createGraphicsPipeline(BaseGraphicsPipelineKey, param);
}

void colorImage() {
	using namespace FDR::Internal;

	auto extent = FVK::getSwapchainExtent(BaseSwapchainKey);
	auto format = FVK::getSwapchainImageFormat(BaseSwapchainKey);

	vk::ImageCreateInfo imageInfo = {};
	imageInfo.imageType = vk::ImageType::e2D;
	imageInfo.extent.width = extent.width;
	imageInfo.extent.height = extent.height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = vk::ImageTiling::eOptimal;
	imageInfo.initialLayout = vk::ImageLayout::eUndefined;
	imageInfo.usage = vk::ImageUsageFlagBits::eTransientAttachment | vk::ImageUsageFlagBits::eColorAttachment;
	imageInfo.samples = FVK::getMaxUsableSampleCount(BasePhysicalDeviceKey);
	imageInfo.sharingMode = vk::SharingMode::eExclusive;

	FVK::ImageParameter param;
	param.logicalDeviceKey = BaseDeviceKey;
	param.pInfo = &imageInfo;

	FVK::createImage(BaseColorImageKey, param);

	vk::MemoryRequirements memRequirements = FVK::getImageMemoryRequirements(BaseDeviceKey, BaseColorImageKey);

	FVK::DeviceMemoryAllocateInfo allocInfo = {
		.allocationSize = memRequirements.size,
		.memoryTypeIndex = FVK::findMemoryType(BasePhysicalDeviceKey,memRequirements.memoryTypeBits,vk::MemoryPropertyFlagBits::eDeviceLocal)
	};

	FVK::DeviceMemoryParameter param2;
	param2.pInfo = &allocInfo;
	param2.logicalDeviceKey = BaseDeviceKey;

	FVK::allocateDeviceMemory(BaseColorImageMemoryKey, param2);
	FVK::bindImageMemory(BaseDeviceKey, BaseColorImageKey, BaseColorImageMemoryKey);

}

void colorImageView() {
	using namespace FDR::Internal;

	auto format = FVK::getSwapchainImageFormat(BaseSwapchainKey);

	vk::ImageSubresourceRange range = {
		range.aspectMask = vk::ImageAspectFlagBits::eColor,
		range.baseMipLevel = 0,
		range.levelCount = 1,
		range.baseArrayLayer = 0,
		range.layerCount = 1,
	};

	FVK::ImageViewCreateInfo info;
	info.imageKey = BaseColorImageKey;
	info.viewType = vk::ImageViewType::e2D;
	info.format = format;
	info.subresourceRange = range;

	FVK::ImageViewParameter param;
	param.logicalDeviceKey = BaseDeviceKey;
	param.pInfo = &info;
	FVK::createImageView(BaseColorImageViewKey, param);

}

void depthImage() {
	using namespace FDR::Internal;

	const auto extent = FVK::getSwapchainExtent(BaseSwapchainKey);

	const auto format = FVK::findSupportedFormat(BasePhysicalDeviceKey,
		{ vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint },
		vk::ImageTiling::eOptimal,
		vk::FormatFeatureFlagBits::eDepthStencilAttachment
	);
	vk::ImageCreateInfo imageInfo = {};
	imageInfo.imageType = vk::ImageType::e2D;
	imageInfo.extent.width = extent.width;
	imageInfo.extent.height = extent.height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = vk::ImageTiling::eOptimal;
	imageInfo.initialLayout = vk::ImageLayout::eUndefined;
	imageInfo.usage = vk::ImageUsageFlagBits::eDepthStencilAttachment;
	imageInfo.samples = FVK::getMaxUsableSampleCount(BasePhysicalDeviceKey);
	imageInfo.sharingMode = vk::SharingMode::eExclusive;

	FVK::ImageParameter param;
	param.logicalDeviceKey = BaseDeviceKey;
	param.pInfo = &imageInfo;

	FVK::createImage(BaseDepthImageKey, param);

	vk::MemoryRequirements memRequirements = FVK::getImageMemoryRequirements(BaseDeviceKey, BaseColorImageKey);

	FVK::DeviceMemoryAllocateInfo allocInfo = {
		.allocationSize = memRequirements.size,
		.memoryTypeIndex = FVK::findMemoryType(BasePhysicalDeviceKey,memRequirements.memoryTypeBits,vk::MemoryPropertyFlagBits::eDeviceLocal)
	};

	FVK::DeviceMemoryParameter param2;
	param2.pInfo = &allocInfo;
	param2.logicalDeviceKey = BaseDeviceKey;

	FVK::allocateDeviceMemory(BaseDepthImageMemoryKey, param2);
	FVK::bindImageMemory(BaseDeviceKey, BaseDepthImageKey, BaseDepthImageMemoryKey);

	vk::ImageSubresourceRange range = {
		range.aspectMask = vk::ImageAspectFlagBits::eDepth,
		range.baseMipLevel = 0,
		range.levelCount = 1,
		range.baseArrayLayer = 0,
		range.layerCount = 1,
	};

	FVK::ImageViewCreateInfo info;
	info.imageKey = BaseDepthImageKey;
	info.viewType = vk::ImageViewType::e2D;
	info.format = format;
	info.subresourceRange = range;

	FVK::ImageViewParameter param3;
	param3.logicalDeviceKey = BaseDeviceKey;
	param3.pInfo = &info;
	FVK::createImageView(BaseDepthImageViewKey, param3);


}

void commandPool() {
	using namespace FDR::Internal;

	FVK::CommandPoolCreateInfo info;

	info.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
	info.queueFamilyIndex = FVK::getGraphicsQueueFmily(BasePhysicalDeviceKey);


	FVK::CommandPoolParameter param;
	param.logicalDeviceKey = BaseDeviceKey;
	param.pInfo = &info;
	FVK::createCommandPool(BaseCommandPoolKey, param);

}

void commandBuffer() {
	using namespace FDR::Internal;

	auto size = FVK::getSwapchainMinImageCount(BaseSwapchainKey);

	FVK::CommandBufferAllocateInfo info;
	info.commandPoolKey = BaseCommandPoolKey;
	info.commandBufferCount = size;
	info.level = vk::CommandBufferLevel::ePrimary;

	FVK::CommandBufferParameter param;
	param.pInfo = &info;
	param.logicalDeviceKey = BaseDeviceKey;

	FVK::allocateCommandBuffers(BaseCommandBuffersKey, param);
}

void frameBuffer() {
	using namespace FDR::Internal;

	auto size = FVK::getSwapchainMinImageCount(BaseSwapchainKey);
	const auto extent = FVK::getSwapchainExtent(BaseSwapchainKey);

	for (uint16_t i = 0; i < size; i++) {
		std::vector<FVK::ImageViewKey<std::string>> attachments = {
			FVK::ImageViewKey<std::string>{BaseColorImageViewKey.get()},
			FVK::ImageViewKey<std::string>{BaseDepthImageViewKey.get()},
			BaseSwapchainImageViewKeys.at(i)
		};

		FVK::FrameBufferCreateInfo info;
		info.renderPassKey = BaseRenderPassKey;
		info.imageViewKeys = attachments;
		info.width = extent.width;
		info.height = extent.height;
		info.layers = 1;

		FVK::FrameBufferParameter param;
		param.pInfo = &info;
		param.logicalDeviceKey = BaseDeviceKey;


		BaseFrameBufferKeys.emplace_back(std::string("_BFB") + std::to_string(i));

		FVK::createFrameBuffer(BaseFrameBufferKeys[i], param);
	}

}

void sync() {
	using namespace FDR::Internal;

	for (uint16_t i = 0; i < MaxFramesInFlight; i++) {

		BaseImageAvailableSemaphoreKeys.emplace_back(std::string("_BIAS") + std::to_string(i));
		BaseRenderFinishedSemaphoreKeys.emplace_back(std::string("_BRFS") + std::to_string(i));

		FVK::SemaphoreCreateInfo info;
		FVK::SemaphoreParameter param;
		param.pInfo = &info;
		param.logicalDeviceKey = BaseDeviceKey;

		FVK::createSemaphore(BaseImageAvailableSemaphoreKeys[i], param);
		FVK::createSemaphore(BaseRenderFinishedSemaphoreKeys[i], param);

		BaseInFlightFenceKeys.emplace_back(std::string("_BIFF") + std::to_string(i));

		FVK::FenceCreateInfo finfo;
		finfo.flags = vk::FenceCreateFlagBits::eSignaled;

		FVK::FenceParameter fparam;
		fparam.logicalDeviceKey = BaseDeviceKey;
		fparam.pInfo = &finfo;

		FVK::createFence(BaseInFlightFenceKeys[i], fparam);
	}
}

void queue() {
	using namespace FDR::Internal;

	{
		FVK::QueueGetInfo info;
		info.queueFamilyIndex = FVK::getGraphicsQueueFmily(BasePhysicalDeviceKey);
		info.queueIndex = 0;
		FVK::QueueParameter param;
		param.logicalDevice = BaseDeviceKey;
		param.pInfo = &info;
		FVK::getQueue(BaseGraphicsQueueKey, param);

	}
	{
		FVK::QueueGetInfo info;
		info.queueFamilyIndex = FVK::getPresentQueueFmily(BasePhysicalDeviceKey);
		info.queueIndex = 0;
		FVK::QueueParameter param;
		param.logicalDevice = BaseDeviceKey;
		param.pInfo = &info;

		FVK::getQueue(BasePresentQueueKey, param);
	}
}


void descriptorPool() {
	using namespace FDR::Internal;

	std::vector<vk::DescriptorPoolSize> psize(1);
	psize[0].type = vk::DescriptorType::eCombinedImageSampler;
	psize[0].descriptorCount = 20;

	FVK::DescriptorPoolCreateInfo info;
	info.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;
	info.poolSizeCount = 1;
	info.pPoolSizes = psize.data();
	info.maxSets = 20;


	FVK::DescriptorPoolParameter param;
	param.pInfo = &info;
	param.logicalDeviceKey = BaseDeviceKey;

	FVK::createDescriptorPool(BaseImGuiDescriptorPoolKey, param);
}

void setupImGui() {
	using namespace FDR::Internal;

	FVK::ImGuiCreateInfo info;
	info.windowKey = BaseWindowKey;
	info.instanceKey = BaseInstanceKey;
	info.physicalDeviceKey = BasePhysicalDeviceKey;
	info.logicalDeviceKey = BaseDeviceKey;
	info.descriptorPoolKey = BaseImGuiDescriptorPoolKey;
	info.imageCount = FVK::getSwapchainMinImageCount(BaseSwapchainKey);
	info.minImageCount = FVK::getSwapchainMinImageCount(BaseSwapchainKey);
	info.msaaSamples = static_cast<VkSampleCountFlagBits>(FVK::getMaxUsableSampleCount(BasePhysicalDeviceKey));
	info.presentQueueKey = BasePresentQueueKey;
	info.renderPassKey = BaseRenderPassKey;

	FVK::ImGuiFontInfo font;
	ImFontConfig textConfig;
	textConfig.PixelSnapH = true;
	textConfig.RasterizerMultiply = 0.9f;
	textConfig.GlyphExtraSpacing = { -0.65f,0.0f };
	//textConfig.GlyphOffset = { 0.0f,2.0f };
	textConfig.OversampleH = 3;
	textConfig.OversampleV = 3;

	font.filePath = ImGuiDefaultFontFilePath;
	font.glyphRanges = FVK::ImGuiGlyphRangesType::Japanese;
	font.sizePixels = 32.0f;

	font.config = &textConfig;

	FVK::ImGuiFontInfo font2;
	ImFontConfig textConfig2;
	textConfig2.RasterizerMultiply = 0.9f;
	textConfig2.MergeMode = false;
	textConfig2.PixelSnapH = true;
	textConfig2.GlyphExtraSpacing = { -0.65f,0.0f };
	//textConfig2.GlyphOffset = { 0.0f,2.0f };
	textConfig2.OversampleH = 3;
	textConfig2.OversampleV = 3;
	font2.filePath = ImGuiDefaultFontFilePath;
	font2.glyphRanges = FVK::ImGuiGlyphRangesType::Japanese;
	font2.sizePixels = 26.0f;
	font2.config = &textConfig2;

	ImFontConfig iconConfig;
	iconConfig.MergeMode = true;
	iconConfig.PixelSnapH = true;
	iconConfig.GlyphOffset = { 0.0f,9.0f };
	iconConfig.OversampleH = 3;
	iconConfig.OversampleV = 3;
	iconConfig.GlyphMinAdvanceX = 40.0f;
	////iconConfig.GlyphExtraSpacing = { -0.65f,0.0f };
	FVK::ImGuiFontInfo iconFont;
	static const ImWchar iconsRanges[] = { ICON_MIN_MD, ICON_MAX_MD, 0 };
	iconFont.filePath = ImGuiDefaultIconFontFilePath;
	iconFont.glyphRanges = iconsRanges;
	iconFont.sizePixels = 40.0f;
	iconFont.config = &iconConfig;


	//default iconfont2
	ImFontConfig iconConfig2;
	iconConfig2.MergeMode = true;
	iconConfig2.PixelSnapH = true;
	iconConfig2.GlyphOffset = { 0.0f, 1.0f };
	//iconConfig2.GlyphExtraSpacing = { 0.60f, -0.60f };

	iconConfig2.OversampleH = 3;//FreeType -> ignore
	iconConfig2.OversampleV = 3;//FreeType -> ignore

	iconConfig2.GlyphMinAdvanceX = 40.0f;
	FVK::ImGuiFontInfo iconFont2;
	static const ImWchar iconsRanges2[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	iconFont2.filePath = ImGuiDefaultIconFont2FilePath;
	iconFont2.glyphRanges = iconsRanges2;
	iconFont2.sizePixels = 33.0f;
	iconFont2.config = &iconConfig2;

	FVK::ImGuiFontInfo fonts[] = { font,iconFont,iconFont2,font2 };

	FVK::ImGuiParameter param;
	param.commandPoolKey = BaseCommandPoolKey;
	param.graphicsQueueKey = BaseGraphicsQueueKey;
	param.pInfo = &info;
	param.pFontInfos = fonts;
	param.fontInfoCount = std::extent_v<decltype(fonts), 0>;

	FVK::createImGui(BaseImGuiKey, param);
	ImGui::GetIO().FontGlobalScale = 0.525f;

}

struct Camera {
	glm::vec3 pos = { 0.0f,0.0f,-4.0f };
	glm::vec3 center = { 0.0f,0.0f,0.0f };
	glm::vec3 up = { 0.0f,1.0f,0.0f };
	float aspect = 16.0f / 9.0f;
};
//
//void updateUniformBuffer(auto data, void* cam) {
//	Camera* camera = static_cast<Camera*>(cam);
//
//	FVK::UBO ubo{};
//
//	ubo.model = glm::mat4(1);// glm::scale(glm::mat4(1), glm::vec3(1.0f + time, 1.0f * time, 1.0f));
//	ubo.view = glm::lookAt(camera->pos, camera->center, camera->up);
//	ubo.proj = glm::perspective(glm::radians(45.0f), camera->aspect, 0.1f, 10.0f);
//
//	ubo.proj[1][1] *= -1;
//
//	void* vkdata;
//	vkMapMemory(std::get<0>(data), std::get<1>(data).second, 0, sizeof(ubo), 0, &vkdata);
//	memcpy(vkdata, &ubo, sizeof(ubo));
//	vkUnmapMemory(std::get<0>(data), std::get<1>(data).second);
//}

void makeCommands() {
	using namespace FDR::Internal;

	for (uint16_t i = 0; i < FVK::getSwapchainMinImageCount(BaseSwapchainKey); i++) {
		FVK::BeginCommandBufferCommand beginCommandBuffer = FVK::makeBeginCommandBufferCommand(BaseCommandBuffersKey, i);
		MainCommands.push(beginCommandBuffer);

		FVK::BeginRenderPassCommand beginRenderPass
			= FVK::makeBeginRenderPassCommand(BaseCommandBuffersKey, BaseRenderPassKey, BaseFrameBufferKeys[i], i);
		vk::Rect2D renderArea = {
			.offset = vk::Offset2D(0, 0),
			.extent = FVK::getSwapchainExtent(BaseSwapchainKey),
		};
		beginRenderPass->setRenderArea(renderArea);
		MainCommands.push(beginRenderPass);

		FVK::BindGraphicsPipelineCommand bindGPipeline
			= FVK::makeBindGraphicsPipelineCommand(BaseCommandBuffersKey, BaseGraphicsPipelineKey, i);
		MainCommands.push(bindGPipeline);

		FVK::SetScissorCommand setScissor = FVK::makeSetScissorCommand(BaseCommandBuffersKey, i);
		auto extent = FVK::getSwapchainExtent(BaseSwapchainKey);
		vk::Rect2D scissor;
		scissor.extent = extent;
		setScissor->setScissor(scissor);
		MainCommands.push(setScissor);

		FVK::SetViewportCommand setViewport = FVK::makeSetViewportCommand(BaseCommandBuffersKey, i);
		vk::Viewport viewport;
		viewport.width = static_cast<float>(extent.width);
		viewport.height = static_cast<float>(extent.height);
		setViewport->setViewport(viewport);
		MainCommands.push(setViewport);

		FVK::ImGuiRenderDrawDataCommand imguiRender
			= FVK::makeImGuiRenderDrawDataCommand(BaseCommandBuffersKey, i);
		MainCommands.push(imguiRender);

		FVK::EndRenderPassCommand endRenderPass = FVK::makeEndRenderPassCommand(BaseCommandBuffersKey, i);
		MainCommands.push(endRenderPass);


		FVK::EndCommandBufferCommand endCommandBuffer = FVK::makeEndCommandBufferCommand(BaseCommandBuffersKey, i);
		MainCommands.push(endCommandBuffer);

		FVK::NextCommand next = FVK::makeNextCommand();
		MainCommands.push(next);
	}

}

void sampler() {
	using namespace FDR::Internal;

	FVK::SamplerCreateInfo info;
	info.magFilter = vk::Filter::eLinear;
	info.minFilter = vk::Filter::eLinear;
	info.mipmapMode = vk::SamplerMipmapMode::eLinear;
	info.addressModeU = vk::SamplerAddressMode::eRepeat;
	info.addressModeV = vk::SamplerAddressMode::eRepeat;
	info.addressModeW = vk::SamplerAddressMode::eRepeat;
	info.anisotropyEnable = VK_FALSE;
	info.maxAnisotropy = VK_FALSE;// FVK::getPhysicalDeviceProperties(BasePhysicalDeviceKey).limits.maxSamplerAnisotropy;
	info.compareEnable = VK_FALSE;
	info.compareOp = vk::CompareOp::eAlways;
	info.borderColor = vk::BorderColor::eIntOpaqueBlack;
	info.unnormalizedCoordinates = VK_FALSE;

	FVK::SamplerParameter param;

	param.logicalDeviceKey = BaseDeviceKey;
	param.pInfo = &info;

	FVK::createSampler(BaseSamplerKey, param);
}

void FDR::Internal::Initialization::initialize(const FullScreenWithBarInfo& info) {

	window(info.title);
	instance();
	surface();
	physicalDevice();
	logicalDevice();
	swapchain();
	renderpass();
	descriptorSetLayout();
	shaderModule();
	graphicsPipelineLayout();
	graphicsPipeline();
	colorImage();
	colorImageView();
	depthImage();
	commandPool();
	commandBuffer();
	frameBuffer();
	sync();
	queue();
	descriptorPool();
	setupImGui();

	//èâä˙âªÇ…ïKóvÇ»Ç¢Ç™çÏê¨ÇµÇƒÇ®Ç≠
	sampler();

	makeCommands();

}
