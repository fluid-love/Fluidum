#pragma once

#include "../using.h"

namespace FVK::Internal::Key {

	template<CommandType Type>
	class ConnectionCommandKeysBase {
	private:
		static constexpr inline auto Size = GetCommandDataTypesSize<Type>();
		const std::array<std::vector<OrderKey>, Size> order;

	protected:
		template</*IsKeyコンパイラバグ*/typename... T> requires(Size == sizeof...(T) && !FU::Concept::IsSame<std::vector<OrderKey>, T...>)
			ConnectionCommandKeysBase(const T&... arg)
			: order{ this->toOrderKey<T>(arg)... }
		{}

		//OrderKey...
		template<typename... T> requires(Size == sizeof...(T) && FU::Concept::IsSame<std::vector<OrderKey>, T...>)
			explicit ConnectionCommandKeysBase(const T&... arg)
			: order{ arg... }
		{}

		ConnectionCommandKeysBase()
			: order({})
		{
			assert(Size == 0);
		}
	private:

		template<typename T>
		static OrderKey toOrderKeyHelper(const T& key) {
			if constexpr (std::same_as<typename T::KeyType, IndexKey>)
				return GKeyManager.toOrderKey(T::Type, key);
			else if constexpr (std::same_as<typename T::KeyType, OrderKey>)
				return key;
			else if constexpr (std::same_as<typename T::KeyType, const char*>)
				return GKeyManager.toOrderKey(key);
			else if constexpr (std::same_as<typename T::KeyType, std::string>)
				return GKeyManager.toOrderKey(key);
			else//SelectKey
				return OrderKey(std::numeric_limits<OrderKey::Type>::max());//未実装 GKeyManager.toOrderKey(key);
		}

		//一度通し番号に変換しておいて使う時に戻す
		//Tはvector<Key>かKey
		template<typename T>
		static std::vector<OrderKey> toOrderKey(const T& key) {

			//keyがvectorできた場合
			if constexpr (FU::Concept::IsStdVector<T>) {
				std::vector<OrderKey> result(key.size());
				for (std::size_t i = 0, size = key.size(); i < size; i++) {
					result[i] = toOrderKeyHelper<T::value_type>(key[i]);
				}
				return result;
			}
			else {//単発
				return { toOrderKeyHelper<T>(key) };
			}
		}

	protected:

		_NODISCARD std::array<std::vector<IndexKey>, Size> convert() const {
			if constexpr (Size == 0) {
				return {};
			}
			else {
				std::array<std::vector<IndexKey>, Size> result = {};
				for (std::size_t i = 0; i < Size; i++) {
					std::vector<IndexKey> temp(order[i].size());
					for (std::size_t j = 0, size = order[i].size(); j < size; j++) {
						temp[j] = GKeyManager.toIndexKey(order[i][j]);
					}
					result[i] = std::move(temp);
				}

				return result;
			}
		}


	private:
		friend class ::FVK::Internal::Key::Converter;
		friend class ::FVK::Internal::Manager::Manager;

	};

	template<CommandType>
	struct ConnectionCommandKeys final {
		FluidumUtils_Class_Delete_ConDestructor(ConnectionCommandKeys)
	};

	template<>
	struct ConnectionCommandKeys<CommandType::BeginCommandBuffer> final : public ConnectionCommandKeysBase<CommandType::BeginCommandBuffer> {
		template<IsKeyType T1>
		explicit ConnectionCommandKeys(
			const CommandBufferKey<T1>& commandBuffer
		) : ConnectionCommandKeysBase(commandBuffer)
		{}
		friend ::FVK::Internal::Manager::Manager;
	};

	template<>
	struct ConnectionCommandKeys<CommandType::EndCommandBuffer> final : public ConnectionCommandKeysBase<CommandType::EndCommandBuffer> {
		template<IsKeyType T1>
		explicit ConnectionCommandKeys(
			const CommandBufferKey<T1>& commandBuffer
		) : ConnectionCommandKeysBase(commandBuffer)
		{}
		friend ::FVK::Internal::Manager::Manager;
	};

