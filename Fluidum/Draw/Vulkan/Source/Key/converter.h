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
					return RequiredDataKeysBase<Type>::toOrderKey(std::get<0>(var));
				else if (var.index() == 1)
					return RequiredDataKeysBase<Type>::toOrderKey(std::get<1>(var));
				else if (var.index() == 2)
					return RequiredDataKeysBase<Type>::toOrderKey(std::get<2>(var));
				else //SelectKeyType
					return RequiredDataKeysBase<Type>::toOrderKey(std::get<3>(var));
			};

			return RequiredDataKeysBase<Type>(func.operator() < T > (vars)...);
		}

		//VariantKey��variant<vector>
		template<CommandType Type, typename... T>
		_NODISCARD static auto variantKeysToConnectionCommandKeysBase(const T&... var) {

			auto func = []<IsVariantKey U>(const U & var)->std::vector<OrderKey> {
				if (var.index() == 0)
					return ConnectionCommandKeysBase<Type>::toOrderKey(std::get<0>(var));
				else if (var.index() == 1)
					return ConnectionCommandKeysBase<Type>::toOrderKey(std::get<1>(var));
				else if (var.index() == 2)
					return ConnectionCommandKeysBase<Type>::toOrderKey(std::get<2>(var));
				else //SelectKeyType
					return ConnectionCommandKeysBase<Type>::toOrderKey(std::get<3>(var));
			};

			return ConnectionCommandKeysBase<Type>(func.operator() < T > (var)...);
		}


		//T��Key
		template<FvkType Type, IsKeyType ChangeKeyType, typename T>
		_NODISCARD static auto changeKey(const T& key) {
			using KeyType = T::KeyType;

			//CharKey��ϊ�
			if constexpr (std::same_as<KeyType, CharKeyType>) {
				//StringKey��
				if constexpr (std::same_as<ChangeKeyType, StringKeyType>) {
					return Key<Type, StringKeyType>(key);
				}
			}
			//IndexKey��ϊ�
			else if constexpr (std::same_as<KeyType, IndexKeyType>) {
				//StringKey��
				if constexpr (std::same_as<ChangeKeyType, StringKeyType>) {
					return Key<Type, StringKeyType>(GKeyManager.toStrKey(key));
				}
			}
			//StringKey��ϊ�
			else if constexpr (std::same_as<KeyType, StringKeyType>) {
				//StringKey��
				if constexpr (std::same_as<ChangeKeyType, StringKeyType>) {
					return key;
				}
			}

			//���̂�������
		}

		//T��Key
		template<FvkType Type, IsKeyType ChangeKeyType, IsKeyType KeyType>
		_NODISCARD static ChangeKeyType changeKeyType(const KeyType& key) {

			//CharKey��ϊ�
			if constexpr (std::same_as<KeyType, CharKeyType>) {
				//StringKey��
				if constexpr (std::same_as<ChangeKeyType, StringKeyType>) {
					return StringKeyType(key);
				}
			}
			//IndexKey��ϊ�
			else if constexpr (std::same_as<KeyType, IndexKeyType>) {
				//StringKey��
				if constexpr (std::same_as<ChangeKeyType, StringKeyType>) {
					return StringKeyType(GKeyManager.toStrKey(key));
				}
			}
			//StringKey��ϊ�
			else if constexpr (std::same_as<KeyType, StringKeyType>) {
				//StringKey��
				if constexpr (std::same_as<ChangeKeyType, StringKeyType>) {
					return key;
				}
			}

			//���̂�������
		}


		template<IsKeyType ChangeKeyType, typename T>
		_NODISCARD static ChangeKeyType keyToKeyType(const T& key) {

			//CharKey��ϊ�
			if constexpr (std::same_as<T::KeyType, CharKeyType>) {
				//StringKey��
				if constexpr (std::same_as<ChangeKeyType, StringKeyType>) {
					return StringKeyType(key);
				}
				//IndexKey
				else if constexpr (std::same_as<ChangeKeyType, IndexKeyType>) {
					return GKeyManager.toIndexKey(key.get());
				}
			}
			//IndexKey��ϊ�
			else if constexpr (std::same_as<T::KeyType, IndexKeyType>) {
				//StringKey��
				if constexpr (std::same_as<ChangeKeyType, StringKeyType>) {
					return StringKeyType(GKeyManager.toStrKey(key));
				}
				//IndexKey
				else if constexpr (std::same_as<ChangeKeyType, IndexKeyType>) {
					return key.get();
				}
			}
			//StringKey��ϊ�
			else if constexpr (std::same_as<T::KeyType, StringKeyType>) {
				//StringKey��
				if constexpr (std::same_as<ChangeKeyType, StringKeyType>) {
					return key;
				}
				//IndexKey
				else if constexpr (std::same_as<ChangeKeyType, IndexKeyType>) {
					return GKeyManager.toIndexKey(key.get());
				}
			}

			//���̂�������
		}

		template<FvkType FType>
		struct ObjectKeyTypeHelper final {
			template<IsKeyType KType>
			using Type = Key<FType, KType>;
		};

		//������variant����variant<vector<Key<Type,SringKey>>,...>
		template<FvkType Type, IsVariantKey T>
		_NODISCARD static auto variantKeysToVariantStrinKeyTypeVectorKey(const std::vector<T>& vars) {

			auto func = [&]<IsVariantKey U>(const U & var)-> StringKeyType {
				if (var.index() == 0)
					return StringKeyType(std::get<0>(var));
				else if (var.index() == 1)
					return StringKeyType(std::get<1>(var));
				else if (var.index() == 2)//Indexkey
					return GKeyManager.toStrKey(Type, std::get<2>(var));
				else //SelectKeyType
					return StringKeyType();// d::get<3>(var));
			};

			using Result = VariantVectorKey<ObjectKeyTypeHelper<Type>::Type>;
			using ValueType = std::vector<Key<Type, StringKeyType>>;

			ValueType result;

			for (std::size_t i = 0, size = vars.size(); i < size; i++) {
				result.emplace_back(Key<Type, StringKeyType>(func(vars[i])));
			}
			return Result(std::move(result));
		}


		template</*IsKey �R���p�C���o�O*/typename T>
		_NODISCARD static IndexKey toIndexKey(const T& key) {
			if constexpr (std::same_as<T::KeyType, IndexKeyType>)
				return GKeyManager.toIndexKey(T::Type, key);
			else
				return GKeyManager.toIndexKey(key);
		}

		template<typename T>
		_NODISCARD static std::vector<IndexKey> toIndexKeyVector(const T& key) {
			if constexpr (FU::Concept::IsStdVector<T>) {
				std::vector<IndexKey> result(key.size());
				for (std::size_t i = 0, size = key.size(); i < size; i++) {
					result[i] = keyToKeyType<IndexKey>(key[i]);
				}
				return result;
			}
			else {
				return { keyToKeyType<IndexKey>(key) };
			}
		}

		template<typename T>
		_NODISCARD static StringKeyType keyToStringKeyType(const T& key) {
			changeKeyType<T::Type, StringKeyType>(key.get());
		}

		//Type�ɗ~����FvkType������
		template<FvkType Type, typename T>
		_NODISCARD static IndexKey keyToConnectionIndexKeyType(const T& key) {
			return GKeyManager.toConnectionIndexKey(key.get(), Type);
		}

		template<FvkType Type, IsVariantKey T>
		_NODISCARD static auto variantKeyToIndexKey(const T& var) {

			if (var.index() == 0)
				return Key<Type, IndexKey>(keyToKeyType<IndexKeyType>(std::get<0>(var)));
			else if (var.index() == 1)
				return Key<Type, IndexKey>(keyToKeyType<IndexKeyType>(std::get<1>(var)));
			else if (var.index() == 2)
				return Key<Type, IndexKey>(keyToKeyType<IndexKeyType>(std::get<2>(var)));
			else //SelectKeyType
				return Key<Type, IndexKey>(UINT32_MAX);
		}


	};


}