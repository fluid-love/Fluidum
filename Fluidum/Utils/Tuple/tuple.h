#pragma once

#include <iostream>
#include <tuple>
#include <assert.h>
#include <array>

#include "../Concept/concept.h"

namespace FU::Tuple {

	template<typename... T>
	concept IsTuple = ::FU::Concept::IsTuple<T...>;

}

namespace FU::Tuple {

	namespace Internal {
		template <Size N, typename Class, typename Return, typename ... Args>
		consteval auto GetFunctionArgType(Return(Class::*)(Args...)) {
			static_assert(N < sizeof...(Args));
			return std::tuple_element_t<N, std::tuple<Args...>>();
		};

		template <Size N, typename Return, typename ... Args>
		consteval auto GetFunctionArgType(Return(*)(Args...)) {
			static_assert(N < sizeof...(Args));
			return std::tuple_element_t<N, std::tuple<Args...>>();
		};

		template<Size N, auto Function>
			requires(::FU::Concept::IsFunction<Function>)
		struct FunctionArgType final {
			using Type = decltype(GetFunctionArgType<N>(Function));
		};
	}

	//void hoge(int) -> FunctionArgType<0, hoge> == int
	//struct A{void hoge(int){} }; -> FunctionArgType<0, &A::hoge> == int
	template<Size N, auto Function>
	using FunctionArgType = Internal::FunctionArgType<N, Function>::Type;

}

namespace FU::Tuple {

	namespace Internal {
		template<auto Function>
		struct ClassFunctionArgsToTuple final {
		private:
			template<typename... Args>
			struct Tuple final {
				using Type = std::tuple<Args...>;
			};
			template <typename Class, typename Return, typename... Args>
			static constexpr auto arg(Return(Class::*)(Args...)) {
				return Tuple<Args...>();
			}
		public:
			using Type = typename decltype(arg(Function))::Type;
		};

		template<auto Function>
		struct NonClassFunctionArgsToTuple final {
		private:
			template<typename... Args>
			struct Tuple final {
				using Type = std::tuple<Args...>;
			};
			template <typename Return, typename... Args>
			static constexpr auto arg(Return(*)(Args...)) {
				return Tuple<Args...>();
			}
		public:
			using Type = typename decltype(arg(Function))::Type;
		};

		template<auto Function>
			requires Concept::IsFunction<Function>
		struct FunctionArgsToTuple final {
			using Type = std::conditional_t<Concept::IsMemberFunctionPointer<Function>, ClassFunctionArgsToTuple<Function>, NonClassFunctionArgsToTuple<Function>>::Type;
		};
	}

	template<auto Function>
	using FunctionArgsToTupleType = Internal::FunctionArgsToTuple<Function>::Type;

}

namespace FU::Tuple {

	template <typename Class, typename Return, typename ... Args>
	[[nodiscard]] constexpr Size getFunctionArgsSize(Return(Class::*)(Args...)) {
		auto size = std::tuple_size_v<std::tuple<Args...>>;
		return size;
	};

}

namespace FU::Tuple {

	//count same type(Type) in tuple.
	namespace Internal {

		template<Size N, IsTuple Tuple, typename Type>
		consteval Size GetSameTypeSize() {
			if constexpr (N == 0)
				return 0;
			else {
				using T = std::tuple_element_t<N - 1, Tuple>;

				if (std::is_same_v<T, Type>)
					return  GetSameTypeSize<N - 1, Tuple, Type>() + 1;
				else
					return  GetSameTypeSize<N - 1, Tuple, Type>();
			}
		}
	}

	template<IsTuple Tuple, typename Type>
	[[nodiscard]] consteval Size GetSameTypeSize() {
		constexpr auto tupleSize = std::tuple_size_v<Tuple>;
		return Internal::GetSameTypeSize<tupleSize, Tuple, Type>();
	}

}

namespace FU::Tuple {

	//exist same type in tuple -> true
	template <IsTuple Tuple, typename Type>
	[[nodiscard]] consteval bool IsSameTypeContain() {
		if (GetSameTypeSize<Tuple, Type>() > 0)
			return true;
		else
			return false;
	}

}

namespace FU::Tuple {

	namespace Internal {

