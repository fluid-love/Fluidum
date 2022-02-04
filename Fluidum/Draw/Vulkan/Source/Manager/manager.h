#pragma once

#include "type.h"
#include "concept.h"
#include "../Common/fkm.h"

namespace FVK::Internal::Manager {

	class Manager final {
	public:
		//Glfw and DataTuple's constructor may throw an exception.     -> call std::terminate
		//If the initialization of glfw fails, an exception is thrown. -> call std::terminate
		Manager() noexcept;

		~Manager() noexcept;
		FluidumUtils_Class_Delete_CopyMove(Manager);

	public:
		/*
		Exception:
			std::exception
			CollisionOfKeys
			FailedToCreate
			NotSupported
		*/
		//strong
		//if item don't need any other data.
		template<FvkType Type, IsParameter Parameter>
		void add(const char* key, Parameter& parameter) {
			static_assert(GetRequiredDataSize<Type>() == 0);

			//Same name is not allowed.
			if (GKeyManager.isKeyCollided(key))
				Exception::throwCollisionOfKeys();

			//No duplicate names guaranteed.
			try {
				//strong
				GKeyManager.add<ToHandleType<Type>()>(key);
			}
			catch (const FKM::Exception::Error& e) {
				if (e.code() == FKM::Exception::ErrorType::ReachIndexLimit) {
					GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "The maximum number of items that can be added has been exceeded.");
				}
				else {
					GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Internal Error.");
				}
				Exception::throwFailedToCreate();
			}

