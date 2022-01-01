#pragma once

#include "type.h"
#include "concept.h"
#include "../Common/fkm.h"

namespace FVK::Internal::Manager {

	class Manager final {
	public:
		Manager();
		~Manager();
		FluidumUtils_Class_Delete_CopyMove(Manager)

	public:
		/*
		Exception:
			std::exception
			CollisionOfKeys
			FailedToCreate
		*/
		//strong
		//if item don't need any other data.
		template<FvkType Type, IsParameter Parameter>
		void add(const char* key, Parameter& parameter) {
			static_assert(GetRequiredDataSize<Type>() == 0);

			//Same name is not allowed.
			if (GKeyManager.isKeyCollided(key))
				Exception::throwCollisionOfKeys(key);

			//No duplicate names guaranteed.
			try {
				//strong
				GKeyManager.add<ToHandleType<Type>()>(key);
			}
			catch (const FKM::Exception::Error& e) {
				if (e.code() == FKM::Exception::ErrorType::ReachIndexLimit) {
					GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "The maximum number of items that can be added has been exceeded.");
					Exception::throwFailedToCreate();
				}
				else {
					GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "");
					Exception::throwUnexpected();
				}
			}

			auto ptr = this->getItemVectorPtr<Type>();
			Data::Data<Type> requiredData({});

			//std::vector::value_type(Class of each item).
			using ValueType = std::tuple_element_t<CorrespondenceAt<Type>(), DataTuple>::value_type;

