#pragma once

#include "variant.h"

namespace FVK::Internal::Key {

	class Converter final {
	public:
		//VariantKey or variant<vector>
		template<FvkType Type, typename... T>
		[[nodiscard]] static auto variantKeysToConnectionKeysBase(const T&... vars) {

			auto func = []<typename U>(const U & var)->std::vector<OrderKey> {
				if (var.index() == 0)
					return RequiredDataKeys<Type>::toOrderKey(std::get<0>(var));
				else if (var.index() == 1)
					return RequiredDataKeys<Type>::toOrderKey(std::get<1>(var));
				else if (var.index() == 2)
					return RequiredDataKeys<Type>::toOrderKey(std::get<2>(var));
				else //SelectKeyType
					return RequiredDataKeys<Type>::toOrderKey(std::get<3>(var));
			};

			return RequiredDataKeys<Type>(func.operator() < T > (vars)...);
		}

		//VariantKey or variant<vector>
		template<FvkType Type, typename... T>
		[[nodiscard]] static auto keysToConnectionKeys(const T&... keys) {
			return RequiredDataKeys<Type>(RequiredDataKeys<Type>::toOrderKey(keys)...);
		}

		//T is Key
		template<FvkType Type, IsKeyType ChangeKeyType, typename T>
		[[nodiscard]] static auto changeKey(const T& key) {
			using KeyType = T::KeyType;

			//CharKey‚ğ•ÏŠ·
			if constexpr (std::same_as<KeyType, CharKey>) {
				//StringKey‚É
				if constexpr (std::same_as<ChangeKeyType, StrKey>) {
					return Key<Type, StrKey>(key);
				}
			}
			//IndexKey‚ğ•ÏŠ·
			else if constexpr (std::same_as<KeyType, IndexKey>) {
				//StringKey‚É
				if constexpr (std::same_as<ChangeKeyType, StrKey>) {
					return Key<Type, StrKey>(GKeyManager.toStrKey(key));
				}
			}
			//StringKey‚ğ•ÏŠ·
			else if constexpr (std::same_as<KeyType, StrKey>) {
				//StringKey‚É
				if constexpr (std::same_as<ChangeKeyType, StrKey>) {
					return key;
				}
			}

			//‚»‚Ì‚½–¢À‘•
		}

		//T‚ÍKey
		template<FvkType Type, IsKeyType ChangeKeyType, IsKeyType KeyType>
		[[nodiscard]] static ChangeKeyType changeKeyType(const KeyType& key) {

			//CharKey‚ğ•ÏŠ·
			if constexpr (std::same_as<KeyType, CharKey>) {
				//StringKey‚É
				if constexpr (std::same_as<ChangeKeyType, StrKey>) {
					return StrKey(key);
				}
			}
			//IndexKey‚ğ•ÏŠ·
			else if constexpr (std::same_as<KeyType, IndexKey>) {
				//StringKey‚É
				if constexpr (std::same_as<ChangeKeyType, StrKey>) {
					return StrKey(GKeyManager.toStrKey(key));
				}
			}
			//StringKey‚ğ•ÏŠ·
			else if constexpr (std::same_as<KeyType, StrKey>) {
				//StringKey‚É
				if constexpr (std::same_as<ChangeKeyType, StrKey>) {
					return key;
				}
			}

			//‚»‚Ì‚½–¢À‘•
		}


		template<IsKeyType ChangeKeyType, typename T>
		[[nodiscard]] static ChangeKeyType keyToKeyType(const T& key) {

			//CharKey
			if constexpr (std::same_as<T::KeyType, CharKey>) {
				//StringKey
				if constexpr (std::same_as<ChangeKeyType, StrKey>) {
					return StrKey(key);
				}
				//IndexKey
				else if constexpr (std::same_as<ChangeKeyType, IndexKey>) {
					return GKeyManager.toIndexKey(key.get());
				}
			}
			//IndexKey‚ğ•ÏŠ·
			else if constexpr (std::same_as<T::KeyType, IndexKey>) {
				//StringKey‚É
				if constexpr (std::same_as<ChangeKeyType, StrKey>) {
					return StrKey(GKeyManager.toStrKey(key));
				}
				//IndexKey
				else if constexpr (std::same_as<ChangeKeyType, IndexKey>) {
					return key.get();
				}
			}
			//StringKey‚ğ•ÏŠ·
			else if constexpr (std::same_as<T::KeyType, StrKey>) {
				//StringKey‚É
				if constexpr (std::same_as<ChangeKeyType, StrKey>) {
					return key;
				}
				//IndexKey
				else if constexpr (std::same_as<ChangeKeyType, IndexKey>) {
					return GKeyManager.toIndexKey(key.get());
				}
			}

			//‚»‚Ì‚½–¢À‘•
		}