		template <Size N, IsTuple Tuple>
		consteval bool IsSameTypeInTuple() {
			if constexpr (N == 0)
				return false;
			else {
				using T = std::tuple_element_t<N - 1, Tuple>;
				if constexpr (FU::Tuple::GetSameTypeSize<Tuple, T>() > 1)
					return true;
				else
					return false;
			}
		}

	}

	template <IsTuple Tuple>
	[[nodiscard]] consteval bool IsSameTypeInTuple() {
		constexpr auto tupleSize = std::tuple_size_v<Tuple>;
		return Internal::IsSameTypeInTuple<tupleSize, Tuple>();
	}

}

namespace FU::Tuple {

	namespace Internal {
		template <Size N, IsTuple Tuple, typename Type>
		consteval Size GetSameTypeIndex() {
			if constexpr (N == 0)
				return std::numeric_limits<Size>::max();
			else {
				using T = std::tuple_element_t<N - 1, Tuple>;
				if (std::is_same_v<T, Type>)
					return  N - 1;
				else
					return GetSameTypeIndex<N - 1, Tuple, Type>();
			}
		}
	}


	//e.g.
	//getSameTypeIndex<std::tuple<int,float>>() == 1
	//static_assert: getSameTypeIndex<std::tuple<int,int>>() same type in tuple
	template <IsTuple Tuple, typename Type>
	[[nodiscard]] consteval Size GetSameTypeIndex() {
		static_assert(GetSameTypeSize<Tuple, Type>() == 1);
		constexpr auto tupleSize = std::tuple_size_v<Tuple>;
		return Internal::GetSameTypeIndex<tupleSize, Tuple, Type>();
	}

	//Under
	template <Size Under, IsTuple Tuple, typename Type>
	[[nodiscard]] consteval Size GetSameTypeIndexUnder() {
		return Internal::GetSameTypeIndex<Under, Tuple, Type>();
	}

}

namespace FU::Tuple {

	template<Size ...T>
	struct Indices final {
		using Value = std::index_sequence<T...>;

		template<Size Index>
		constexpr auto push() const ->Indices<T..., Index> {
			return Indices<T..., Index>();
		}

		template<Size Index>
		constexpr auto pushFront() const ->Indices<Index, T...> {
			return Indices<Index, T...>();
		}
	};

}

namespace FU::Tuple {

	namespace Internal {

		template <Size N, IsTuple BaseTuple, IsTuple Tuple>
		consteval auto GetSameTupleTypeIndices() {
			if constexpr (N == 0)
				return Indices<>();
			else {
				using T = std::tuple_element_t<N - 1, Tuple>;
				constexpr auto index = FU::Tuple::GetSameTypeIndex<BaseTuple, T>();
				auto result = GetSameTupleTypeIndices<N - 1, BaseTuple, Tuple>();
				return result.push<index>();
			}
		}

		template <IsTuple BaseTuple, IsTuple Tuple>
		[[nodiscard]] consteval auto GetSameTupleTypeIndices_() {
			constexpr auto tupleSize = std::tuple_size_v<Tuple>;
			return GetSameTupleTypeIndices<tupleSize, BaseTuple, Tuple>();
		}

	}

	template <IsTuple BaseTuple, IsTuple Tuple>
	struct SameTupleTypeIndices final {
		using Sequence = decltype(Internal::GetSameTupleTypeIndices_<BaseTuple, Tuple>())::template Value;
	};

}

namespace FU::Tuple {

	namespace Internal {

		template<Size N, IsTuple Tuple>
		consteval bool IsConstElementContain() {
			if constexpr (N == 0)
				return false;
			else {
				using T = std::tuple_element_t<N - 1, Tuple>;
				if (std::is_const_v<T>)
					return false;
				else
					return IsConstElementContain<N - 1, Tuple>();
			}
		}

	}

	template<IsTuple Tuple>
	[[nodiscard]] consteval bool IsConstElementContain() {
		constexpr auto tupleSize = std::tuple_size_v<Tuple>;
		return Internal::IsConstElementContain<tupleSize, Tuple>();
	}

}

namespace FU::Tuple {

	namespace Internal {

		template<typename ...T>
		struct TempTuple final {
			using Tuple = std::tuple<T...>;

			template<typename U>
			constexpr auto pushBack()const {
				return TempTuple<T..., U>();
			}

			template<typename U>
			constexpr auto pushFront() const {
				return TempTuple<U, T...>();
			}
		};