			//https://eel.is/c++draft/vector.modifiers
			//strong
			static_assert(std::is_nothrow_move_constructible_v<ValueType>);
			ptr->emplace_back(ValueType(ManagerPassKey{}, requiredData, parameter));
		}

		//追加する
		template<FvkType Type, IsParameter Parameter>
		void add(const char* key, Parameter& parameter, const Key::RequiredDataKeysBase<Type>& keys) {
			constexpr auto size = GetRequiredDataSize<Type>();
			static_assert(size > 0, "Size == 0は別の関数addを使う");
			//keyの被り
			if (Internal::GKeyManager.isKeyCollision(key))
				Exception::throwCollisionOfKeys(key);

			//connection
			const std::array<std::vector<IndexKey>, size> connections = keys.convert();

			//必要なデータ型 DataTypeArray
			constexpr auto dataTypes = GetRequiredDataTypes<Type>();


			/*必要なデータを入れる*/
			//参照で作るので注意　各クラスで保存しておかないこと erase addを挟まないこと
			auto filledData = fillData<dataTypes>(std::make_index_sequence<size>(), connections);
			Data::Data<Type> requiredData(std::move(filledData));

			auto ptr = getPtr<Type>();

			//std::vector::value_type 各アイテムのクラス
			using ValueType = std::tuple_element_t<CorrespondenceAt<Type>(), DataTuple>::value_type;
			ptr->emplace_back(ValueType(ManagerPassKey{}, requiredData, parameter));

			//keyを登録
			GKeyManager.add_recursive<ToObjectType<Type>()>(key, toKeyAddType<dataTypes>(connections));
		}

	public:
		/*
		Exception:
			std::exception
			CollisionOfKeys
			FailedToCreate
		*/
		//strong
		//if item don't need any other data.		
		template<FvkType Type, Key::IsKey T>
		void erase(const T& key) {
			//throw NotFound
			IndexKey index{};
			try {
				index = Key::Converter::toIndexKey(key);
			}
			catch () {

			}

			auto ptr = getPtr<Type>();

			if constexpr (IsRequireWaitIdle<Data::Info<Type>>) {
				vk::Device device = ptr->at(index).get().device;
				auto result = device.waitIdle();
				if (result != vk::Result::eSuccess) {
					throw std::runtime_error("Failed to waitIdle.");
				}
			}

			ptr->at(index).destroy();
			ptr->erase(ptr->cbegin() + index);

			//keyを削除
			GKeyManager.erase(Type, index);
		}

		//infoをconst参照で返す
		//addやeraseを挟むと無効なアドレスを指す可能性に注意
		template<FvkType Type, /*Key::IsKeyコンパイラのバグ*/typename K>
		_NODISCARD const auto& refInfo(const K& key) const {
			const IndexKey index = Key::Converter::toIndexKey(key);
			const auto data = getPtr<Type>();
			return data->at(index).get();
		}

		//itemをconst参照で返す
		//addやeraseを挟むと無効なアドレスを指す可能性に注意
		template<FvkType Type, /*Key::IsKeyコンパイラのバグ*/typename K>
		_NODISCARD const auto& refItem(const K& key) const {
			const IndexKey index = Key::Converter::toIndexKey(key);
			const auto data = getPtr<Type>();
			return data->at(index);
		}


		//Command作成
		template<CommandType Type>
		_NODISCARD auto makeCommand(const Key::ConnectionCommandKeysBase<Type>& keys, const uint32_t commandBufferIndex) {
			//必要なデータ型 DataTypeArray
			constexpr auto size = GetCommandDataTypesSize<Type>();
			constexpr auto dataTypes = GetCommandDataTypes<Type>();

			//connection
			const std::array<std::vector<IndexKey>, size> connections = keys.convert();

			//必要なデータを入れる
			auto filledData = fillData<dataTypes>(std::make_index_sequence<size>(), connections);
			Data::CommandData<Type> requiredData(std::move(filledData));

			using Element = Command::CommandElement<Type>;
			std::shared_ptr<Element> elementPtr(new Element(ManagerPassKey{}, requiredData, commandBufferIndex));
			return Command::Command<Type>(std::move(elementPtr));
		}

	private:
		template<typename T>
		static consteval FvkType AnyCommandKeyFvkType() {
			//std::vectorなら
			if constexpr (FU::Concept::IsStdVector<T>) {
				return ToObjectVectorType<T::value_type::Type>();
			}
			else {
				return T::Type;
			}
		}

	public:
		//AnyCommand作成
		template</*IsKey*/typename... T>
		_NODISCARD auto makeAnyCommand(const std::array<std::vector<IndexKey>, sizeof...(T)>& connections) {
			//必要なデータ型 DataTypeArray
			constexpr auto size = sizeof...(T);
			constexpr auto dataTypes = std::array<FvkType, size>{AnyCommandKeyFvkType<T>()...};

			//必要なデータを入れる
			auto filledData = fillData<dataTypes>(std::make_index_sequence<size>(), connections);

			using Element = Command::AnyCommandElement<AnyCommandKeyFvkType<T>()...>;
			std::shared_ptr<Element> elementPtr(new Element(ManagerPassKey{}, filledData));
			return Command::AnyCommand<AnyCommandKeyFvkType<T>()...>(std::move(elementPtr));
		}

	private:

		//no-throw
		//not const
		template<FvkType Type>
		[[nodiscard]] auto getItemVectorPtr() noexcept {
			return &std::get<CorrespondenceAt<Type>()>(data);
		}

		//no-throw
		//const
		template<FvkType Type>
		[[nodiscard]] auto getItemVectorPtr() const noexcept {
			return &std::get<CorrespondenceAt<Type>()>(data);
		}

		/*
		Exception:
			std::exception
		*/
		//strong
		template<FvkType Type>
		[[nodiscard]] auto getItem(const FKM::IndexKey key) const {
			auto data = getItemVectorPtr<Type>();
			auto& at = data->at(key);
			static_assert(noexcept(at.get()));
			return at.get();
		}

		/*
		Exception:
			std::exception
		*/
		//strong
		template<FvkType Type>
		[[nodiscard]] Data::CorrespondenceType<Type> getRef(const std::vector<FKM::IndexKey>& keys) const {
			using T = Data::CorrespondenceType<Type>;

			//no-throw
			auto data = getItemVectorPtr<Type>();

			if constexpr (FU::Concept::IsStdVector<T>) {
				std::remove_const_t<T> result{};

				for (Size i = 0, size = keys.size(); i < size; i++) {
					result.emplace_back(this->getItem(keys[i]));
				}
				return result;
			}
			else {//not vector
				assert(keys.size() == 1);
				//strong
				return this->getItem(keys.at(0));
			}
		}

		template<auto DataTypes, FU::Concept::IsStdArray Array, std::size_t... Index>
		[[nodiscard]] auto fillData(std::index_sequence<Index...>, const Array& connections) {
			return std::make_tuple(this->getRef<DataTypes.at(Index)>(connections.at(Index))...);
		}

		template<auto DataTypeArray>
		static consteval auto DataTypeArrayToAddKeyDataTypeArray() {
			using Array = decltype(DataTypeArray);
			Array result{};
			for (Size i = 0; const auto & x : DataTypeArray) {
				result.at(i) = ToHandleType(x);
				i++;
			}
			return result;
		}

		/*
		Exception:
			std::exception
		*/
		//strong
		template<auto DataTypeArray, Size N>
		std::vector<std::pair<FvkType, IndexKey>> toKeyAddType(const std::array<std::vector<IndexKey>, N>& indices) const {
			assert(DataTypeArray.size() == N);
			constexpr auto currectDataTypeArray = DataTypeArrayToAddKeyDataTypeArray<DataTypeArray>();
			std::vector<std::pair<FvkType, IndexKey>> result{};
			static_assert(std::is_nothrow_move_constructible_v<decltype(result)::value_type>);
			for (Size i = 0; i < N; i++) {
				for (Size j = 0, size = indices[i].size(); j < size; j++) {
					result.emplace_back(std::make_pair(currectDataTypeArray[i], indices[i][j]));
				}
			}
			return result;
		}

	public://terminate
		//must be terminated explicitly.
		void terminate();

	private:
		template<std::size_t Index = std::tuple_size_v<DataTuple>>
		void terminate_recursive() {
			if constexpr (Index == 0)
				return;
			else {
				//glfwだけはvectorでない
				if constexpr (Index == 1) {
					Glfw& glfw = std::get<std::tuple_element_t<Index - 1, DataTuple>>(this->data);
					glfw.destroy();
				}
				else {
					using ItemsType = std::tuple_element_t<Index - 1, DataTuple>;
					auto& items = std::get<ItemsType>(this->data);
					for (int64_t i = items.size() - 1; i >= 0; i--) {
						items[i].destroy();
						GKeyManager.erase(ItemTypeToFvkType<ItemsType::value_type>(), IndexKey(static_cast<IndexKey::Type>(i)));
					}
					terminate_recursive<Index - 1>();
				}
			}
		}

	private://data
		DataTuple data = [&]() -> DataTuple {
			auto element = []<Size Index, bool IsVector>() ->auto {
				if constexpr (IsVector)
					return std::tuple_element_t<Index, DataTuple>();
				else
					return ManagerPassKey();
			};
			return[&]<Size...Index>(std::integer_sequence<Size, Index...>)->DataTuple {
				return DataTuple(element.operator() < Index, FU::Concept::IsStdVector<std::tuple_element_t<Index, DataTuple>> > ()...);
			}(std::make_integer_sequence<Size, std::tuple_size_v<DataTuple>>());
		}();


	};
}


//Manager
namespace FVK::Internal {
	inline std::unique_ptr<Manager::Manager> GManager = nullptr;
}