		template<FvkType FType>
		struct ObjectKeyTypeHelper final {
			template<IsKeyType KType>
			using Type = Key<FType, KType>;
		};

		//•¡”‚Ìvariant‚©‚çvariant<vector<Key<Type,SringKey>>,...>
		template<FvkType Type, IsVariantKey T>
		[[nodiscard]] static auto variantKeysToVariantStrinKeyTypeVectorKey(const std::vector<T>& vars) {

			auto func = [&]<IsVariantKey U>(const U & var)-> StrKey {
				if (var.index() == 0)
					return StrKey(std::get<0>(var));
				else if (var.index() == 1)
					return StrKey(std::get<1>(var));
				else if (var.index() == 2)//Indexkey
					return GKeyManager.toStrKey(Type, std::get<2>(var));
				else //SelectKeyType
					return StrKey();// d::get<3>(var));
			};

			using Result = VariantVectorKey<ObjectKeyTypeHelper<Type>::Type>;
			using ValueType = std::vector<Key<Type, StrKey>>;

			ValueType result;

			for (std::size_t i = 0, size = vars.size(); i < size; i++) {
				result.emplace_back(Key<Type, StrKey>(func(vars[i])));
			}
			return Result(std::move(result));
		}

		/*
		Exception;
			NotFound
			Unexpected
		*/
		//strong
		//Get indexkey from the key.
		template<::FVK::Internal::Key::IsKey T>
		[[nodiscard]] static IndexKey toIndexKey(const T& key) {
			try {
				if constexpr (std::same_as<T::KeyType, IndexKey> || std::same_as<T::KeyType, SelectKey>)
					return GKeyManager.toIndexKey(T::Type, key);//strong
				else
					return GKeyManager.toIndexKey(key);//strong
			}
			catch (const std::exception&) {
				Exception::throwUnexpected();
			}
			catch (...) {
				Exception::throwNotFound();
			}
		}

		template<typename T>
		[[nodiscard]] static std::vector<IndexKey> toIndexKeyVector(const T& key) {
			if constexpr (FU::Concept::IsStdVector<T>) {
				std::vector<IndexKey> result(key.size());
				for (Size i = 0, size = key.size(); i < size; i++) {
					result[i] = keyToKeyType<IndexKey>(key[i]);
				}
				return result;
			}
			else {
				return { keyToKeyType<IndexKey>(key) };
			}
		}

		template<typename T>
		[[nodiscard]] static StrKey keyToStringKeyType(const T& key) {
			changeKeyType<T::Type, StrKey>(key.get());
		}

		//Type‚É—~‚µ‚¢FvkType‚ğ“ü‚ê‚é
		template<FvkType Type, typename T>
		[[nodiscard]] static IndexKey keyToConnectionIndexKeyType(const T& key) {
			return GKeyManager.toConnectionIndexKey(key.get(), Type);
		}

		template<FvkType Type, IsVariantKey T>
		[[nodiscard]] static auto variantKeyToIndexKey(const T& var) {
			if (var.index() == 0)
				return Key<Type, IndexKey>(keyToKeyType<IndexKey>(std::get<0>(var)));
			else if (var.index() == 1)
				return Key<Type, IndexKey>(keyToKeyType<IndexKey>(std::get<1>(var)));
			else if (var.index() == 2)
				return Key<Type, IndexKey>(keyToKeyType<IndexKey>(std::get<2>(var)));
			else //SelectKeyType
				return Key<Type, IndexKey>(UINT32_MAX);
		}

	};

}