		template<Size I, Size N, IsTuple Tuple, typename Type>
		consteval auto ChangeElementType_() {
			if constexpr (N == 0) {
				return TempTuple<>();
			}
			else if constexpr (I == (N - 1)) {
				auto result = ChangeElementType_<N, N - 1, Tuple, Type>();
				auto next = result.pushBack<Type>();
				return next;
			}
			else {
				using T = std::tuple_element_t<N - 1, Tuple>;
				auto result = ChangeElementType_<I, N - 1, Tuple, Type>();
				auto next = result.pushBack<T>();
				return next;
			}
		}

		template<Size N, IsTuple Tuple, typename Type>
		[[nodiscard]] consteval auto ChangeElementType() {
			constexpr auto tupleSize = std::tuple_size_v<Tuple>;
			return ChangeElementType_<N, tupleSize, Tuple, Type>();
		}

	}

	template<Size N, IsTuple Tuple, typename Type>
	struct ChangeElementType final {
		using ChangedTuple = decltype(Internal::ChangeElementType<N, Tuple, Type>())::template Tuple;
	};

}

namespace FU::Tuple {

	namespace Internal {

		template<typename U>
		struct ReturnType final {
			constexpr ReturnType() {};
			using Ty = U;
		};

		template<Size Size1, Size Size2, IsTuple Tuple1, IsTuple Tuple2>
		consteval auto GetValue() {
			if constexpr (Size1 > 0) {
				using V = std::tuple_element_t<Size1 - 1, Tuple1>;
				return ReturnType<V>();
			}
			else if constexpr (Size2 > 0) {
				using V = std::tuple_element_t<Size2 - 1, Tuple2>;
				return ReturnType<V>();
			}
		}

		template<Size Size1, Size Size2, bool Next, IsTuple Tuple1, IsTuple Tuple2, typename... T>
		struct TupleCat final {

			using ValueType = decltype(GetValue<Size1, Size2, Tuple1, Tuple2>())::template Ty;
			using Result = TupleCat<!Next ? Size1 - 1 : 0, Next ? Size2 - 1 : Size2, (Size1 <= 1), Tuple1, Tuple2, ValueType, T...>;
			using Type = Result::Type;
		};

		template<IsTuple Tuple1, IsTuple Tuple2, typename... T>
		struct TupleCat<0, 0, true, Tuple1, Tuple2, T...> final {
			using Type = std::tuple<T...>;
		};

		template<IsTuple Tuple1, IsTuple Tuple2>
		struct Cat final {
			using Type = Internal::TupleCat<std::tuple_size_v<Tuple2>, std::tuple_size_v<Tuple1>, false, Tuple2, Tuple1>::Type;
		};

		template<Size N, IsTuple Tuple1, IsTuple Tuple2, IsTuple...Tuple>
		struct Cat_ final {
			using Cat12 = Cat<Tuple1, Tuple2>::Type;
			using Type = Cat_<N - 1, Cat12, Tuple...>::Type;
		};

		template<IsTuple Tuple1, IsTuple Tuple2, IsTuple...Tuple>
		struct Cat_<2, Tuple1, Tuple2, Tuple...> final {
			static_assert(sizeof...(Tuple) == 0);
			using Type = Cat<Tuple1, Tuple2>::Type;
		};

	}

	template<IsTuple...Tuple>
	using Cat = Internal::Cat_<sizeof...(Tuple), Tuple...>::Type;

}

namespace FU::Tuple {

	//std::tuple<int*> -> std::tuple<std::unique_ptr<int>>
	namespace Internal {

		template<Size N, IsTuple Tuple>
		consteval auto ChangeRawPtrToUniquePtr() {
			if constexpr (N == 0)
				return Tuple();
			else {
				using T = std::tuple_element_t<N - 1, Tuple>;
				if constexpr (std::is_pointer_v<T>) {
					using UPtr = std::unique_ptr<std::remove_pointer_t<T>>;
					using ChangedTuple = ::FU::Tuple::ChangeElementType<N - 1, Tuple, UPtr>::ChangedTuple;
					return ChangeRawPtrToUniquePtr<N - 1, ChangedTuple>();
				}
				else {
					return ChangeRawPtrToUniquePtr<N - 1, Tuple>();
				}
			}
		}

