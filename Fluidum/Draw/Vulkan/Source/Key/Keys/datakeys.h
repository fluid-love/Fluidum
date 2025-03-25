#pragma once

#include "../using.h"

namespace FVK::Internal::Key {

	template<FvkType Type>
	class RequiredDataKeys final {
	private:
		static constexpr inline auto N = GetRequiredDataSize<Type>();
		const std::array<std::vector<OrderKey>, N> order;

	private:
		template<IsKey... T> requires(N == sizeof...(T) && !FU::Concept::IsSame<std::vector<OrderKey>, T...>)
			explicit RequiredDataKeys(const T&... arg)
			: order{ this->toOrderKey<T>(arg)... }
		{}

		//OrderKey...
		template<typename... T> requires(N == sizeof...(T) && FU::Concept::IsSame<std::vector<OrderKey>, T...>)
			explicit RequiredDataKeys(const T&... arg)
			: order{ arg... }
		{}

		RequiredDataKeys() noexcept
			: order({})
		{
			assert(N == 0);
		}

	private:
		/*
		Exception:
			std::exception
			FKM::NotFound
		*/
		//strong
		template<typename T>
		static OrderKey toOrderKeyHelper(const T& key) {
			if constexpr (std::same_as<typename T::KeyType, IndexKey>)
				return GKeyManager.toOrderKey(T::Type, key.get());
			else if constexpr (std::same_as<typename T::KeyType, OrderKey>)
				return GKeyManager.toOrderKey(key);
			else if constexpr (std::same_as<typename T::KeyType, CharKey>)
				return GKeyManager.toOrderKey(key.get());
			else if constexpr (std::same_as<typename T::KeyType, StrKey>)
				return GKeyManager.toOrderKey(key.get());
			else//SelectKey
				return OrderKey(std::numeric_limits<OrderKey::KeyType>::max());//–¢ŽÀ‘• GKeyManager.toOrderKey(key);
		}

		//T is vector<Key> or Key
		template<typename T>
		static std::vector<OrderKey> toOrderKey(const T& key) {

			//vector
			if constexpr (FU::Concept::IsStdVector<T>) {
				std::vector<OrderKey> result(key.size());
				for (Size i = 0, size = key.size(); i < size; i++) {
					result[i] = toOrderKeyHelper<typename T::value_type>(key[i]);
				}
				return result;
			}
			else {//Key
				return { toOrderKeyHelper<T>(key) };
			}
		}

	private:
		//strong
		[[nodiscard]] std::array<std::vector<IndexKey>, N> convert() const {
			if constexpr (N == 0) {
				return {};
			}
			else {
				std::array<std::vector<IndexKey>, N> result{};
				for (Size i = 0; i < N; i++) {
					std::vector<IndexKey> temp(order[i].size());
					for (Size j = 0, size = order[i].size(); j < size; j++) {
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
