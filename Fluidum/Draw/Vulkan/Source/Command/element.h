#pragma once

#include "../Common/include.h"
#include "../ImGui/backend.h"

//forward
namespace FVK::Internal::Command {
	class Commands;
}

namespace FVK::Internal::Command {

	template<CommandType>
	class CommandElement final {
		FluidumUtils_Class_Delete_ConDestructor(CommandElement);
	};

	template<>
	class CommandElement<CommandType::BeginCommandBuffer> final {
	public:
		explicit CommandElement(ManagerPassKey, const Data::BeginCommandBufferData& data, const UI32 index)
			: commandBuffer(data.get<FvkType::CommandBuffer>().commandBuffers.at(index))
		{}
		~CommandElement() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(CommandElement);

	private:
		//strong
		void call() {
			vk::CommandBufferBeginInfo beginInfo{};
			beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

			auto result = commandBuffer.begin(beginInfo);
			if (result != vk::Result::eSuccess)
				Exception::throwUnexpected();
		}

	private:
		const vk::CommandBuffer commandBuffer;

	private:
		friend class Commands;//call

	};

	template<>
	class CommandElement<CommandType::EndCommandBuffer> final {
	public:
		explicit CommandElement(ManagerPassKey, const Data::EndCommandBufferData& data, const UI32 index)
			: commandBuffer(data.get<FvkType::CommandBuffer>().commandBuffers.at(index))
		{}
		~CommandElement() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(CommandElement);

	private:
		//strong
		void call() {
			auto result = commandBuffer.end();
			if (result != vk::Result::eSuccess)
				Exception::throwUnexpected();
		}

	private:
		const vk::CommandBuffer commandBuffer;

	private:
		friend class Commands;//call

	};

	template<>
	class CommandElement<CommandType::BindVertexBuffers> final {
	public:
		explicit CommandElement(ManagerPassKey, const Data::BindVertexBuffersData& data, const UI32 index)
			: commandBuffer(data.get<FvkType::CommandBuffer>().commandBuffers.at(index)),
			vertexBuffer(data.get<FvkType::VertexBuffer>().vertexBuffer)
		{}
		~CommandElement() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(CommandElement);

	private:
		void call() const noexcept {
			commandBuffer.bindVertexBuffers(0, 1, &vertexBuffer, &offset);
		}
		//multiple‘Î‰ž—\’è
		//vk::Buffer mainVertexBuffers[1] = { data.vertexBuffer };
		//vk::DeviceSize mainOffsets[1] = { 0 };
		//this->commandBuffer.bindVertexBuffers( 0, 1, mainVertexBuffers, mainOffsets);

		//strong
		void setOffset(const vk::DeviceSize offset) {
			LockGuard lock(GMutex);
			this->offset = offset;
		}

	private:
		vk::DeviceSize offset = 0;

		const vk::CommandBuffer commandBuffer;
		const vk::Buffer vertexBuffer;

	private:
		friend class Commands;//call

	};

	template<>
	class CommandElement<CommandType::BeginRenderPass> final {
	public:
		explicit CommandElement(ManagerPassKey, const Data::BeginRenderPassData& data, const UI32 index)
			: commandBuffer(data.get<FvkType::CommandBuffer>().commandBuffers.at(index)),
			renderPass(data.get<FvkType::RenderPass>().renderPass),
			frameBuffer(data.get<FvkType::FrameBuffer>().frameBuffer)
		{}
		~CommandElement() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(CommandElement);

	private:
		void call() const noexcept {
			auto clearValues = getClearValues();
			vk::RenderPassBeginInfo renderPassInfo = {
				.renderPass = renderPass,
				.framebuffer = frameBuffer,
				.renderArea = renderArea,
				.clearValueCount = static_cast<UI32>(clearValues.size()),
				.pClearValues = clearValues.data()
			};
			commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
		}

	public:
		void setBackColor(const std::array<float, 4>& color) {
			LockGuard lock(GMutex);
			this->backColor = color;
		}

		std::array<float, 4> getBackColor() {
			LockGuard lock(GMutex);
			return this->backColor;
		}

		void setRenderArea(const vk::Rect2D& renderArea) {
			LockGuard lock(GMutex);
			this->renderArea = renderArea;
		}

	private:
		vk::Rect2D renderArea = {
			.offset = vk::Offset2D(0, 0),
			.extent = vk::Extent2D(0, 0),
		};

		std::array<float, 4> backColor = { 0.0f, 0.0f, 0.0f, 1.0f };

