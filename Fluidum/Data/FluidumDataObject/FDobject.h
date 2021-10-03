#pragma once

#include "VertexLog/FDvertexlog.h"
#include "property/Camera/FDproperty.h"
#include "../common/FDcommon.h"

namespace FD {

	//ベース
	class FvkKey final : protected Fluidum::Utils::Class::Lock {
	public:
		FvkKey(Internal::ManagerKey) {}
		~FvkKey() = default;
		FluidumUtils_Class_NoCopyNoMove(FvkKey)

	private:
		template<template <typename T> typename U>
		using Key = U<const char*>;

		template<template <typename T> typename U>
		using Keys = Object::Keys<U>;

	public:
		using Lock::getLock;

	public:

		const Key<FVK::WindowKey> window{ "FW" };
		const Key<FVK::InstanceKey> instance{ "FI" };
		const Key<FVK::SurfaceKey> surface{ "FS" };
		const Key<FVK::SwapchainKey> swapchain{ "FSC" };
		const Key<FVK::PhysicalDeviceKey> physicalDevice{ "FPD" };
		const Key<FVK::RenderPassKey> renderPass{ "FRP" };
		const Key<FVK::LogicalDeviceKey> device{ "FLD" };

		const Key<FVK::ImageKey> colorImage{ "FCI" };
		const Key<FVK::ImageViewKey> colorImageView{ "FCIV" };
		const Key<FVK::ImageKey> depthImage{ "FDI" };
		const Key<FVK::ImageViewKey> depthImageView{ "FDIV" };

		Keys<FVK::SwapchainImageViewKey> swapchainImageViews;
		const Key<FVK::CommandPoolKey> commandPool{ "FCP" };


		Keys<FVK::UniformBufferKey> uniformBuffers;
		Keys<FVK::UniformBufferMemoryKey> uniformBufferMemories;

		const Key<FVK::DescriptorSetLayoutKey> descriptorSetLayout{ "FDSL" };

		const Key<FVK::DescriptorPoolKey> uniformBufferDescriptorPool{ "FUBDP" };
		const Key<FVK::DescriptorPoolKey> textureDescriptorPool{ "FTDP" };

		Keys<FVK::DescriptorSetKey> descriptorSets;
		Keys<FVK::CommandBufferKey> commandBuffers;
		Keys<FVK::FrameBufferKey> frameBuffers;

		const Key<FVK::ShaderModuleKey> vertShaderModule{ "FVSM" };
		const Key<FVK::ShaderModuleKey> fragShaderModule{ "FFSM" };

		const Key<FVK::GraphicsPipelineLayoutKey> graphicalPipelineLayout{ "FGPL" };
		const Key<FVK::GraphicsPipelineKey> graphicalPipeline{ "FGP" };

		Keys<FVK::SemaphoreKey> semaphores;
		Keys<FVK::FenceKey> fences;

		const Key<FVK::DrawKey> draw{ "FD" };
		const Key<FVK::SamplerKey> sampler{ "FSMP" };
		const Key<FVK::FvkImGuiKey> imgui{ "FIG" };
	};

	struct Vertex final : protected Fluidum::Utils::Class::Lock {
	public:
		Vertex(Internal::ManagerKey) {}
		~Vertex() = default;
		FluidumUtils_Class_NoCopyNoMove(Vertex)

	public:
		using Lock::getLock;

	public:
		Object::Keys<FVK::VertexBufferKey> vertexBuffers = { FVK::VertexBufferKey<std::string>{"FVB"} };
		Object::Keys<FVK::IndexBufferKey> indexBuffers = { FVK::IndexBufferKey<std::string>{"FIB"} };

		static inline std::vector<Object::Vertices> vertices = { {
			{{-0.5f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
			{{0.5f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f}},
			{{0.0f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
			{{0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 1.0f, 1.0f}},
			{{0.0f, 0.0f, 0.5f}, {0.0f, 1.0f, 1.0f, 1.0f}},
			{{0.0f, 0.0f, -0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}},
		} };

		static inline std::vector<Object::Indices> indices =
		{ {
			0,3, 0,2, 0,4, 0,5,
			1,2, 1,3, 1,4, 1,5,
			4,2, 4,3, 5,2, 5,3
		} };


		std::vector<vk::VertexInputAttributeDescription> attributeDescriptions =
		{
		{
			{
			.location = 0,
			.binding = 0,
			.format = vk::Format::eR32G32B32Sfloat,
			.offset = offsetof(Object::Vertex, Object::Vertex::pos),
			}
			,
			{
			.location = 1,
			.binding = 0,
			.format = vk::Format::eR32G32B32A32Sfloat,
			.offset = offsetof(Object::Vertex, Object::Vertex::color)
			}
		}
		};

		vk::VertexInputBindingDescription bindingDescripton = {
			.binding = 0,
			.stride = sizeof(Object::Vertex),
			.inputRate = vk::VertexInputRate::eVertex
		};



	};

	class SharedObject final {
	public:
		SharedObject(Internal::ManagerKey) {}
		~SharedObject() = default;
		FluidumUtils_Class_NoCopyNoMove(SharedObject)

	public:


		std::vector<FVK::SetScissorCommand> scissors;//single thread / shared_ptr
		std::vector<FVK::SetViewportCommand> viewports;//single thread / shared_ptr




	};

	class VertexRead final {


	};

	class MainCamera final :protected Fluidum::Utils::Class::Lock {
	public:
		MainCamera(Internal::ManagerKey) {};
		~MainCamera() = default;
		FluidumUtils_Class_NoCopyNoMove(MainCamera)

	public:
		using Lock::getLock;
	public:
		Object::Camera::Camera camera = {};
		Object::Camera::Speed speed = Object::Camera::Limit::SpeedD;

	};



	//FluidumVK
	class FluidumVK final : public ::FVK::FluidumVK {
	public:
		FluidumVK(Internal::ManagerKey) : ::FVK::FluidumVK() {};
		~FluidumVK() = default;
		FluidumUtils_Class_NoCopyNoMove(FluidumVK)
	};

	//Commands
	class Commands final : protected FVK::Commands {
	public:
		Commands(std::nullptr_t null) : FVK::Commands(null) {}
		Commands(Internal::ManagerKey) : FVK::Commands() {}
		~Commands() = default;
		FluidumUtils_Class_Default_CopyMove(Commands)

	public:
		using FVK::Commands::operator->;
		using FVK::Commands::useCount;
		using FVK::Commands::operator=;

	public://明示的なupcastを許可		
		explicit operator const FVK::Commands* () const noexcept {
			return this;
		}
		explicit operator FVK::Commands* () noexcept {
			return this;
		}
		explicit operator FVK::Commands() const noexcept {
			return *this;
		}

	};
}
