#include "FVKapi.h"

namespace FVK::Internal {
	void addKeyCallback(const magic_enum::string_view& type, const char* key, OrderKey::Type order, const std::vector<OrderKey>& connections) {
		std::stringstream ss;
		ss << "Add " << "Index:" << order << " Key:" << key << " Type:" << type << " Connections:";
		for (const auto& x : connections)
			ss << x << " ";
		ss << '.';
		GMessenger.add<FU::Log::Type::None>(ss.str());
	}

	void eraseKeyCallback(const magic_enum::string_view& type, const char* key, OrderKey::Type order) {
		std::stringstream ss;
		ss << "Erase  " << "Index:" << order << " Key:" << key << " Type:" << type << '.';

		GMessenger.add<FU::Log::Type::None>(ss.str());
	}
}

void FVK::initialize() {
	using namespace Internal;
	LockGuard lock(GMutex);

	//callback
	GKeyManager.setAddCallback(addKeyCallback);
	GKeyManager.setEraseCallback(eraseKeyCallback);

	//ä˘Ç…çÏê¨Ç≥ÇÍÇƒÇ¢ÇÈ
	if (GManager)
		::FVK::Internal::Exception::throwFailedToInitialize("Failed to initialize FluidumVK. FluidumVK has already been created.");

	GManager = std::make_unique<Manager::Manager>();

	GMessenger.add<FU::Log::Type::None>("Initialize FluidumVK.");
}

void FVK::terminate() {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();

	GManager->terminate();
	GManager.reset();

	GMessenger.add<FU::Log::Type::None>("Terminate FluidumVK.");
}

bool FVK::isFluidumVKInitialized() noexcept {
	return Internal::GManager.operator bool();
}

void FVK::setMessengerCallbackFunction(const MessengerCallbackType callback) {
	Internal::GMessenger.setCallback(callback);
}

void FVK::createInstance(const char* key, const InstanceParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();
	GManager->add<FvkType::Instance>(key, parameter);
}

void FVK::createInstance(const char* key, const InstanceMessangerParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();

	GManager->add<FvkType::Instance>(key, parameter);

	std::string messangerKey = "__Messanger_";
	messangerKey += key;
	const IndexKey instanceIndex = Internal::GKeyManager.toIndexKey(key);
	const Key::RequiredDataKeys<FvkType::Messanger> connections{ InstanceKey<const char*>(key) };
	GManager->add<FvkType::Messanger>(messangerKey.c_str(), parameter.messangerParameter, connections);
}

void FVK::createSurface(const char* key, const SurfaceParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();
	const auto connections = Key::Converter::variantKeysToConnectionKeysBase<FvkType::Surface>(
		parameter.windowKey.value(),
		parameter.instanceKey.value()
		);
	GManager->add<FvkType::Surface>(key, parameter, connections);
}

void FVK::pickPhysicalDevice(const char* key, const PhysicalDeviceParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();
	const auto connections = Key::Converter::variantKeysToConnectionKeysBase<FvkType::PhysicalDevice>(
		parameter.instanceKey.value()
		);
	GManager->add<FvkType::PhysicalDevice>(key, parameter, connections);
}

void FVK::pickPhysicalDevice(const char* key, const PhysicalDeviceSwapchainParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();
	const auto connections = Key::Converter::variantKeysToConnectionKeysBase<FvkType::PhysicalDevice_Swapchain>(
		parameter.surfaceKey.value()
		);
	GManager->add<FvkType::PhysicalDevice_Swapchain>(key, parameter, connections);
}

void FVK::createLogicalDevice(const char* key, const LogicalDeviceParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();
	const auto connections = Key::Converter::variantKeysToConnectionKeysBase<FvkType::LogicalDevice>(
		parameter.physicalDeviceKey.value()
		);
	GManager->add<FvkType::LogicalDevice>(key, parameter, connections);
}

FluidumVK_API void FVK::getQueue(const char* key, const QueueParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();
	const auto connections = Key::Converter::variantKeysToConnectionKeysBase<FvkType::Queue>(
		parameter.logicalDevice.value()
		);
	GManager->add<FvkType::Queue>(key, parameter, connections);
}

