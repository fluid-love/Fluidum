#pragma once

#include "../../Common/common.h"

namespace FS::Utils {

	template<FD::IsDataAble... Data>
	class ResetData final : public Scene {
	public:
		explicit ResetData(
			Data* const... data
		) : data(std::tuple(data...)) {
			FluidumScene_Log_Constructor(::FS::Utils::ResetData<Data...>);

			this->reset<sizeof...(Data)>();
		}
		void Constructor(Data...);

		~ResetData() noexcept {
			FluidumScene_Log_Destructor(::FS::Utils::ResetData<Data...>);
		}

		FluidumUtils_Class_Delete_CopyMove(ResetData);

	private:
		using Tuple = std::tuple<Data* const...>;
		Tuple data;

	public:
		virtual void call() override {
			assert(false);
		}

		template<std::size_t Size>
		void reset() {
			using T = FU::Concept::remove_all_t<std::tuple_element_t<Size - 1, Tuple>>;
			if constexpr (std::same_as<T, FD::ImPlotWrite>) {
				std::get<Size - 1>(this->data)->reset();
			}
			else if constexpr (std::same_as<T, FD::Calc::ArrayWrite>) {
				std::get<Size - 1>(this->data)->reset();
			}
			else {
				static_assert(FU::Concept::DelayAssert_T<T>);
			}

			if constexpr (Size > 1) {
				this->reset<Size - 1>();
			}

		}

	};

}