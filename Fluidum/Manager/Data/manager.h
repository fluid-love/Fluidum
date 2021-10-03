#pragma once

#include "type.h"

namespace FD {

	template<typename ...T>
	class Manager final {

		//Dataとして保存できる型かをチェック
		//template<DataAble... T>でするとKeyが使えないのでstatic_assertで
		static_assert((IsDataAble<T> && ...), "指定した型はDataとして管理できません．Dataの型を確認してください．");


	private:///using
		using RawDataTuple = std::tuple<T...>;//無加工のデータの型
		using DataTuple = FU::Tuple::RawPtrToUniquePtr<RawDataTuple>::Type;//生ポインタをunique_ptrに変換

	private://assert
		//同じ型は禁止
		static_assert(FU::Tuple::isSameTypeInTuple<RawDataTuple>() == false, "Dataに同じ型が含まれています．");

	private:
		static constexpr inline std::size_t DataSize = static_cast<uint16_t>(sizeof...(T));

		//同期
		std::mutex mtx{};

	public:
		FluidumUtils_Class_Default_ConDestructor(Manager)
			FluidumUtils_Class_Delete_CopyMove(Manager)

	public:
		//dataを取得
		//deleteしてはいけない
		template<std::size_t Index>
		_NODISCARD auto get() {
			std::lock_guard<std::mutex> lock(mtx);//lock

			//unique_ptrならgetしたのを返す
			if constexpr (FU::Concept::IsUniquePtr<std::tuple_element_t<Index, DataTuple>>) {
				//dataが作られていない
				if (!std::get<Index>(data))
					throw std::runtime_error("Failed to get data. There was an attempt to get not-created data.");
				return std::get<Index>(data).get();
			}
			else//それ以外はアドレスを
				return &std::get<Index>(data);
		}

		//dataが作成されているか
		template<std::size_t Index>
		_NODISCARD bool isDataCreated() {
			std::lock_guard<std::mutex> lock(mtx);//lock
			static_assert(FU::Concept::IsUniquePtr<std::tuple_element_t<Index, DataTuple>>);
			if (!std::get<Index>(data))
				return false;
			return true;
		}

		template<IsDataAble Data, typename ...Arg>
		void createData(Arg&&...arg) {
			std::lock_guard<std::mutex> lock(mtx);//lock

			static_assert(std::is_pointer_v<Data>, "ポインタ以外は終始存在します．");

			//PtrにUniquePtrをつける
			using RemovePtr = std::remove_pointer_t<Data>;
			using Type = std::unique_ptr<std::remove_pointer_t<Data>>;

			//データが既に作られているなら
			if (std::get<Type>(data))
				throw std::runtime_error("Failed to create data. Data is already created.");

			//PassKeyが必要なら
			if constexpr (IsConstructibleFromManagerPassKey<RemovePtr>)
				std::get<Type>(data) = std::make_unique<RemovePtr>(Internal::ManagerPassKey{}, std::forward<Arg>(arg)...);
			else
				std::get<Type>(data) = std::make_unique<RemovePtr>(std::forward<Arg>(arg)...);
		}

		template<IsDataAble Data>
		void deleteData() {
			std::lock_guard<std::mutex> lock(mtx);//lock

			static_assert(std::is_pointer_v<Data>, "ポインタ以外は終始存在します．");

			//PtrにUniquePtrをつける
			using Type = std::unique_ptr<std::remove_pointer_t<Data>>;

			//データが作られていない
			if (!std::get<Type>(data))
				throw std::runtime_error("Failed to delete data．Data is not created.");

			std::get<Type>(data).reset();
		}

	private://データ本体		
		DataTuple data = []<std::size_t...Index>(std::index_sequence<Index...>)->DataTuple {

			auto helper = []<std::size_t I>() {
				if constexpr (IsConstructibleFromManagerPassKey<std::tuple_element_t<I, DataTuple>>)
					return Internal::ManagerPassKey();
				else
					return std::nullptr_t();
			};

			return DataTuple(helper.template operator() < Index > () ...);
		}(std::make_index_sequence<DataSize>());

	};


}