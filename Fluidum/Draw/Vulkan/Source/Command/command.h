#pragma once

#include "element.h"

namespace FVK::Internal::Command {

	template<CommandType Type>
	struct Command final {
	private:
		using Element = CommandElement<Type>;
		using SharedPtr = std::shared_ptr<Element>;

	private:
		SharedPtr ptr;

	private:
		explicit Command(SharedPtr&& ptr) :ptr(std::move(ptr)) {}//Manager‚Ì‚Ý
	public:
		~Command() = default;
		FluidumUtils_Class_Default_CopyMove(Command)

	public:
		_NODISCARD Element* operator->() const noexcept {
			return this->ptr.get();
		}

		_NODISCARD std::size_t count() const noexcept {
			return this->ptr.use_count();
		}

	private:
		friend ::FVK::Internal::Manager::Manager;
		friend Commands;
	};

	template<FvkType ...T>
	struct AnyCommand final {
	private:
		using Element = AnyCommandElement<T...>;
		using SharedPtr = std::shared_ptr<Element>;

	private://data
		SharedPtr ptr = nullptr;

	private:
		explicit AnyCommand(SharedPtr&& ptr) :ptr(std::move(ptr)) {}//Manager‚Ì‚Ý
	public:
		~AnyCommand() = default;
		FluidumUtils_Class_Default_CopyMove(AnyCommand)

	public:
		_NODISCARD Element* operator->() const noexcept {
			return this->ptr.get();
		}

		_NODISCARD std::size_t count() const noexcept {
			return this->ptr.use_count();
		}

	private:
		friend ::FVK::Internal::Manager::Manager;
		friend Commands;
	};

}

namespace FVK::Internal::Command {

	using BeginCommandBufferCommand = Command<CommandType::BeginCommandBuffer>;
	using EndCommandBufferCommand = Command<CommandType::EndCommandBuffer>;
	using BindVertexBuffersCommand = Command<CommandType::BindVertexBuffers>;
	using BeginRenderPassCommand = Command<CommandType::BeginRenderPass>;
	using BindDescriptorSetCommand = Command<CommandType::BindDescriptorSet>;
	using BindGraphicsPipelineCommand = Command<CommandType::BindGraphicsPipeline>;
	using DrawIndexedCommand = Command<CommandType::DrawIndexed>;
	using BindIndexBufferCommand = Command<CommandType::BindIndexBuffer>;
	using EndRenderPassCommand = Command<CommandType::EndRenderPass>;
	using SetScissorCommand = Command<CommandType::SetScissor>;
	using SetViewportCommand = Command<CommandType::SetViewport>;
	using ImGuiRenderDrawDataCommand = Command<CommandType::ImGuiRenderDrawData>;
	using BindImGuiImageCommand = Command<CommandType::BindImGuiImage>;
	using UpdateVertexBufferCommand = Command<CommandType::UpdateVertexBuffer>;
	using UpdateIndexBufferCommand = Command<CommandType::UpdateIndexBuffer>;
	using TransitionImageLayoutCommand = Command<CommandType::TransitionImageLayout>;
	using QueueSubmitCommand = Command<CommandType::QueueSubmit>;
	using NextCommand = Command<CommandType::Next>;

}
