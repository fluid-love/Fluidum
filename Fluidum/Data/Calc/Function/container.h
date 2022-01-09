#pragma once

#include "../../Common/common.h"

namespace FD::Calc::Internal {

	template<typename...Data>
	class FunctionContainer final {
	public:
		FluidumUtils_Class_Default_ConDestructor(FunctionContainer);
			FluidumUtils_Class_Delete_CopyMove(FunctionContainer);
	private:
		struct Ref final {
			FU::Class::ClassCode::CodeType type{};
			uint16_t index{};
		};
		std::vector<Ref> ref{};
		std::tuple<std::vector<Data>...> data{};
		std::size_t index = 0;
	public:
		template<typename T> requires(std::same_as<T, Data> || ...)
			void push(T&& d) {
			auto& dataRef = std::get<std::vector<T>>(this->data);
			dataRef.emplace_back(std::forward<T>(d));
			constexpr FU::Class::ClassCode::CodeType code = FU::Class::ClassCode::GetClassCode<T>();

			assert(!dataRef.empty());
			ref.emplace_back(Ref{ code , static_cast<uint16_t>(dataRef.size() - 1) });
			index++;
		}

		_NODISCARD bool empty() const noexcept {
			return ref.empty();
		}


	};

}