		std::array<vk::ClearValue, 2> getClearValues() const {
			std::array<vk::ClearValue, 2> arr{};
			arr[0].color = backColor;
			arr[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0);
			return arr;
		};

		const vk::CommandBuffer commandBuffer;
		const vk::RenderPass renderPass;
		const vk::Framebuffer frameBuffer;

	private:
		friend class Commands;//call

	};

	template<>
	struct CommandElement<CommandType::BindDescriptorSet> final {
	public:
		explicit CommandElement(ManagerPassKey, const Data::BindDescriptorSetData& data, const UI32 index)
			: commandBuffer(data.get<FvkType::CommandBuffer>().commandBuffers.at(index)),
			pipelineLayout(data.get<FvkType::GraphicsPipelineLayout>().graphicsPipelineLayout),
			descriptorSet(data.get<FvkType::DescriptorSet>().descriptorSet)
		{}
		~CommandElement() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(CommandElement);

	private:
		void call() const noexcept {
			commandBuffer.bindDescriptorSets(
				vk::PipelineBindPoint::eGraphics,
				pipelineLayout,
				0,
				1,
				&descriptorSet,
				static_cast<UI32>(dynamicOffsets.size()),
				dynamicOffsets.data()
			);
		}

	public:
		void setDynamicOffsets(const std::vector<UI32>& dynamicOffsets) {
			LockGuard lock(GMutex);
			this->dynamicOffsets = dynamicOffsets;
		}

	private:
		std::vector<UI32> dynamicOffsets = { 0 };

		const vk::CommandBuffer commandBuffer;
		const vk::PipelineLayout pipelineLayout;
		const vk::DescriptorSet descriptorSet;

	private:
		friend class Commands;//call

	};

	template<>
	struct CommandElement<CommandType::BindGraphicsPipeline> final {
	public:
		explicit CommandElement(ManagerPassKey, const Data::BindGraphicsPipelineData& data, const UI32 index)
			: commandBuffer(data.get<FvkType::CommandBuffer>().commandBuffers.at(index)),
			graphicsPipeline(data.get<FvkType::GraphicsPipeline>().graphicsPipeline)
		{}
		~CommandElement() = default;
		FluidumUtils_Class_Delete_CopyMove(CommandElement);

	private:
		void call() {
			commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);
		}

