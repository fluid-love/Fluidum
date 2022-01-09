#pragma once

#include "../../Utils/include.h"
#include "type.h"

namespace FD {

	//T is DataType
	template<typename ...T>
	class Manager final {
		//ng: template<DataAble... T> -> because of "friend class"
		static_assert((IsDataAble<T> && ...), "T does not meet requirements. See IsDataAble.");


	private://using
		using RawDataTuple = std::tuple<T...>;//raw data-type
		using DataTuple = FU::Tuple::RawPtrToUniquePtr<RawDataTuple>::Type;//raw-ptr to unique_ptr

	private://assert
		//same type
		static_assert(FU::Tuple::IsSameTypeInTuple<RawDataTuple>() == false, "No same data types are allowed.");

	private:
		//https://eel.is/c++draft/expr.sizeof#:array,sizeof
		//The result of sizeof and sizeof... is a prvalue of type std::size_t
		static constexpr inline Size DataSize = sizeof...(T);

		//sync
		std::mutex mtx{};

	public:
		FluidumUtils_Class_Default_ConDestructor(Manager);
			FluidumUtils_Class_Delete_CopyMove(Manager);

	public:
		//get data
		//Do Not Use "delete"!!
		template<Size Index>
		[[nodiscard]] auto get() {
			std::lock_guard<std::mutex> lock(mtx);//lock

			//if unique_ptr
			if constexpr (FU::Concept::IsUniquePtr<std::tuple_element_t<Index, DataTuple>>) {
				//not maked
				if (!std::get<Index>(data))
					throw std::runtime_error("Failed to get data. There was an attempt to get not-created data.");
				return std::get<Index>(data).get();
			}
			else//not unique_ptr
				return &std::get<Index>(data);
		}

		template<Size Index>
		[[nodiscard]] bool isDataCreated() {
			std::lock_guard<std::mutex> lock(mtx);//lock
			static_assert(FU::Concept::IsUniquePtr<std::tuple_element_t<Index, DataTuple>>);
			if (!std::get<Index>(data))
				return false;
			return true;
		}

		template<IsDataAble Data, typename ...Arg>
		void make(Arg&&...arg) {
			std::lock_guard<std::mutex> lock(mtx);//lock

			static_assert(std::is_pointer_v<Data>, "\"Data\" is not pointer．");

			using RemovePtr = std::remove_pointer_t<Data>;
			using Type = std::unique_ptr<std::remove_pointer_t<Data>>;

			//already maked
			if (std::get<Type>(data))
				throw std::runtime_error("Failed to create data. Data is already maked.");

			//PassKey
			if constexpr (IsConstructibleFromManagerPassKey<RemovePtr>)
				std::get<Type>(data) = std::make_unique<RemovePtr>(Internal::ManagerPassKey{}, std::forward<Arg>(arg)...);
			else
				std::get<Type>(data) = std::make_unique<RemovePtr>(std::forward<Arg>(arg)...);
		}

		template<IsDataAble Data>
		void deleteData() {
			std::lock_guard<std::mutex> lock(mtx);//lock

			static_assert(std::is_pointer_v<Data>, "\"Data\" is not pointer.");

			using Type = std::unique_ptr<std::remove_pointer_t<Data>>;

			//already deleted
			if (!std::get<Type>(data))
				throw std::runtime_error("Failed to delete data．Data is not maked.");

			std::get<Type>(data).reset();
		}

	private:
		DataTuple data = []<Size...Index>(std::index_sequence<Index...>)->DataTuple {

			auto helper = []<Size I>() {
				if constexpr (IsConstructibleFromManagerPassKey<std::tuple_element_t<I, DataTuple>>)
					return Internal::ManagerPassKey();
				else
					return std::nullptr_t();
			};

			return DataTuple(helper.template operator() < Index > () ...);
		}(std::make_index_sequence<DataSize>());

	};


}