void FVK::createSwapchain(const char* key, SwapchainParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();
	const auto connections = Key::Converter::variantKeysToConnectionKeysBase<FvkType::Swapchain>(
		parameter.logicalDeviceKey.value()
		);
	GManager->add<FvkType::Swapchain>(key, parameter, connections);
}

void FVK::createRenderPass(const char* key, const RenderPassParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();
	const auto connections = Key::Converter::variantKeysToConnectionKeysBase<FvkType::RenderPass>(
		parameter.logicalDeviceKey.value()
		);
	GManager->add<FvkType::RenderPass>(key, parameter, connections);
}

void FVK::createDescriptorSetLayout(const char* key, const DescriptorSetLayoutParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();
	const auto connections = Key::Converter::variantKeysToConnectionKeysBase<FvkType::DescriptorSetLayout>(
		parameter.logicalDeviceKey.value()
		);
	GManager->add<FvkType::DescriptorSetLayout>(key, parameter, connections);
}

void FVK::updateDescriptorSet(UpdateDescriptorSetParameter& parameter) {
	using namespace Internal;
	UpdateDescriptorSet update;

	{
		auto device = Key::Converter::variantKeyToIndexKey<FvkType::LogicalDevice>(parameter.logicalDeviceKey.value());
		const Data::LogicalDeviceInfo& info = GManager->refInfo<FvkType::LogicalDevice>(device);
		update.device = info.device;
	}
	{
		auto key = Key::Converter::variantKeyToIndexKey<FvkType::UniformBuffer>(parameter.pInfo->pUniformBufferInfo->uniformBufferKey.value());
		const Data::UniformBufferInfo& info = GManager->refInfo<FvkType::UniformBuffer>(key);
		update.buffer = info.uniformBuffer;
	}
	{
		auto key = Key::Converter::variantKeyToIndexKey<FvkType::DescriptorSet>(parameter.pInfo->descriptorSetKey.value());
		const Data::DescriptorSetInfo& info = GManager->refInfo<FvkType::DescriptorSet>(key);
		update.descriptorSet = info.descriptorSet;
	}
	update.updateUniformBuffer(parameter);

}

void FVK::createShaderModule(const char* key, const ShaderModuleParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();
	const auto connections = Key::Converter::variantKeysToConnectionKeysBase<FvkType::ShaderModule>(
		parameter.logicalDeviceKey.value()
		);
	GManager->add<FvkType::ShaderModule>(key, parameter, connections);
}

std::vector<char> FVK::readShaderFile(const char* filePath) {
	using namespace Internal;
	return ShaderModule::readFile(filePath);
}

void FVK::createGraphicsPipelineLayout(const char* key, GraphicsPipelineLayoutParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();
	//connectionsÇçÏê¨
	const auto connections = Key::Converter::variantKeysToConnectionKeysBase<FvkType::GraphicsPipelineLayout>(
		parameter.logicalDevice.value(),
		parameter.pInfo->descriptorSetLayoutKeys
		);
	GManager->add<FvkType::GraphicsPipelineLayout>(key, parameter, connections);
}

void FVK::createGraphicsPipeline(const char* key, GraphicsPipelineParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();
	const auto shaderStageCount = parameter.pInfo->stageCount;

	std::vector<Key::ShaderModuleVariantKey> shaderModuleKeys;
	for (std::size_t i = 0; i < shaderStageCount; i++) {
		shaderModuleKeys.emplace_back(parameter.pInfo->pStageCreateInfos[i].shaderModuleKey.value());
	}

	const Key::ShaderModuleVariantVectorKey shaderModuleVariantVectorKey =
		Key::Converter::variantKeysToVariantStrinKeyTypeVectorKey<FvkType::ShaderModule>(shaderModuleKeys);

	const auto connections =
		Key::Converter::variantKeysToConnectionKeysBase<FvkType::GraphicsPipeline>(
			parameter.logicalDeviceKey.value(),
			parameter.pInfo->renderPassKey.value(),
			shaderModuleVariantVectorKey,
			parameter.pInfo->graphicsPipelineLayoutKey.value()
			);

	GManager->add<FvkType::GraphicsPipeline>(key, parameter, connections);
}

