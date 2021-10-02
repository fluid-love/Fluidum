#pragma once

#include "FVKtype.h"

namespace FVK::Internal {

	template<CommandType Type>
	_NODISCARD consteval auto GetCommandDataTypes() {
		using enum FvkType;

		if constexpr (Type == CommandType::BeginCommandBuffer)
			return DataTypeArray<1>({ CommandBuffer });
		else if constexpr (Type == CommandType::EndCommandBuffer)
			return DataTypeArray<1>({ CommandBuffer });
		else if constexpr (Type == CommandType::BindVertexBuffers)
			return DataTypeArray<2>({ CommandBuffer, VertexBuffer });
		else if constexpr (Type == CommandType::BeginRenderPass)
			return DataTypeArray<3>({ CommandBuffer, RenderPass, FrameBuffer });
		else if constexpr (Type == CommandType::BindDescriptorSet)
			return DataTypeArray<3>({ CommandBuffer, GraphicsPipelineLayout, DescriptorSet });
		else if constexpr (Type == CommandType::BindGraphicsPipeline)
			return DataTypeArray<2>({ CommandBuffer, GraphicsPipeline });
		else if constexpr (Type == CommandType::BindIndexBuffer)
			return DataTypeArray<2>({ CommandBuffer, IndexBuffer });
		else if constexpr (Type == CommandType::DrawIndexed)
			return DataTypeArray<1>({ CommandBuffer });
		else if constexpr (Type == CommandType::EndRenderPass)
			return DataTypeArray<1>({ CommandBuffer });
		else if constexpr (Type == CommandType::SetScissor)
			return DataTypeArray<1>({ CommandBuffer });
		else if constexpr (Type == CommandType::SetViewport)
			return DataTypeArray<1>({ CommandBuffer });
		else if constexpr (Type == CommandType::ImGuiRenderDrawData)
			return DataTypeArray<1>({ CommandBuffer });
		else if constexpr (Type == CommandType::BindImGuiImage)
			return DataTypeArray<2>({ CommandBuffer, ImGuiImage });
		else if constexpr (Type == CommandType::UpdateVertexBuffer)
			return DataTypeArray<2>({ CommandBuffer, VertexBuffer });
		else if constexpr (Type == CommandType::UpdateIndexBuffer)
			return DataTypeArray<2>({ CommandBuffer, IndexBuffer });
		else if constexpr (Type == CommandType::TransitionImageLayout)
			return DataTypeArray<2>({ CommandBuffer, Image });
		else if constexpr (Type == CommandType::QueueSubmit)
			return DataTypeArray<2>({ CommandBuffer, Queue });
		else if constexpr (Type == CommandType::Next)
			return DataTypeArray<0>();
	}

	template<CommandType Type>
	_NODISCARD consteval std::size_t GetCommandDataTypesSize() {
		return GetCommandDataTypes<Type>().size();
	}

}