		vk::CommandBuffer commandBuffer = nullptr;
		vk::Pipeline graphicsPipeline = nullptr;
	private:
		friend class Commands;//call
	};

	template<>
	struct CommandElement<CommandType::BindIndexBuffer> final {
	public:
		explicit CommandElement(ManagerPassKey, const Data::BindIndexBufferData& data, const UI32 index)
			: commandBuffer(data.get<FvkType::CommandBuffer>().commandBuffers.at(index)),
			indexBuffer(data.get<FvkType::IndexBuffer>().indexBuffer)
		{}
		~CommandElement() = default;
		FluidumUtils_Class_Delete_CopyMove(CommandElement);

	private:
		void call() const noexcept {
			commandBuffer.bindIndexBuffer(indexBuffer, 0, vk::IndexType::eUint32);
		}

		vk::CommandBuffer commandBuffer = nullptr;
		vk::Buffer indexBuffer = nullptr;
	private:
		friend class Commands;//call

	};

	template<>
	struct CommandElement<CommandType::DrawIndexed> final {
	public:
		explicit CommandElement(ManagerPassKey, const Data::DrawIndexedData& data, const UI32 index)
			: commandBuffer(data.get<FvkType::CommandBuffer>().commandBuffers.at(index))
		{}
		~CommandElement() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(CommandElement);

	private:
		void call() const noexcept {
			commandBuffer.drawIndexed(drawSize, 1, 0, 0, 0);
		}

	public:
		void setDrawSize(const UI32 drawSize) {
			LockGuard lock(GMutex);
			this->drawSize = drawSize;
		}

	private:
		UI32 drawSize = 0;

		const vk::CommandBuffer commandBuffer;

	private:
		friend class Commands;//call

	};

	template<>
	struct CommandElement<CommandType::EndRenderPass> final {
	public:
		explicit CommandElement(ManagerPassKey, const Data::EndRenderPassData& data, const UI32 index)
			: commandBuffer(data.get<FvkType::CommandBuffer>().commandBuffers.at(index))
		{}
		~CommandElement() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(CommandElement);

	private:
		void call() const noexcept {
			commandBuffer.endRenderPass();
		}

	private:
		const vk::CommandBuffer commandBuffer;
	private:
		friend class Commands;//call

	};

	template<>
	struct CommandElement<CommandType::SetScissor> final {
	public:
		explicit CommandElement(ManagerPassKey, const Data::SetScissorData& data, const UI32 index)
			: commandBuffer(data.get<FvkType::CommandBuffer>().commandBuffers.at(index))
		{}
		~CommandElement() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(CommandElement);

	private:
		void call() const noexcept {
			commandBuffer.setScissor(0, 1, &scissor);
		}

	public:
		void setScissor(const vk::Rect2D& scissor) {
			LockGuard lock(GMutex);
			this->scissor = scissor;
		}

		void setScissorOffset(const vk::Offset2D& offset) {
			LockGuard lock(GMutex);
			this->scissor.offset = offset;
		}

		void setScissorOffsetX(const int32_t x) {
			LockGuard lock(GMutex);
			this->scissor.offset.x = x;
		}

		void setScissorOffsetY(const int32_t y) {
			LockGuard lock(GMutex);
			this->scissor.offset.y = y;
		}

		void setScissorExtent(const vk::Extent2D& extent) {
			LockGuard lock(GMutex);
			this->scissor.extent = extent;
		}

		void setScissorExtentWidth(const UI32 width) {
			LockGuard lock(GMutex);
			this->scissor.extent.width = width;
		}

		void setScissorExtentHeight(const UI32 height) {
			LockGuard lock(GMutex);
			this->scissor.extent.height = height;
		}

	private:
		vk::Rect2D scissor{};

		const vk::CommandBuffer commandBuffer;

	private:
		friend class Commands;//call

	};

	template<>
	struct CommandElement<CommandType::SetViewport> final {
	public:
		explicit CommandElement(ManagerPassKey, const Data::SetViewportData& data, const UI32 index)
			: commandBuffer(data.get<FvkType::CommandBuffer>().commandBuffers.at(index))
		{}
		~CommandElement() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(CommandElement);

	public:
		void call() const noexcept {
			commandBuffer.setViewport(0, 1, &viewport);
		}

		void setViewport(const vk::Viewport& viewport) {
			LockGuard lock(GMutex);
			this->viewport = viewport;
		}

		void setViewportWidth(const float width) {
			LockGuard lock(GMutex);
			this->viewport.width = width;
		}

		void setViewportHeight(const float height) {
			LockGuard lock(GMutex);
			this->viewport.height = height;
		}

		void setViewportX(const float x) {
			LockGuard lock(GMutex);
			this->viewport.x = x;
		}

		void setViewportY(const float y) {
			LockGuard lock(GMutex);
			this->viewport.y = y;
		}

		void setViewportMaxDepth(const float depth) {
			LockGuard lock(GMutex);
			this->viewport.maxDepth = depth;
		}

		void setViewportMinDepth(const float depth) {
			LockGuard lock(GMutex);
			this->viewport.minDepth = depth;
		}

	private:
		vk::Viewport viewport{};

		const vk::CommandBuffer commandBuffer;

	private:
		friend class Commands;//call
	
	};

	template<>
	struct CommandElement<CommandType::ImGuiRenderDrawData> final {
	public:
		explicit CommandElement(ManagerPassKey, const Data::ImGuiRenderDrawDataData& data, const UI32 index)
			: commandBuffer(data.get<FvkType::CommandBuffer>().commandBuffers.at(index))
		{}
		~CommandElement() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(CommandElement);

	private:
		void call() const {
			ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
		}

	private:
		const vk::CommandBuffer commandBuffer;

	private:
		friend class Commands;//call
	
	};

	template<>
	struct CommandElement<CommandType::BindImGuiImage> final {
	public:
		explicit CommandElement(ManagerPassKey, const Data::BindImGuiImageData& data, const UI32 index)
			: commandBuffer(data.get<FvkType::CommandBuffer>().commandBuffers.at(index)),
			descriptorSet(data.get<FvkType::ImGuiImage>().descriptorSet)
		{}

		~CommandElement() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(CommandElement);

	private:
		void call() {
		}

	private:
		//vk::Pipeline graphicsPipeline = nullptr;
		vk::DescriptorSet descriptorSet = nullptr;
		vk::CommandBuffer commandBuffer = nullptr;

	private:
		friend class Commands;//call
	};

	template<>
	struct CommandElement<CommandType::UpdateVertexBuffer> final {
	public:
		explicit CommandElement(ManagerPassKey, const Data::UpdateVertexBufferData& data, const UI32 index)
			: commandBuffer(data.get<FvkType::CommandBuffer>().commandBuffers.at(index)),
			vertexBuffer(data.get<FvkType::VertexBuffer>().vertexBuffer)
		{}
		~CommandElement() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(CommandElement);

	private:
		void call() const noexcept {
			assert(verticesPtr != nullptr);
			commandBuffer.updateBuffer(vertexBuffer, dstOffset, dataSize, verticesPtr);
		}

		void setDstOffset(const vk::DeviceSize& dstOffset) {
			LockGuard lock(GMutex);
			this->dstOffset = dstOffset;
		}

		void setDataSize(const vk::DeviceSize& dataSize) {
			LockGuard lock(GMutex);
			this->dataSize = dataSize;
		}

		void setVerticesPtr(void* verticesPtr) {
			LockGuard lock(GMutex);
			this->verticesPtr = verticesPtr;
		}

	private:
		vk::DeviceSize dstOffset = 0;
		vk::DeviceSize dataSize = 0;

		void* verticesPtr = nullptr;

		const vk::CommandBuffer commandBuffer;
		const vk::Buffer vertexBuffer;

	private:
		friend class Commands;//call

	};

	template<>
	struct CommandElement<CommandType::UpdateIndexBuffer> final {
	public:
		explicit CommandElement(ManagerPassKey, const Data::UpdateIndexBufferData& data, const UI32 index)
			: commandBuffer(data.get<FvkType::CommandBuffer>().commandBuffers.at(index)),
			indexBuffer(data.get<FvkType::IndexBuffer>().indexBuffer)
		{}
		~CommandElement() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(CommandElement);

	private:
		void call() const noexcept {
			assert(indicesPtr != nullptr);
			commandBuffer.updateBuffer(indexBuffer, dstOffset, dataSize, indicesPtr);
		}

		void setDstOffset(const vk::DeviceSize& dstOffset) {
			LockGuard lock(GMutex);
			this->dstOffset = dstOffset;
		}

		void setDataSize(const vk::DeviceSize& dataSize) {
			LockGuard lock(GMutex);
			this->dataSize = dataSize;
		}

		void setIndicesPtr(void* indicesPtr) {
			LockGuard lock(GMutex);
			this->indicesPtr = indicesPtr;
		}

	private:
		vk::DeviceSize dstOffset = 0;
		vk::DeviceSize dataSize = 0;

		void* indicesPtr = nullptr;

		const vk::CommandBuffer commandBuffer;
		const vk::Buffer indexBuffer;

	private:
		friend class Commands;//call

	};

	template<>
	struct CommandElement<CommandType::TransitionImageLayout> final {
	public:
		explicit CommandElement(ManagerPassKey, const Data::TransitionImageLayoutData& data, const UI32 index)
			: commandBuffer(data.get<FvkType::CommandBuffer>().commandBuffers.at(index)),
			image(data.get<FvkType::Image>().image)
		{}
		~CommandElement() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(CommandElement);

	private:
		void call() const noexcept {
			vk::ImageSubresourceRange range = {
				.aspectMask = vk::ImageAspectFlagBits::eColor,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1,
			};
			vk::ImageMemoryBarrier barrier = {
				.oldLayout = vk::ImageLayout::eUndefined,
				.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal,
				.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.image = this->image,
				.subresourceRange = range
			};

			vk::PipelineStageFlags sourceStage;
			vk::PipelineStageFlags destinationStage;

			barrier.srcAccessMask = vk::AccessFlagBits();
			barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

			sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
			destinationStage = vk::PipelineStageFlagBits::eFragmentShader;

			commandBuffer.pipelineBarrier(
				sourceStage,
				destinationStage,
				vk::DependencyFlags(),
				nullptr,
				nullptr,
				barrier
			);

		}

	private:
		const vk::CommandBuffer commandBuffer ;
		const vk::Image image ;

	private:
		friend class Commands;//call

	};

	template<>
	struct CommandElement<CommandType::QueueSubmit> final {
	public:
		explicit CommandElement(ManagerPassKey, const Data::QueueSubmitData& data, const UI32 index)
			: commandBuffer(data.get<FvkType::CommandBuffer>().commandBuffers.at(index))
			//graphicsQueue(data.get<FvkType::Queue>().queue)
		{}
		~CommandElement() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(CommandElement);

	private:
		//strong
		void call() const {
			vk::SubmitInfo submitInfo = {
				.commandBufferCount = 1,
				.pCommandBuffers = &commandBuffer,
			};
			auto result = graphicsQueue.submit(1, &submitInfo, nullptr);

			if (result != vk::Result::eSuccess)
				Exception::throwUnexpected();
		}

	private:
		const vk::CommandBuffer commandBuffer;
		const vk::Queue graphicsQueue;

	private:
		friend class Commands;//call

	};

	template<>
	struct CommandElement<CommandType::Next> final {
	public:
		explicit CommandElement(ManagerPassKey, const Data::NextData& data, const UI32 index) {}
		~CommandElement() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(CommandElement);

	private:
		void call() {}

	private:
		friend class Commands;//call

	};

}