		template<Size N, IsTuple Tuple>
		struct RawPtrToUniquePtr final {
			using T = std::tuple_element_t<N - 1, Tuple>;
			using ValueType = std::conditional_t<std::is_pointer_v<T>, std::unique_ptr<std::remove_pointer_t<T>>, T>;
			using Temp = ::FU::Tuple::ChangeElementType<N - 1, Tuple, ValueType>::template ChangedTuple;
			using Type = RawPtrToUniquePtr<N - 1, Temp>::Type;
		};

		template<IsTuple Tuple>
		struct RawPtrToUniquePtr<0, Tuple> final {
			using Type = Tuple;
		};

	}

	template<IsTuple Tuple>
	struct RawPtrToUniquePtr final {
		using Type = Internal::RawPtrToUniquePtr<std::tuple_size_v<Tuple>, Tuple>::Type;
	};

}

namespace FU::Tuple {

	//runtime 
	namespace Internal {
		template <Size N>
		struct Visit final {
			template <typename T, typename Function>
			static void call(const T& tuple, const Size index, Function func) {
				if (index == N - 1)
					func(std::get<N - 1>(tuple));
				else
					Visit<N - 1>::call(tuple, index, func);
			}
		};

		template <>
		struct Visit<0> final {
			template <typename T, typename Function>
			static void call(const T& tuple, const Size index, Function func) {
				assert(false);
			}
		};
	}

	template <typename Function, IsTuple Tuple>
	void visit(const Tuple& tuple, const Size index, Function func) {
		Internal::Visit<std::tuple_size_v<Tuple>>::call(tuple, index, func);
	}

}

namespace FU::Tuple {

	//runtime
	namespace Internal {
		template <Size N>
		struct Set final {
			template <typename T, typename SetType>
			static void call(T& tuple, const Size index, const SetType& val) {
				if (index == N - 1) {
					if constexpr (std::same_as<SetType, std::tuple_element_t<N - 1, T>>)
						std::get<N - 1>(tuple) = val;
					else
						assert(false);
				}
				else
					Set<N - 1>::call(tuple, index, val);
			}
		};

		template <>
		struct Set<0> final {
			template <typename T, typename SetType>
			static void call(T& tuple, const Size index, const SetType& val) {
				assert(false);
			}
		};
	}

	template <typename SetType, IsTuple Tuple>
	void set(Tuple& tuple, const Size index, const SetType& val) {
		Internal::Set<std::tuple_size_v<Tuple>>::call(tuple, index, val);
	}

}

namespace FU::Tuple {

	namespace Internal {
		template<Size N, Tuple::IsTuple Tuple>
		constexpr void isDefaultConstrictible(std::array<bool, std::tuple_size_v<Tuple>>& result) {
			if constexpr (N == 0)
				return;
			else {
				if constexpr (std::is_default_constructible_v<std::tuple_element_t<N - 1, Tuple>>) {
					result.at(N - 1) = true;
				}
				isDefaultConstrictible<N - 1, Tuple>(result);
			}
		}
	}

	template<Tuple::IsTuple Tuple>
	[[nodiscard]] constexpr std::array<bool, std::tuple_size_v<Tuple>> isDefaultConstrictible() {
		std::array<bool, std::tuple_size_v<Tuple>> result;
		std::fill(result.begin(), result.end(), false);
		Internal::isDefaultConstrictible<std::tuple_size_v<Tuple>, Tuple>(result);
		return result;
	}

}

namespace FU::Tuple {

	//Array<int,3>::Type == std::tuple<int,int,int>
	namespace Internal {
		template<typename T, Size N, typename... U>
		struct Array final {
			using Type = Array<T, N - 1, U..., T>::Type;
		};

		template<typename T, typename... U>
		struct Array<T, 0, U...> final {
			using Type = std::tuple<U...>;
		};

	}

	template<typename T, Size N>
	using Array = Internal::Array<T, N>::Type;

}

namespace FU::Tuple {

	namespace Internal {
		template<IsTuple Tuple, typename T, Size N, template<typename...>typename Bool, typename... U>
		struct ChangeIf final {
			using CurrentType = std::tuple_element_t<N - 1, Tuple>;
			using PushType = std::conditional_t<std::derived_from<Bool<CurrentType>, std::true_type>, T, CurrentType>;
			using Type = ChangeIf<Tuple, T, N - 1, Bool, PushType, U...>::Type;
		};

