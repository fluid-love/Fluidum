#pragma once

#include "FVKtype.h"

namespace FVK::Internal {

	//各アイテムの作成に必要なデータ
	template<FvkType Type>
	_NODISCARD consteval auto GetRequiredDataTypes() {
		using enum FvkType;
		if constexpr (Type == None)
			return DataTypeArray<0>();
		else if constexpr (Type == Window)
			return DataTypeArray<0>();
		else if constexpr (Type == Instance)
			return DataTypeArray<0>();
		else if constexpr (Type == Messanger)
			return DataTypeArray<1>({ Instance });
		else if constexpr (Type == Surface)
			return DataTypeArray<2>({ Window, Instance });
		else if constexpr (Type == PhysicalDevice)
			return DataTypeArray<1>({ Instance });
		else if constexpr (Type == PhysicalDevice_Swapchain)
			return DataTypeArray<1>({ Surface });
		else if constexpr (Type == LogicalDevice)
			return DataTypeArray<1>({ PhysicalDevice });
		else if constexpr (Type == Queue)
			return DataTypeArray<1>({ LogicalDevice });
		else if constexpr (Type == Swapchain)
			return DataTypeArray<1>({ LogicalDevice });
		else if constexpr (Type == RenderPass)
			return DataTypeArray<1>({ LogicalDevice });
		else if constexpr (Type == DescriptorSetLayout)
			return DataTypeArray<1>({ LogicalDevice });
		else if constexpr (Type == ShaderModule)
			return DataTypeArray<1>({ LogicalDevice });
		else if constexpr (Type == GraphicsPipelineLayout)
			return DataTypeArray<2>({ LogicalDevice, DescriptorSetLayout_Vector });
		else if constexpr (Type == GraphicsPipeline)
			return DataTypeArray<4>({ LogicalDevice, RenderPass ,ShaderModule_Vector, GraphicsPipelineLayout });
		else if constexpr (Type == Image)
			return DataTypeArray<1>({ LogicalDevice });
		else if constexpr (Type == ImageView)
			return DataTypeArray<2>({ LogicalDevice, Image });
		else if constexpr (Type == ImageView_Swapchain)
			return DataTypeArray<2>({ LogicalDevice, Swapchain });
		else if constexpr (Type == DeviceMemory)
			return DataTypeArray<1>({ LogicalDevice });
		else if constexpr (Type == CommandPool)
			return DataTypeArray<1>({ LogicalDevice });
		else if constexpr (Type == VertexBuffer || Type == IndexBuffer)
			return DataTypeArray<3>({ PhysicalDevice, LogicalDevice, CommandPool });
		else if constexpr (Type == UniformBuffer)
			return DataTypeArray<2>({ PhysicalDevice, LogicalDevice });
		else if constexpr (Type == DescriptorPool)
			return DataTypeArray<1>({ LogicalDevice });
		else if constexpr (Type == DescriptorSet)
			return DataTypeArray<3>({ LogicalDevice, DescriptorSetLayout_Vector, DescriptorPool });
		else if constexpr (Type == CommandBuffer)
			return DataTypeArray<2>({ LogicalDevice, CommandPool });
		else if constexpr (Type == FrameBuffer)
			return DataTypeArray<3>({ LogicalDevice, RenderPass, ImageView_Vector });
		else if constexpr (Type == Semaphore)
			return DataTypeArray<1>({ LogicalDevice });
		else if constexpr (Type == Fence)
			return DataTypeArray<1>({ LogicalDevice });
		else if constexpr (Type == Sampler)
			return DataTypeArray<1>({ LogicalDevice });
		else if constexpr (Type == Texture)
			return DataTypeArray<4>({ PhysicalDevice, LogicalDevice, Queue, CommandPool });
		else if constexpr (Type == Draw)
			return DataTypeArray<1>({ LogicalDevice });
		else if constexpr (Type == ImGui)
			return DataTypeArray<8>({ Window,Instance,PhysicalDevice,LogicalDevice,Queue_Vector,RenderPass,CommandPool,DescriptorPool });
		else if constexpr (Type == ImGuiImage)
			return DataTypeArray<3>({ Sampler, Texture, ImGui });

	}


	template<FvkType Type>
	_NODISCARD consteval std::size_t GetRequiredDataSize() {
		return GetRequiredDataTypes<Type>().size();
	}

	_NODISCARD consteval FvkType ToObjectType(const FvkType type) {
		if (type == FvkType::PhysicalDevice_Swapchain)
			return FvkType::PhysicalDevice;
		else if (type == FvkType::Queue_Vector)
			return FvkType::Queue;
		else if (type == FvkType::DescriptorSetLayout_Vector)
			return FvkType::DescriptorSetLayout;
		else if (type == FvkType::ShaderModule_Vector)
			return FvkType::ShaderModule;
		else if (type == FvkType::ImageView_Vector)
			return FvkType::ImageView;
		else if (type == FvkType::ImageView_Swapchain)
			return FvkType::ImageView;
		else if (type == FvkType::Semaphore_Vector)
			return FvkType::Semaphore;
		else if (type == FvkType::Fence_Vector)
			return FvkType::Fence;
		else
			return type;
	}

	_NODISCARD consteval FvkType ToObjectVectorType(const FvkType type) {
		if (type == FvkType::DescriptorSetLayout)
			return FvkType::DescriptorSetLayout_Vector;
		else if (type == FvkType::Queue)
			return FvkType::Queue_Vector;
		else if (type == FvkType::ShaderModule)
			return FvkType::ShaderModule_Vector;
		else if (type == FvkType::ImageView)
			return FvkType::ImageView_Vector;
		else if (type == FvkType::Semaphore)
			return FvkType::Semaphore_Vector;
		else if (type == FvkType::Fence)
			return FvkType::Fence_Vector;
		else
			return type;
	}

	template<FvkType Type>
	_NODISCARD consteval FvkType ToObjectType() {
		return ToObjectType(Type);
	}

	template<FvkType Type>
	_NODISCARD consteval FvkType ToObjectVectorType() {
		return ToObjectVectorType(Type);
	}

}
