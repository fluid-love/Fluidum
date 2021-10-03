#pragma once

#include "datakeys.h"
#include "commandkeys.h"

namespace FVK::Internal::Key {

	using WindowConnectionKeys = RequiredDataKeys<FvkType::Window>;
	using InstanceConnectionKeys = RequiredDataKeys<FvkType::Instance>;
	using GraphicsPipelineLayoutConnectionKeys = RequiredDataKeys<FvkType::GraphicsPipelineLayout>;

}

namespace FVK::Internal::Key {

	using BeginCommandBufferConnectionKeys = ConnectionCommandKeys<CommandType::BeginCommandBuffer>;
	using EndCommandBufferConnectionKeys = ConnectionCommandKeys<CommandType::EndCommandBuffer>;
	using BindVertexbuffersConnectionKeys = ConnectionCommandKeys<CommandType::BindVertexBuffers>;
	using BeginRenderPassConnectionKeys = ConnectionCommandKeys<CommandType::BeginRenderPass>;
	using BindDescriptorSetConnectionKeys = ConnectionCommandKeys<CommandType::BindDescriptorSet>;
	using BindGraphicsPipelineConnectionKeys = ConnectionCommandKeys<CommandType::BindGraphicsPipeline>;
	using BindIndexBufferConnectionKeys = ConnectionCommandKeys<CommandType::BindIndexBuffer>;
	using DrawIndexedConnectionKeys = ConnectionCommandKeys<CommandType::DrawIndexed>;
	using EndRenderPassConnectionKeys = ConnectionCommandKeys<CommandType::EndRenderPass>;
	using SetScissorConnectionKeys = ConnectionCommandKeys<CommandType::SetScissor>;
	using SetViewportConnectionKeys = ConnectionCommandKeys<CommandType::SetViewport>;
	using ImGuiRenderDrawDataConnectionKeys = ConnectionCommandKeys<CommandType::ImGuiRenderDrawData>;
	using BindImGuiImageConnectionKeys = ConnectionCommandKeys<CommandType::BindImGuiImage>;
	using UpdateVertexBufferConnectionKeys = ConnectionCommandKeys<CommandType::UpdateVertexBuffer>;
	using UpdateIndexBufferConnectionKeys = ConnectionCommandKeys<CommandType::UpdateIndexBuffer>;
	using TransitionImageLayoutConnectionKeys = ConnectionCommandKeys<CommandType::TransitionImageLayout>;
	using QueueSubmitConnectionKeys = ConnectionCommandKeys<CommandType::QueueSubmit>;
	using NextConnectionKeys = ConnectionCommandKeys<CommandType::Next>;

}