		template<IsTuple Tuple, typename T, template<typename...>typename Bool, typename ...U>
		struct ChangeIf<Tuple, T, 0, Bool, U...> final {
			using Type = std::tuple<U...>;
		};
	}

	template<IsTuple Tuple, typename T, template<typename>typename Bool>
	struct ChangeIf final {
		using Type = Internal::ChangeIf<Tuple, T, std::tuple_size_v<Tuple>, Bool>::Type;
	};

}

namespace FU::Tuple {

	namespace Internal {
		template<IsTuple Tuple, typename T, Size N, template<typename...>typename Bool, typename... U>
		struct ChangeIfNot final {
			using CurrentType = std::tuple_element_t<N - 1, Tuple>;
			using PushType = std::conditional_t<std::derived_from<Bool<CurrentType>, std::false_type>, T, CurrentType>;
			using Type = ChangeIfNot<Tuple, T, N - 1, Bool, PushType, U...>::Type;
		};

		template<IsTuple Tuple, typename T, template<typename...>typename Bool, typename ...U>
		struct ChangeIfNot<Tuple, T, 0, Bool, U...> final {
			using Type = std::tuple<U...>;
		};
	}

	template<IsTuple Tuple, typename T, template<typename>typename Bool>
	struct ChangeIfNot final {
		using Type = Internal::ChangeIfNot<Tuple, T, std::tuple_size_v<Tuple>, Bool>::Type;
	};

}

namespace FU::Tuple {

	namespace Internal {
		template<typename T, Size N, T Under, T Push, T... Index>
		struct SequenceHelper final {
			using Type = std::conditional_t<
				(Push < Under),
				SequenceHelper<T, 0, Under, 0, Index...>,
				SequenceHelper<T, N - 1, Under, N - 1, Push, Index...>
				>::Type;
		};
		template<typename T, T Under, T Push, T... Index>
		struct SequenceHelper<T, 0, Under, Push, Index...> final {
			using Type = std::integer_sequence<T, Index...>;
		};

		template<typename T, Size N, T Under, T Push, T... Index>
		struct ReverseSequenceHelper final {
			using Type = std::conditional_t<
				(Push < Under),
				ReverseSequenceHelper<T, 0, Under, 0, Index...>,
				ReverseSequenceHelper<T, N - 1, Under, N - 1, Index..., Push>
				>::Type;
		};
		template<typename T, T Under, T Push, T... Index>
		struct ReverseSequenceHelper<T, 0, Under, Push, Index...> final {
			using Type = std::integer_sequence<T, Index...>;
		};

	}


	//makeIntegerSequence<int,2,5> -> return std::integer_sequence<int,2,3,4,5>
	template<std::integral T, T Index1, T Index2>
	[[nodiscard]] constexpr auto makeIntegerSequence() {
		if constexpr (Index1 < Index2)
			return Internal::SequenceHelper<T, Index2, Index1, Index2>::Type();
		else if constexpr (Index1 == Index2)
			return std::integer_sequence<T, Index1>();
		else
			return Internal::ReverseSequenceHelper<T, Index1, Index2, Index1>::Type();
	}

}

namespace FU::Tuple {

	//std::array to std::tuple
	template<auto Array, template<auto>typename Correspondence>
		requires(Concept::IsStdArray<decltype(Array)>)
	class ArrayToTuple final {
		using ArrayType = decltype(Array);

		template<typename Type>
		struct Ty {
			using T = Type;
		};
		template<Size...Index>
		static auto helper(std::index_sequence<Index...>) {
			return Ty<std::tuple<Correspondence<Array.at(Index)>...>>();
		}
	public:
		using Type = typename decltype(helper(std::make_index_sequence<std::tuple_size_v<ArrayType>>()))::T;
	};

	template<auto Array, template<auto>typename Correspondence>
	using ArrayToTupleType = ArrayToTuple<Array, Correspondence>::Type;

}

namespace FU::Tuple {

	namespace Internal {

		template<IsTuple Tuple, typename Func, Size... I>
		void for_each(Tuple& tuple, Func func, std::index_sequence<I...>) {
			auto list = { (func(std::get<I>(tuple)),0)... };
		}

	}

	template<IsTuple Tuple, typename Func>
	void for_each(Tuple& tuple, Func func) {
		Internal::for_each(tuple, func, std::make_index_sequence<std::tuple_size_v<Tuple>>());
	}

}





