	template<>
	struct ConnectionCommandKeys<CommandType::BindVertexBuffers> final : public ConnectionCommandKeysBase<CommandType::BindVertexBuffers> {
		template<IsKeyType T1, IsKeyType T2>
		explicit ConnectionCommandKeys(
			const CommandBufferKey<T1>& commandBuffer,
			const VertexBufferKey<T2>& vertexBuffer
		) : ConnectionCommandKeysBase(commandBuffer, vertexBuffer)
		{}
		friend ::FVK::Internal::Manager::Manager;
	};

	template<>
	struct ConnectionCommandKeys<CommandType::BeginRenderPass> final : public ConnectionCommandKeysBase<CommandType::BeginRenderPass> {
		template<IsKeyType T1, IsKeyType T2, IsKeyType T3>
		explicit ConnectionCommandKeys(
			const CommandBufferKey<T1>& commandBuffer,
			const RenderPassKey<T2>& renderPass,
			const FrameBufferKey<T3>& frameBuffer

		) : ConnectionCommandKeysBase(commandBuffer, renderPass, frameBuffer)
		{}
		friend ::FVK::Internal::Manager::Manager;
	};

	template<>
	struct ConnectionCommandKeys<CommandType::BindDescriptorSet> final : public ConnectionCommandKeysBase<CommandType::BindDescriptorSet> {
		template<IsKeyType T1, IsKeyType T2, IsKeyType T3>
		explicit ConnectionCommandKeys(
			const CommandBufferKey<T1>& commandBuffer,
			const GraphicsPipelineLayoutKey<T2>& graphicsPipelineLayout,
			const DescriptorSetKey<T3>& descriptorSet

		) : ConnectionCommandKeysBase(commandBuffer, graphicsPipelineLayout, descriptorSet)
		{}
		friend ::FVK::Internal::Manager::Manager;
	};

	template<>
	struct ConnectionCommandKeys<CommandType::BindGraphicsPipeline> final : public ConnectionCommandKeysBase<CommandType::BindGraphicsPipeline> {
		template<IsKeyType T1, IsKeyType T2>
		explicit ConnectionCommandKeys(
			const CommandBufferKey<T1>& commandBuffer,
			const GraphicsPipelineKey<T2>& graphicsPipeline
		) : ConnectionCommandKeysBase(commandBuffer, graphicsPipeline)
		{}
		friend ::FVK::Internal::Manager::Manager;
	};

	template<>
	struct ConnectionCommandKeys<CommandType::BindIndexBuffer> final : public ConnectionCommandKeysBase<CommandType::BindIndexBuffer> {
		template<IsKeyType T1, IsKeyType T2>
		explicit ConnectionCommandKeys(
			const CommandBufferKey<T1>& commandBuffer,
			const IndexBufferKey<T2>& indexBuffer
		) : ConnectionCommandKeysBase(commandBuffer, indexBuffer)
		{}
		friend ::FVK::Internal::Manager::Manager;
	};

	template<>
	struct ConnectionCommandKeys<CommandType::DrawIndexed> final : public ConnectionCommandKeysBase<CommandType::DrawIndexed> {
		template<IsKeyType T1>
		explicit ConnectionCommandKeys(
			const CommandBufferKey<T1>& commandBuffer
		) : ConnectionCommandKeysBase(commandBuffer)
		{}
		friend ::FVK::Internal::Manager::Manager;
	};

	template<>
	struct ConnectionCommandKeys<CommandType::EndRenderPass> final : public ConnectionCommandKeysBase<CommandType::EndRenderPass> {
		template<IsKeyType T1>
		explicit ConnectionCommandKeys(
			const CommandBufferKey<T1>& commandBuffer
		) : ConnectionCommandKeysBase(commandBuffer)
		{}
		friend ::FVK::Internal::Manager::Manager;
	};

	template<>
	struct ConnectionCommandKeys<CommandType::SetScissor> final : public ConnectionCommandKeysBase<CommandType::SetScissor> {
		template<IsKeyType T1>
		explicit ConnectionCommandKeys(
			const CommandBufferKey<T1>& commandBuffer
		) : ConnectionCommandKeysBase(commandBuffer)
		{}
		friend ::FVK::Internal::Manager::Manager;
	};