			try {
				auto ptr = this->getItemVectorPtr<Type>();
				Data::Data<Type> requiredData({});

				//std::vector::value_type.
				using ValueType = std::tuple_element_t<CorrespondenceAt<Type>(), DataTuple>::value_type;
				//Class of each item
				using ElmType = std::tuple_element_t<CorrespondenceAt<Type>(), DataTuple>::value_type::element_type;

				std::unique_ptr<ElmType> unique(new ElmType(ManagerPassKey{}, requiredData, parameter));

				try {
					//https://eel.is/c++draft/vector.modifiers
					//strong
					static_assert(FU::Concept::IsNothrowMoveConstructibleAssignable<ValueType>);
					ptr->emplace_back(std::move(unique));
				}
				catch (...) {
					GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "emplace_back throw exception.");
					unique->destroy();//valid
					static_assert(noexcept(unique->destroy()));
					std::rethrow_exception(std::current_exception());
				}
			}
			catch (...) {
				try {
					GKeyManager.remove(key);
				}
				catch (...) {
					std::terminate();
				}
				std::rethrow_exception(std::current_exception());
			}

		}

		//strong
		template<FvkType Type, IsParameter Parameter>
		void add(const char* key, Parameter& parameter, const Key::RequiredDataKeys<Type>& keys) {
			constexpr Size size = GetRequiredDataSize<Type>();
			static_assert(size > 0, "Size == 0.");

			if (Internal::GKeyManager.isKeyCollided(key))
				Exception::throwCollisionOfKeys();

			//connection
			const std::array<std::vector<IndexKey>, size> connections = keys.convert();

			//DataTypeArray
			constexpr auto dataTypes = GetRequiredDataTypes<Type>();

			//Fill the necessary data.
			//strong
			auto filledData = this->fillData<dataTypes>(std::make_index_sequence<size>(), connections);
			Data::Data<Type> requiredData(std::move(filledData));

			//no-throw
			auto ptr = this->getItemVectorPtr<Type>();

			//register key
			//No duplicate names guaranteed.
			try {
				//strong
				GKeyManager.add_recursive<ToHandleType<Type>()>(key, toKeyAddType<dataTypes>(connections));
			}
			catch (const ::FKM::Exception::Error& e) {
				if (e.code() == FKM::Exception::ErrorType::ReachIndexLimit) {
					GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "The maximum number of items that can be added has been exceeded.");
				}
				else {
					GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Internal Error.");
				}
				Exception::throwFailedToCreate();
			}
			catch (...) {
				std::rethrow_exception(std::current_exception());
			}

			try {
				//std::vector::value_type.
				using ValueType = std::tuple_element_t<CorrespondenceAt<Type>(), DataTuple>::value_type;
				//Class of each item
				using ElmType = std::tuple_element_t<CorrespondenceAt<Type>(), DataTuple>::value_type::element_type;

				std::unique_ptr<ElmType> unique(new ElmType(ManagerPassKey{}, requiredData, parameter));

				try {
					//https://eel.is/c++draft/vector.modifiers
					//strong
					static_assert(FU::Concept::IsNothrowMoveConstructibleAssignable<ValueType>);
					ptr->emplace_back(std::move(unique));
				}
				catch (...) {
					GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "emplace_back throw exception.");
					unique->destroy();//valid
					std::rethrow_exception(std::current_exception());
				}
			}
			catch (...) {
				try {
					GKeyManager.remove(key);
				}
				catch (...) {
					std::terminate();
				}
				std::rethrow_exception(std::current_exception());
			}

		}

	public:
		template<FvkType Type>
		[[nodiscard]] static consteval bool IsItemNothrowDestructible() {
			using ElmType = std::tuple_element_t<CorrespondenceAt<Type>(), DataTuple>::value_type::element_type;
			return std::is_nothrow_invocable_v<decltype(&ElmType::destroy), ElmType>;
		}

	public:
		//no-throw
		/*
		Return true if destructible.Does not always mean destructible,
		as it may fail due to lack of memory or other reasons.
		*/
		template<FvkType Type, Key::IsKey T>
		[[nodiscard]] bool isItemDestructible(const T& key) const noexcept {
			try {
				//NotFound, Unexpected
				//strong
				const IndexKey index = Key::Converter::toIndexKey(key);

				//used in other items.
				//strong
				const bool result = GKeyManager.isKeyConnected(Type, index);
				if (!result) {
					return false;
				}
			}
			catch (...) {
				return false;
			}
			return true;
		}

		/*
		Exception:
			std::exception
			NotFound
			FailedToDestroy
			Unexpected
		*/
		//strong
		//if item don't need any other data.	
		template<FvkType Type, Key::IsKey T>
		void destroy(const T& key) {

			//NotFound, Unexpected
			//strong
			const IndexKey index = Key::Converter::toIndexKey(key);

			try {
				//used in other items.
				//strong
				const bool result = GKeyManager.isKeyConnected(Type, index);
				if (!result) {
					Exception::throwFailedToDestroy();
				}
			}
			catch (...) {
				Exception::throwFailedToDestroy();
			}

			auto ptr = getItemVectorPtr<Type>();

			if constexpr (IsRequireWaitIdle<Data::Info<Type>>) {
				vk::Device device = ptr->at(index)->get().device;
				auto result = device.waitIdle();
				if (result != vk::Result::eSuccess) {
					GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to waitIdle.");
					Exception::throwFailedToDestroy();
				}
			}

			auto* item = ptr->at(index).get();

			//Most of them are noexcept, but some may throw a FailedToDestroy.
			item->destroy();

			//no-throw
			//https://eel.is/c++draft/vector#modifiers-4
			static_assert(std::is_nothrow_move_constructible_v<std::tuple_element_t<CorrespondenceAt<Type>(), DataTuple>::value_type>);
			static_assert(std::is_nothrow_move_assignable_v<std::tuple_element_t<CorrespondenceAt<Type>(), DataTuple>::value_type>);
			ptr->erase(ptr->cbegin() + index);

			//remove key
			try {
				//strong
				GKeyManager.remove(Type, index);
			}
			catch (const ::FKM::Exception::Error& e) {
				if (e.code() == ::FKM::Exception::ErrorType::NotFound) {
					//serious internal error
					GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Internal Error.");
					std::terminate();
				}
			}
			catch (...) { //retry
				GMessenger.add<FU::Log::Type::Warning>(__FILE__, __LINE__, "Failed to remove key.");
				try {
					//strong
					GKeyManager.remove(Type, index);
				}
				catch (...) {
				}
				std::terminate();
			}

		}

	public:
		/*
		Exception:
			NotFound
			Unexpected
		*/
		//strong
		//return const&
		template<FvkType Type, Key::IsKey K>
		[[nodiscard]] const auto& refInfo(const K& key) const {
			const IndexKey index = Key::Converter::toIndexKey(key);
			const auto data = this->getItemVectorPtr<Type>();
			if (data->size() <= index)
				Exception::throwUnexpected();
			return data->at(index)->get();//return info&
		}

		/*
		Exception:
			NotFound
			Unexpected
		*/
		//strong
		//return const&
		template<FvkType Type, Key::IsKey K>
		[[nodiscard]] const auto& refItem(const K& key) const {
			const IndexKey index = Key::Converter::toIndexKey(key);
			const auto data = getItemVectorPtr<Type>();
			if (data->size() <= index)
				Exception::throwUnexpected();
			return *data->at(index).get();//return item&
		}

	public:
		//make Command
		template<CommandType Type>
		[[nodiscard]] auto makeCommand(const Key::ConnectionCommandKeysBase<Type>& keys, const UI32 commandBufferIndex) {
			constexpr auto size = GetCommandDataSize<Type>();
			constexpr auto dataTypes = GetCommandDataTypes<Type>();

			//connection
			const std::array<std::vector<IndexKey>, size> connections = keys.convert();

			auto filledData = this->fillData<dataTypes>(std::make_index_sequence<size>(), connections);
			Data::CommandData<Type> requiredData(std::move(filledData));

			using Element = Command::CommandElement<Type>;
			std::shared_ptr<Element> elementPtr(new Element(ManagerPassKey{}, requiredData, commandBufferIndex));
			return Command::Command<Type>(std::move(elementPtr));
		}

	private://helper
		template<typename T>
		static consteval FvkType AnyCommandKeyFvkType() {
			//if std::vector
			if constexpr (FU::Concept::IsStdVector<T>) {
				return ToHandleVectorType<T::value_type::Type>();
			}
			else {
				return T::Type;
			}
		}

	public:
		//make AnyCommand
		template<Key::IsKey ...T>
		[[nodiscard]] auto makeAnyCommand(const std::array<std::vector<IndexKey>, sizeof...(T)>& connections) {
			constexpr auto size = sizeof...(T);
			constexpr auto dataTypes = std::array<FvkType, size>{AnyCommandKeyFvkType<T>()...};

			auto filledData = this->fillData<dataTypes>(std::make_index_sequence<size>(), connections);

			using Element = Command::AnyCommandElement<AnyCommandKeyFvkType<T>()...>;
			std::shared_ptr<Element> elementPtr(new Element(ManagerPassKey{}, filledData));
			return Command::AnyCommand<AnyCommandKeyFvkType<T>()...>(std::move(elementPtr));
		}

	private:
		//no-throw
		//not const
		template<FvkType Type>
		[[nodiscard]] auto getItemVectorPtr() noexcept {
			return &std::get<CorrespondenceAt<Type>()>(this->data);
		}

		//no-throw
		//const
		template<FvkType Type>
		[[nodiscard]] auto getItemVectorPtr() const noexcept {
			return &std::get<CorrespondenceAt<Type>()>(this->data);
		}

		/*
		Exception:
			std::exception
		*/
		//strong
		template<FvkType Type>
		[[nodiscard]] Data::CorrespondenceType<Type> getRef(const std::vector<FKM::IndexKey>& keys) const {
			using T = Data::CorrespondenceType<Type>;
			const auto vector = this->getItemVectorPtr<Type>();
			if constexpr (FU::Concept::IsStdVector<T>) {
				std::remove_const_t<T> result{};

				for (Size i = 0, size = keys.size(); i < size; i++) {
					result.emplace_back(vector->at(keys[i]).get()->get());//vector -> uniqur_ptr::get() -> element_type::get
				}
				return result;
			}
			else {//not vector
				assert(keys.size() == 1);
				return vector->at(keys[0]).get()->get();
			}
		}

		/*
		Exception:
			std::exception
		*/
		//strong
		template<auto DataTypes, FU::Concept::IsStdArray Array, Size... Index>
		[[nodiscard]] auto fillData(std::index_sequence<Index...>, const Array& connections) const {
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
		/*
			FailedToDestroy

			Serious
		*/
		//strong or no-exception-safety
		//must be terminated explicitly.
		//When FailedToDestroy is thrown, it is strong.
		void terminate();

	private://helper
		template<Size Index = std::tuple_size_v<DataTuple>>
		void terminate_recursive() {
			if constexpr (Index == 0)
				return;
			else {
				using ItemsType = std::tuple_element_t<Index - 1, DataTuple>;
				auto& items = std::get<ItemsType>(this->data);
				for (ISize i = static_cast<ISize>(items.size()) - 1; i >= 0; i--) {
					items[i]->destroy();

					try {
						GKeyManager.remove(ItemTypeToFvkType<ItemsType::value_type::element_type>(), IndexKey(static_cast<IndexKey::KeyType>(i)));
					}
					catch (...) {
						GMessenger.add<FU::Log::Type::Warning>(__FILE__, __LINE__, "Failed to remove key.");
					}
				}
				terminate_recursive<Index - 1>();
			}
		}

	private://data
		Glfw glfw{ ManagerPassKey{} };
		DataTuple data{};

	};

}


//Manager
namespace FVK::Internal {
	inline std::unique_ptr<Manager::Manager> GManager = nullptr;
}

