#pragma once

#include "../Common/type.h"
#include "../Common/fvktype.h"
#include "../Common/commandtype.h"
#include "type.h"

namespace FVK::Internal::Data {

	template<FvkType Type>
	struct Data final {
	public:
		using Tuple = DataTuple<GetRequiredDataTypes<Type>()>;

	public:
		explicit Data(const Tuple& data) : data(data) {}
		explicit Data(Tuple&& data) : data(std::move(data)) {}

		~Data() noexcept = default;
		FluidumUtils_Class_Default_CopyMove(Data);

	public:
		template<FvkType T> requires(!FU::Concept::IsStdVector<CorrespondenceType<T>>)
			[[nodiscard]] CorrespondenceType<T>::type& get() const noexcept {
			return std::get<CorrespondenceType<T>>(data).get();
		}

		//vector<reference_wapper<ref>>
		template<FvkType T> requires(FU::Concept::IsStdVector<CorrespondenceType<T>>)
			[[nodiscard]] auto& get() const noexcept {
			return std::get<CorrespondenceType<T>>(data);
		}
	private:
		const Tuple data;

	};

	using WindowData = Data<FvkType::Window>;
	using InstanceData = Data<FvkType::Instance>;
	using MessengerData = Data<FvkType::Messenger>;
	using SurfaceData = Data<FvkType::Surface>;
	using PhysicalDeviceData = Data<FvkType::PhysicalDevice>;
	using PhysicalDeviceSwapchainData = Data<FvkType::PhysicalDevice_Swapchain>;
	using LogicalDeviceData = Data<FvkType::LogicalDevice>;
	using QueueData = Data<FvkType::Queue>;
	using SwapchainData = Data<FvkType::Swapchain>;
	using RenderPassData = Data<FvkType::RenderPass>;
	using DescriptorSetLayoutData = Data<FvkType::DescriptorSetLayout>;
	using ShaderModuleData = Data<FvkType::ShaderModule>;
	using GraphicsPipelineLayoutData = Data<FvkType::GraphicsPipelineLayout>;
	using GraphicsPipelineData = Data<FvkType::GraphicsPipeline>;
	using ImageData = Data<FvkType::Image>;
	using ImageViewData = Data<FvkType::ImageView>;
	using ImageViewSwapchainData = Data<FvkType::ImageView_Swapchain>;
	using DeviceMemoryData = Data<FvkType::DeviceMemory>;
	using CommandPoolData = Data<FvkType::CommandPool>;
	using VertexBufferData = Data<FvkType::VertexBuffer>;
	using IndexBufferData = Data<FvkType::IndexBuffer>;
	using UniformBufferData = Data<FvkType::UniformBuffer>;
	using DescriptorPoolData = Data<FvkType::DescriptorPool>;
	using DescriptorSetData = Data<FvkType::DescriptorSet>;
	using CommandBufferData = Data<FvkType::CommandBuffer>;
	using FrameBufferData = Data<FvkType::FrameBuffer>;
	using SemaphoreData = Data<FvkType::Semaphore>;
	using FenceData = Data<FvkType::Fence>;
	using SamplerData = Data<FvkType::Sampler>;
	using TextureData = Data<FvkType::Texture>;
	using ImGuiData = Data<FvkType::ImGui>;
	using ImGuiImageData = Data<FvkType::ImGuiImage>;

}

namespace FVK::Internal::Data {

	template<CommandType Type>
	struct CommandData final {
	public:
		using Tuple = ::FVK::Internal::Data::DataTuple<GetCommandDataTypes<Type>()>;

	public:
		explicit CommandData(const Tuple& data) : data(data) {}
		explicit CommandData(Tuple&& data) : data(std::move(data)) {}

		~CommandData() noexcept = default;
		FluidumUtils_Class_Default_CopyMove(CommandData);

	public:
		template<FvkType T> requires(!FU::Concept::IsStdVector<CorrespondenceType<T>>)
			[[nodiscard]] CorrespondenceType<T>::type& get() const noexcept {
			return std::get<CorrespondenceType<T>>(data).get();
		}

		//vector<reference_wapper<ref>>
		template<FvkType T> requires(FU::Concept::IsStdVector<CorrespondenceType<T>>)
			[[nodiscard]] auto& get() const noexcept {
			return std::get<CorrespondenceType<T>>(data);
		}
	private:
		const Tuple data;

	};


	using BeginCommandBufferData = CommandData<CommandType::BeginCommandBuffer>;
	using EndCommandBufferData = CommandData<CommandType::EndCommandBuffer>;
	using BindVertexBuffersData = CommandData<CommandType::BindVertexBuffers>;
	using BindIndexBufferData = CommandData<CommandType::BindIndexBuffer>;
	using DrawIndexedData = CommandData<CommandType::DrawIndexed>;
	using BindDescriptorSetData = CommandData<CommandType::BindDescriptorSet>;
	using BindGraphicsPipelineData = CommandData<CommandType::BindGraphicsPipeline>;
	using BeginRenderPassData = CommandData<CommandType::BeginRenderPass>;
	using EndRenderPassData = CommandData<CommandType::EndRenderPass>;
	using SetScissorData = CommandData<CommandType::SetScissor>;
	using SetViewportData = CommandData<CommandType::SetViewport>;
	using ImGuiRenderDrawDataData = CommandData<CommandType::ImGuiRenderDrawData>;
	using BindImGuiImageData = CommandData<CommandType::BindImGuiImage>;
	using UpdateVertexBufferData = CommandData<CommandType::UpdateVertexBuffer>;
	using UpdateIndexBufferData = CommandData<CommandType::UpdateIndexBuffer>;
	using TransitionImageLayoutData = CommandData<CommandType::TransitionImageLayout>;
	using QueueSubmitData = CommandData<CommandType::QueueSubmit>;
	using NextData = CommandData<CommandType::Next>;
	using AnyData = CommandData<CommandType::Any>;
	using CommandsData = CommandData<CommandType::Commands>;

}