void FVK::createImage(const char* key, const ImageParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();
	auto connections = Key::Converter::variantKeysToConnectionKeysBase<FvkType::Image>(parameter.logicalDeviceKey.value());
	GManager->add<FvkType::Image>(key, parameter, connections);
}

void FVK::createImageView(const char* key, ImageViewParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();
	auto connections = Key::Converter::variantKeysToConnectionKeysBase<FvkType::ImageView>(parameter.logicalDeviceKey.value(), parameter.pInfo->imageKey.value());
	GManager->add<FvkType::ImageView>(key, parameter, connections);
}

void FVK::createImageView(const char* key, const SwapchainImageViewParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();
	auto connections = Key::Converter::variantKeysToConnectionKeysBase<FvkType::ImageView_Swapchain>(parameter.logicalDeviceKey.value(), parameter.swapchainKey.value());
	GManager->add<FvkType::ImageView_Swapchain>(key, parameter, connections);
}

void FVK::allocateDeviceMemory(const char* key, const DeviceMemoryParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();
	auto connections = Key::Converter::variantKeysToConnectionKeysBase<FvkType::DeviceMemory>(parameter.logicalDeviceKey.value());
	GManager->add<FvkType::DeviceMemory>(key, parameter, connections);
}

void FVK::createCommandPool(const char* key, const CommandPoolParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();
	auto connections = Key::Converter::variantKeysToConnectionKeysBase<FvkType::CommandPool>(parameter.logicalDeviceKey.value());
	GManager->add<FvkType::CommandPool>(key, parameter, connections);
}

void FVK::createVertexBuffer(const char* key, const VertexBufferParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();
	auto connections = Key::Converter::variantKeysToConnectionKeysBase<FvkType::VertexBuffer>(
		parameter.physicalDeviceKey.value(),
		parameter.logicalDeviceKey.value(),
		parameter.commandPoolKey.value()
		);
	GManager->add<FvkType::VertexBuffer>(key, parameter, connections);
}

void FVK::createIndexBuffer(const char* key, const IndexBufferParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();
	auto connections = Key::Converter::variantKeysToConnectionKeysBase<FvkType::IndexBuffer>(
		parameter.physicalDeviceKey.value(),
		parameter.logicalDeviceKey.value(),
		parameter.commandPoolKey.value()
		);
	GManager->add<FvkType::IndexBuffer>(key, parameter, connections);
}

void FVK::createUniformBuffer(const char* key, const UniformBufferParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();
	auto connections = Key::Converter::variantKeysToConnectionKeysBase<FvkType::UniformBuffer>(
		parameter.physicalDeviceKey.value(),
		parameter.logicalDeviceKey.value()
		);
	GManager->add<FvkType::UniformBuffer>(key, parameter, connections);
}

void FVK::createDescriptorPool(const char* key, const DescriptorPoolParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();
	auto connections = Key::Converter::variantKeysToConnectionKeysBase<FvkType::DescriptorPool>(
		parameter.logicalDeviceKey.value()
		);
	GManager->add<FvkType::DescriptorPool>(key, parameter, connections);
}

void FVK::allocateDescriptorSet(const char* key, DescriptorSetParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();
	auto connections = Key::Converter::variantKeysToConnectionKeysBase<FvkType::DescriptorSet>(
		parameter.logicalDeviceKey.value(),
		parameter.pInfo->decsriptorSetLayoutKeys.value(),
		parameter.pInfo->descriptorPoolKey.value()
		);
	GManager->add<FvkType::DescriptorSet>(key, parameter, connections);
}