	template<>
	struct ConnectionCommandKeys<CommandType::SetViewport> final : public ConnectionCommandKeysBase<CommandType::SetViewport> {
		template<IsKeyType T1>
		explicit ConnectionCommandKeys(
			const CommandBufferKey<T1>& commandBuffer
		) : ConnectionCommandKeysBase(commandBuffer)
		{}
		friend ::FVK::Internal::Manager::Manager;
	};

	template<>
	struct ConnectionCommandKeys<CommandType::ImGuiRenderDrawData> final : public ConnectionCommandKeysBase<CommandType::ImGuiRenderDrawData> {
		template<IsKeyType T1>
		explicit ConnectionCommandKeys(
			const CommandBufferKey<T1>& commandBuffer
		) : ConnectionCommandKeysBase(commandBuffer)
		{}
		friend ::FVK::Internal::Manager::Manager;
	};

	template<>
	struct ConnectionCommandKeys<CommandType::BindImGuiImage> final : public ConnectionCommandKeysBase<CommandType::BindImGuiImage> {
		template<IsKeyType T1, IsKeyType T2>
		explicit ConnectionCommandKeys(
			const CommandBufferKey<T1>& commandBuffer,
			const ImGuiImageKey<T2>& imguiImage
		) : ConnectionCommandKeysBase(commandBuffer, imguiImage)
		{}
		friend ::FVK::Internal::Manager::Manager;
	};

	template<>
	struct ConnectionCommandKeys<CommandType::UpdateVertexBuffer> final : public ConnectionCommandKeysBase<CommandType::UpdateVertexBuffer> {
		template<IsKeyType T1, IsKeyType T2>
		explicit ConnectionCommandKeys(
			const CommandBufferKey<T1>& commandBuffer,
			const VertexBufferKey<T2>& vertexBuffer
		) : ConnectionCommandKeysBase(commandBuffer, vertexBuffer)
		{}
		friend ::FVK::Internal::Manager::Manager;
	};

	template<>
	struct ConnectionCommandKeys<CommandType::UpdateIndexBuffer> final : public ConnectionCommandKeysBase<CommandType::UpdateIndexBuffer> {
		template<IsKeyType T1, IsKeyType T2>
		explicit ConnectionCommandKeys(
			const CommandBufferKey<T1>& commandBuffer,
			const IndexBufferKey<T2>& indexBuffer
		) : ConnectionCommandKeysBase(commandBuffer, indexBuffer)
		{}
		friend ::FVK::Internal::Manager::Manager;
	};

	template<>
	struct ConnectionCommandKeys<CommandType::TransitionImageLayout> final : public ConnectionCommandKeysBase<CommandType::TransitionImageLayout> {
		template<IsKeyType T1, IsKeyType T2>
		explicit ConnectionCommandKeys(
			const CommandBufferKey<T1>& commandBuffer,
			const ImageKey<T2>& image
		) : ConnectionCommandKeysBase(commandBuffer, image)
		{}
		friend ::FVK::Internal::Manager::Manager;
	};

	template<>
	struct ConnectionCommandKeys<CommandType::QueueSubmit> final : public ConnectionCommandKeysBase<CommandType::QueueSubmit> {
		template<IsKeyType T1, IsKeyType T2>
		explicit ConnectionCommandKeys(
			const CommandBufferKey<T1>& commandBuffer,
			const LogicalDeviceKey<T2>& logicalDevice
		) : ConnectionCommandKeysBase(commandBuffer, logicalDevice)
		{}
		friend ::FVK::Internal::Manager::Manager;
	};

	template<>
	struct ConnectionCommandKeys<CommandType::Next> final : public ConnectionCommandKeysBase<CommandType::Next> {
		explicit ConnectionCommandKeys()
			: ConnectionCommandKeysBase()
		{}
		friend ::FVK::Internal::Manager::Manager;
	};

}