namespace FVK::Internal::Command {

	template<FvkType ...T>
	class AnyCommandElement final {
	private:
		//from Manager
		using InfoTuple = std::tuple<Data::CorrespondenceType<T>...>;

	public:
		//Receive at Info and copy what we need.
		using ArgTuple = std::tuple<Data::CorrespondenceVulkanObject<T>...>;

	public:
		void setFunction(void(*func)(const ArgTuple&, void*)) {
			LockGuard lock(GMutex);
			this->func = func;
		}

		void setUserData(void* data) {
			LockGuard lock(GMutex);
			this->userData = data;
		}

	private:
		void(*func)(const ArgTuple&, void*) = [](const ArgTuple&, void*) {};
		void* userData = nullptr;

	public:
		explicit AnyCommandElement(ManagerPassKey, const InfoTuple& info)
			: data(infoTupleToArgTuple(info, std::make_index_sequence<sizeof...(T)>()))
		{}
		~AnyCommandElement() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(AnyCommandElement);

	private:
		const ArgTuple data;

	public:
		void call() {
			this->func(this->data, this->userData);
		}

	private:
		template<Size Index>
		auto infoTupleToArgTupleHelper(const InfoTuple& info) {
			using Elm = std::tuple_element_t<Index, InfoTuple>;
			if constexpr (std::same_as<Elm, Data::CorrespondenceType<FvkType::LogicalDevice>>) {
				return std::get<Index>(info).get().device;
			}
			else if constexpr (std::same_as<Elm, Data::CorrespondenceType<FvkType::Queue>>) {
				return std::get<Index>(info).get().queue;
			}
			else if constexpr (std::same_as<Elm, Data::CorrespondenceType<FvkType::Queue_Vector>>) {
				const auto size = std::get<Index>(info).size();
				std::vector<vk::Queue> result(size);
				for (Size i = 0; i < size; i++)
					result[i] = std::get<Index>(info).at(i).get().queue;
				return result;
			}
			else if constexpr (std::same_as<Elm, Data::CorrespondenceType<FvkType::Swapchain>>) {
				return std::get<Index>(info).get().swapchain;
			}
			else if constexpr (std::same_as<Elm, Data::CorrespondenceType<FvkType::DeviceMemory>>) {
				return std::get<Index>(info).get().deviceMemory;
			}
			else if constexpr (std::same_as<Elm, Data::CorrespondenceType<FvkType::UniformBuffer>>) {
				return std::make_pair(std::get<Index>(info).get().uniformBuffer, std::get<Index>(info).get().uniformBufferMemory);
			}
			else if constexpr (std::same_as<Elm, Data::CorrespondenceType<FvkType::CommandBuffer>>) {
				return std::get<Index>(info).get().commandBuffers;
			}
			else if constexpr (std::same_as<Elm, Data::CorrespondenceType<FvkType::Semaphore_Vector>>) {
				const auto size = std::get<Index>(info).size();
				std::vector<vk::Semaphore> result(size);
				for (Size i = 0; i < size; i++)
					result[i] = std::get<Index>(info).at(i).get().semaphore;
				return result;
			}
			else if constexpr (std::same_as<Elm, Data::CorrespondenceType<FvkType::Fence_Vector>>) {
				const auto size = std::get<Index>(info).size();
				std::vector<vk::Fence> result(size);
				for (Size i = 0; i < size; i++)
					result[i] = std::get<Index>(info).at(i).get().fence;
				return result;
			}

		}

		template<Size...Index>
		ArgTuple infoTupleToArgTuple(const InfoTuple& info, std::index_sequence<Index...>) {
			return std::make_tuple(infoTupleToArgTupleHelper<Index>(info)...);
		}

	private:
		friend class ::FVK::Internal::Manager::Manager;

	};

}
