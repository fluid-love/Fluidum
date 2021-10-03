#pragma once

#include "../using.h"

namespace FVK::Internal::Key {

	//各アイテムの作成に必要なデータのkeyをOrderKey入れる
	template<FvkType Type>
	class RequiredDataKeysBase {
	private:
		static constexpr inline auto Size = GetRequiredDataSize<Type>();
		const std::array<std::vector<OrderKey>, Size> order;

	protected:
		template</*IsKeyコンパイラバグ*/typename... T> requires(Size == sizeof...(T) && !FU::Concept::IsSame<std::vector<OrderKey>, T...>)
			RequiredDataKeysBase(const T&... arg)
			: order{ this->toOrderKey<T>(arg)... }
		{}

		//OrderKey...
		template<typename... T> requires(Size == sizeof...(T) && FU::Concept::IsSame<std::vector<OrderKey>, T...>)
			explicit RequiredDataKeysBase(const T&... arg)
			: order{ arg... }
		{}

		RequiredDataKeysBase()
			: order({})
		{
			assert(Size == 0);
		}
	private:

		template<typename T>
		static OrderKey toOrderKeyHelper(const T& key) {
			if constexpr (std::same_as<typename T::KeyType, IndexKey>)
				return GKeyManager.toOrderKey(T::Type, key.get());
			else if constexpr (std::same_as<typename T::KeyType, OrderKey>)
				return key;
			else if constexpr (std::same_as<typename T::KeyType, CharKeyType>)
				return GKeyManager.toOrderKey(key.get());
			else if constexpr (std::same_as<typename T::KeyType, StringKeyType>)
				return GKeyManager.toOrderKey(key.get());
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
					result[i] = toOrderKeyHelper<typename T::value_type>(key[i]);
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

}

namespace FVK::Internal::Key {

	template<FvkType Type>
	struct RequiredDataKeys final : public RequiredDataKeysBase<FvkType::None> {
		FluidumUtils_Class_Delete_ConDestructor(RequiredDataKeys)
	};

	template<>
	struct RequiredDataKeys<FvkType::Window> final : public RequiredDataKeysBase<FvkType::Window> {
		RequiredDataKeys() : RequiredDataKeysBase() {}
		friend ::FVK::Internal::Manager::Manager;
	};
	template<>
	struct RequiredDataKeys<FvkType::Instance> final : public RequiredDataKeysBase<FvkType::Instance> {
		RequiredDataKeys() : RequiredDataKeysBase() {}
		friend ::FVK::Internal::Manager::Manager;
	};

	template<>
	struct RequiredDataKeys<FvkType::Messenger> final : public RequiredDataKeysBase<FvkType::Messenger> {
		template<IsKeyType T1>
		explicit RequiredDataKeys(
			const InstanceKey<T1>& instance
		) : RequiredDataKeysBase(instance)
		{}
		friend ::FVK::Internal::Manager::Manager;
	};

	template<>
	struct RequiredDataKeys<FvkType::Surface> final : public RequiredDataKeysBase<FvkType::Surface> {
		template<IsKeyType T1, IsKeyType T2>
		explicit RequiredDataKeys(
			const WindowKey<T1>& window,
			const InstanceKey<T2>& instance
		) : RequiredDataKeysBase(window, instance)
		{}
		friend ::FVK::Internal::Manager::Manager;
	};

	template<>
	struct RequiredDataKeys<FvkType::PhysicalDevice> final : public RequiredDataKeysBase<FvkType::PhysicalDevice> {
		template<IsKeyType T1>
		explicit RequiredDataKeys(
			const InstanceKey<T1>& instance
		) : RequiredDataKeysBase(instance)
		{}
		friend ::FVK::Internal::Manager::Manager;
	};
	template<>
	struct RequiredDataKeys<FvkType::PhysicalDevice_Swapchain> final : public RequiredDataKeysBase<FvkType::PhysicalDevice_Swapchain> {
		template<IsKeyType T1, IsKeyType T2>
		explicit RequiredDataKeys(
			const SurfaceKey<T2>& surface
		) : RequiredDataKeysBase(surface)
		{}
		friend ::FVK::Internal::Manager::Manager;
	};

	template<>
	struct RequiredDataKeys<FvkType::LogicalDevice> final : public RequiredDataKeysBase<FvkType::LogicalDevice> {
		template<IsKeyType T1>
		explicit RequiredDataKeys(
			const PhysicalDeviceKey<T1>& physicalDevice
		) : RequiredDataKeysBase(physicalDevice)
		{}
		friend ::FVK::Internal::Manager::Manager;
	};

	template<>
	struct RequiredDataKeys<FvkType::Swapchain> final : public RequiredDataKeysBase<FvkType::Swapchain> {
		template<IsKeyType T1, IsKeyType T2>
		explicit RequiredDataKeys(
			const SurfaceKey<T1>& surface,
			const LogicalDeviceKey<T2>& logicalDevice
		) : RequiredDataKeysBase(surface, logicalDevice)
		{}
		friend ::FVK::Internal::Manager::Manager;
	};

	template<>
	struct RequiredDataKeys<FvkType::RenderPass> final : public RequiredDataKeysBase<FvkType::RenderPass> {
		template<IsKeyType T1>
		explicit RequiredDataKeys(
			const LogicalDeviceKey<T1>& logicalDevice
		) : RequiredDataKeysBase(logicalDevice)
		{}
		friend ::FVK::Internal::Manager::Manager;
	};

	template<>
	struct RequiredDataKeys<FvkType::DescriptorSetLayout> final : public RequiredDataKeysBase<FvkType::DescriptorSetLayout> {
		template<IsKeyType T1>
		explicit RequiredDataKeys(
			const LogicalDeviceKey<T1>& logicalDevice
		) : RequiredDataKeysBase(logicalDevice)
		{}
		friend ::FVK::Internal::Manager::Manager;
	};

	template<>
	struct RequiredDataKeys<FvkType::ShaderModule> final : public RequiredDataKeysBase<FvkType::ShaderModule> {
		template<IsKeyType T1>
		explicit RequiredDataKeys(
			const LogicalDeviceKey<T1>& logicalDevice
		) : RequiredDataKeysBase(logicalDevice)
		{}
		friend ::FVK::Internal::Manager::Manager;
	};

	template<>
	struct RequiredDataKeys<FvkType::GraphicsPipelineLayout> final : public RequiredDataKeysBase<FvkType::GraphicsPipelineLayout> {
		template<IsKeyType T1, IsKeyType T2>
		explicit RequiredDataKeys(
			const LogicalDeviceKey<T1>& logicalDevice,
			const DescriptorSetLayoutKey<T2>& descriptorSetLayout
		) : RequiredDataKeysBase(logicalDevice)
		{}
		friend ::FVK::Internal::Manager::Manager;
	};

	template<>
	struct RequiredDataKeys<FvkType::GraphicsPipeline> final : public RequiredDataKeysBase<FvkType::GraphicsPipeline> {
		template<IsKeyType T1, IsKeyType T2>
		explicit RequiredDataKeys(
			const LogicalDeviceKey<T1>& logicalDevice,
			const DescriptorSetLayoutKey<T2>& descriptorSetLayout
		) : RequiredDataKeysBase(logicalDevice)
		{}
		friend ::FVK::Internal::Manager::Manager;
	};


}