void FVK::allocateCommandBuffers(const char* key, CommandBufferParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();
	auto connections = Key::Converter::variantKeysToConnectionKeysBase<FvkType::CommandBuffer>(
		parameter.logicalDeviceKey.value(),
		parameter.pInfo->commandPoolKey.value()
		);
	GManager->add<FvkType::CommandBuffer>(key, parameter, connections);
}

void FVK::createFrameBuffer(const char* key, FrameBufferParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();
	auto connections = Key::Converter::variantKeysToConnectionKeysBase<FvkType::FrameBuffer>(
		parameter.logicalDeviceKey.value(),
		parameter.pInfo->renderPassKey.value(),
		parameter.pInfo->imageViewKeys.value()
		);
	GManager->add<FvkType::FrameBuffer>(key, parameter, connections);
}

void FVK::createSemaphore(const char* key, const SemaphoreParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();
	auto connections = Key::Converter::variantKeysToConnectionKeysBase<FvkType::Semaphore>(
		parameter.logicalDeviceKey.value()
		);
	GManager->add<FvkType::Semaphore>(key, parameter, connections);
}

void FVK::createFence(const char* key, const FenceParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();
	auto connections = Key::Converter::variantKeysToConnectionKeysBase<FvkType::Fence>(
		parameter.logicalDeviceKey.value()
		);
	GManager->add<FvkType::Fence>(key, parameter, connections);
}

void FVK::createSampler(const char* key, const SamplerParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();
	auto connections = Key::Converter::variantKeysToConnectionKeysBase<FvkType::Sampler>(
		parameter.logicalDeviceKey.value()
		);
	GManager->add<FvkType::Sampler>(key, parameter, connections);
}

void FVK::createTexture(const char* key, const TextureParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();
	auto connections = Key::Converter::variantKeysToConnectionKeysBase<FvkType::Texture>(
		parameter.physicalDeviceKey.value(),
		parameter.logicalDeviceKey.value(),
		parameter.graphicsQueueKey.value(),
		parameter.commandPoolKey.value()
		);

	GManager->add<FvkType::Texture>(key, parameter, connections);
}

void FVK::createImGui(const char* key, const ImGuiParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();

	std::vector<Key::QueueVariantKey> keys = {
		parameter.graphicsQueueKey.value(),
		parameter.pInfo->presentQueueKey.value()
	};
	const Key::QueueVariantVectorKey queueVariantVectorKeys =
		Key::Converter::variantKeysToVariantStrinKeyTypeVectorKey<FvkType::Queue>(keys);

	auto connections = Key::Converter::variantKeysToConnectionKeysBase<FvkType::ImGui>(
		parameter.pInfo->windowKey.value(),
		parameter.pInfo->instanceKey.value(),
		parameter.pInfo->physicalDeviceKey.value(),
		parameter.pInfo->logicalDeviceKey.value(),
		queueVariantVectorKeys,
		parameter.pInfo->renderPassKey.value(),
		parameter.commandPoolKey.value(),
		parameter.pInfo->descriptorPoolKey.value()
		);
	GManager->add<FvkType::ImGui>(key, parameter, connections);
}

ImTextureID FVK::createImGuiImage(const char* key, const ImGuiImageParameter& parameter) {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();

	auto connections = Key::Converter::variantKeysToConnectionKeysBase<FvkType::ImGuiImage>(
		parameter.samplerKey.value(),
		parameter.textureKey.value(),
		parameter.imguiKey.value()
		);
	GManager->add<FvkType::ImGuiImage>(key, parameter, connections);

	const Data::ImGuiImageInfo& info = GManager->refInfo<FvkType::ImGuiImage>(ImGuiImageKey<const char*>(key));
	return static_cast<ImTextureID>(info.descriptorSet);
}

FVK::NextCommand FVK::makeNextCommand() {
	using namespace Internal;
	LockGuard lock(GMutex);
	API::checkManagerEmpty();
	const auto connections = Key::NextConnectionKeys();
	return GManager->makeCommand<CommandType::Next>(connections, std::numeric_limits<uint32_t>::max());
}

void FVK::exeCommands(Commands& commands) {
	Internal::API::checkManagerEmpty();
	commands.call();
}


