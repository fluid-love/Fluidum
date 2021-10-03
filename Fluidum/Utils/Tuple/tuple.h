#pragma once

#include <iostream>
#include <tuple>
#include <assert.h>
#include <array>

#include "../Concept/concept.h"

namespace FU::Tuple {


	template<typename... T>
	concept IsTuple = ::FU::Concept::IsTuple<T...>;


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	template <std::size_t N, typename Class, typename Return, typename ... Args>
	_NODISCARD constexpr auto getFunctionArgType(Return(Class::*)(Args...)) {
		return std::tuple_element_t<N, std::tuple<Args...>>();
	};

	template <std::size_t N, typename Return, typename ... Args>
	_NODISCARD constexpr auto getFunctionArgType(Return(*)(Args...)) {
		return std::tuple_element_t<N, std::tuple<Args...>>();
	};


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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

	template<auto Function>
	using FunctionArgsToTupleType = FunctionArgsToTuple<Function>::Type;


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	template <typename Class, typename Return, typename ... Args>
	_NODISCARD constexpr auto getFunctionArgsSize(Return(Class::*)(Args...)) {
		auto size = std::tuple_size_v<std::tuple<Args...>>;
		return size;
	};


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//count same type(Type) in tuple.
	namespace Internal {

		template<std::size_t Size, IsTuple Tuple, typename Type>
		_NODISCARD consteval std::size_t getSameTypeSize() {
			if constexpr (Size == 0)
				return 0;
			else {
				using T = std::tuple_element_t<Size - 1, Tuple>;

				if (std::is_same_v<T, Type>)
					return  getSameTypeSize<Size - 1, Tuple, Type>() + 1;
				else
					return  getSameTypeSize<Size - 1, Tuple, Type>();
			}
		}
	}

	template<IsTuple Tuple, typename Type>
	_NODISCARD consteval std::size_t getSameTypeSize() {
		constexpr auto tupleSize = std::tuple_size_v<Tuple>;
		return Internal::getSameTypeSize<tupleSize, Tuple, Type>();
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//exist same type in tuple -> true
	template <IsTuple Tuple, typename Type>
	_NODISCARD consteval bool isSameTypeContain() {
		if (getSameTypeSize<Tuple, Type>() > 0)
			return true;
		else
			return false;
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	namespace Internal {

		template <std::size_t Size, IsTuple Tuple>
		_NODISCARD consteval bool isSameTypeInTuple() {
			if constexpr (Size == 0)
				return false;
			else {
				using T = std::tuple_element_t<Size - 1, Tuple>;
				if constexpr (FU::Tuple::getSameTypeSize<Tuple, T>() > 1)
					return true;
				else
					return false;
			}
		}

	}

	template <IsTuple Tuple>
	_NODISCARD consteval bool isSameTypeInTuple() {
		constexpr auto tupleSize = std::tuple_size_v<Tuple>;
		return Internal::isSameTypeInTuple<tupleSize, Tuple>();
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	namespace Internal {
		template <std::size_t Size, IsTuple Tuple, typename Type>
		consteval std::size_t getSameTypeIndex() {
			if constexpr (Size == 0)
				return std::numeric_limits<std::size_t>::max();
			else {
				using T = std::tuple_element_t<Size - 1, Tuple>;
				if (std::is_same_v<T, Type>)
					return  Size - 1;
				else
					return getSameTypeIndex<Size - 1, Tuple, Type>();
			}
		}
	}


	//e.g.
	//getSameTypeIndex<std::tuple<int,float>>() == 1
	//static_assert: getSameTypeIndex<std::tuple<int,int>>() same type in tuple
	template <IsTuple Tuple, typename Type>
	consteval std::size_t getSameTypeIndex() {
		static_assert(getSameTypeSize<Tuple, Type>() == 1);
		constexpr auto tupleSize = std::tuple_size_v<Tuple>;
		return Internal::getSameTypeIndex<tupleSize, Tuple, Type>();
	}

	//Under
	template <std::size_t Under, IsTuple Tuple, typename Type>
	consteval std::size_t getSameTypeIndexUnder() {
		return Internal::getSameTypeIndex<Under, Tuple, Type>();
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<std::size_t ...T>
	struct Indices final {
		using Value = std::index_sequence<T...>;

		template<std::size_t index>
		constexpr auto push() const ->Indices<T..., index> {
			return Indices<T..., index>();
		}

		template<std::size_t index>
		constexpr auto pushFront() const ->Indices<index, T...> {
			return Indices<index, T...>();
		}
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	namespace Internal {

		template <std::size_t Size, IsTuple BaseTuple, IsTuple Tuple>
		consteval auto getSameTupleTypeIndices() {
			if constexpr (Size == 0)
				return Indices<>();
			else {
				using T = std::tuple_element_t<Size - 1, Tuple>;
				constexpr auto index = FU::Tuple::getSameTypeIndex<BaseTuple, T>();
				auto result = getSameTupleTypeIndices<Size - 1, BaseTuple, Tuple>();
				return result.push<index>();
			}
		}

	}

	template <IsTuple BaseTuple, IsTuple Tuple>
	consteval auto getSameTupleTypeIndices() {
		constexpr auto tupleSize = std::tuple_size_v<Tuple>;
		return Internal::getSameTupleTypeIndices<tupleSize, BaseTuple, Tuple>();
	}

	template <IsTuple BaseTuple, IsTuple Tuple>
	struct SameTupleTypeIndices final {
		using Sequence = decltype(getSameTupleTypeIndices<BaseTuple, Tuple>())::template Value;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	namespace Internal {

		template<std::size_t Size, IsTuple Tuple>
		_NODISCARD consteval bool isConstElementContain() {
			if constexpr (Size == 0)
				return false;
			else {
				using T = std::tuple_element_t<Size - 1, Tuple>;
				if (std::is_const_v<T>)
					return false;
				else
					return isConstElementContain<Size - 1, Tuple>();
			}
		}

	}

	template<IsTuple Tuple>
	_NODISCARD consteval bool isConstElementContain() {
		constexpr auto tupleSize = std::tuple_size_v<Tuple>;
		return Internal::isConstElementContain<tupleSize, Tuple>();
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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

		template<std::size_t N, std::size_t Size, IsTuple Tuple, typename Type>
		_NODISCARD consteval auto changeElementType() {
			if constexpr (Size == 0) {
				return TempTuple<>();
			}
			else if constexpr (N == (Size - 1)) {
				auto result = changeElementType<N, Size - 1, Tuple, Type>();
				auto next = result.pushBack<Type>();
				return next;
			}
			else {
				using T = std::tuple_element_t<Size - 1, Tuple>;
				auto result = changeElementType<N, Size - 1, Tuple, Type>();
				auto next = result.pushBack<T>();
				return next;
			}
		}

	}

	template<std::size_t N, IsTuple Tuple, typename Type>
	_NODISCARD consteval auto changeElementType() {
		constexpr auto tupleSize = std::tuple_size_v<Tuple>;
		return Internal::changeElementType<N, tupleSize, Tuple, Type>();
	}

	template<std::size_t N, IsTuple Tuple, typename Type>
	struct ChangeElementType final {
		using ChangedTuple = decltype(changeElementType<N, Tuple, Type>())::template Tuple;
	};


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	namespace Internal {

		template<typename U>
		struct ReturnType final {
			constexpr ReturnType() {};
			using Ty = U;
		};

		template<std::size_t Size1, std::size_t Size2, IsTuple Tuple1, IsTuple Tuple2>
		_NODISCARD consteval auto getValue() {
			if constexpr (Size1 > 0) {
				using V = std::tuple_element_t<Size1 - 1, Tuple1>;
				return ReturnType<V>();
			}
			else if constexpr (Size2 > 0) {
				using V = std::tuple_element_t<Size2 - 1, Tuple2>;
				return ReturnType<V>();
			}
		}

		template<std::size_t Size1, std::size_t Size2, bool Next, IsTuple Tuple1, IsTuple Tuple2, typename... T>
		struct TupleCat final {

			using ValueType = decltype(getValue<Size1, Size2, Tuple1, Tuple2>())::template Ty;
			using Result = TupleCat<!Next ? Size1 - 1 : 0, Next ? Size2 - 1 : Size2, (Size1 <= 1), Tuple1, Tuple2, ValueType, T...>;
			using Type = Result::Type;
		};

		template<IsTuple Tuple1, IsTuple Tuple2, typename... T>
		struct TupleCat<0, 0, true, Tuple1, Tuple2, T...> final {
			using Type = std::tuple<T...>;
		};

	}

	//TupleÇåãçá
	template<IsTuple Tuple1, IsTuple Tuple2>
	struct Cat final {
		using Type = Internal::TupleCat<std::tuple_size_v<Tuple2>, std::tuple_size_v<Tuple1>, false, Tuple2, Tuple1>::Type;
	};


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//std::tuple<int*> -> std::tuple<std::unique_ptr<int>>
	namespace Internal {

		template<std::size_t Size, IsTuple Tuple>
		_NODISCARD consteval auto changeRawPtrToUniquePtr() {
			if constexpr (Size == 0)
				return Tuple();
			else {
				using T = std::tuple_element_t<Size - 1, Tuple>;
				if constexpr (std::is_pointer_v<T>) {
					using UPtr = std::unique_ptr<std::remove_pointer_t<T>>;
					using ChangedTuple = ChangeElementType<Size - 1, Tuple, UPtr>::ChangedTuple;
					return changeRawPtrToUniquePtr<Size - 1, ChangedTuple>();
				}
				else {
					return changeRawPtrToUniquePtr<Size - 1, Tuple>();
				}
			}
		}

		template<std::size_t Size, IsTuple Tuple>
		struct RawPtrToUniquePtr final {
			using T = std::tuple_element_t<Size - 1, Tuple>;
			using ValueType = std::conditional_t<std::is_pointer_v<T>, std::unique_ptr<std::remove_pointer_t<T>>, T>;
			using Temp = ChangeElementType<Size - 1, Tuple, ValueType>::ChangedTuple;
			using Type = RawPtrToUniquePtr<Size - 1, Temp>::Type;
		};

		template<IsTuple Tuple>
		struct RawPtrToUniquePtr<0, Tuple> final {
			using Type = Tuple;
		};

	}

	template<IsTuple Tuple>
	_NODISCARD consteval auto rawPtrToUniquePtr() {
		constexpr auto tupleSize = std::tuple_size_v<Tuple>;
		return Internal::changeRawPtrToUniquePtr<tupleSize, Tuple>();
	}

	template<IsTuple Tuple>
	struct RawPtrToUniquePtr final {
		using Type = Internal::RawPtrToUniquePtr<std::tuple_size_v<Tuple>, Tuple>::Type;
	};



	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//runtime 
	namespace Internal {
		template <std::size_t Size>
		struct Visit final {
			template <typename T, typename Function>
			static void call(const T& tuple, const std::size_t index, Function func) {
				if (index == Size - 1)
					func(std::get<Size - 1>(tuple));
				else
					Visit<Size - 1>::call(tuple, index, func);
			}
		};

		template <>
		struct Visit<0> final {
			template <typename T, typename Function>
			static void call(const T& tuple, const std::size_t index, Function func) {
				assert(false);
			}
		};
	}

	template <typename Function, IsTuple Tuple>
	void visit(const Tuple& tuple, const std::size_t index, Function func) {
		Internal::Visit<std::tuple_size_v<Tuple>>::call(tuple, index, func);
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	//runtime
	namespace Internal {
		template <std::size_t Size>
		struct Set final {
			template <typename T, typename SetType>
			static void call(T& tuple, const std::size_t index, const SetType& val) {
				if (index == Size - 1) {
					if constexpr (std::same_as<SetType, std::tuple_element_t<Size - 1, T>>)
						std::get<Size - 1>(tuple) = val;
					else
						assert(false);
				}
				else
					Set<Size - 1>::call(tuple, index, val);
			}
		};

		template <>
		struct Set<0> final {
			template <typename T, typename SetType>
			static void call(T& tuple, const std::size_t index, const SetType& val) {
				assert(false);
			}
		};
	}

	template <typename SetType, IsTuple Tuple>
	void set(Tuple& tuple, const std::size_t index, const SetType& val) {
		Internal::Set<std::tuple_size_v<Tuple>>::call(tuple, index, val);
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	namespace Internal {
		template<std::size_t Size, Tuple::IsTuple Tuple>
		constexpr void isDefaultConstrictible(std::array<bool, std::tuple_size_v<Tuple>>& result) {
			if constexpr (Size == 0)
				return;
			else {
				if constexpr (std::is_default_constructible_v<std::tuple_element_t<Size - 1, Tuple>>) {
					result.at(Size - 1) = true;
				}
				isDefaultConstrictible<Size - 1, Tuple>(result);
			}
		}
	}

	template<Tuple::IsTuple Tuple>
	_NODISCARD constexpr std::array<bool, std::tuple_size_v<Tuple>> isDefaultConstrictible() {
		std::array<bool, std::tuple_size_v<Tuple>> result;
		std::fill(result.begin(), result.end(), false);
		Internal::isDefaultConstrictible<std::tuple_size_v<Tuple>, Tuple>(result);
		return result;
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//Array<int,3>::Type == std::tuple<int,int,int>
	namespace Internal {
		template<typename T, std::size_t Size, typename... U>
		struct Array final {
			using Type = Array<T, Size - 1, U..., T>::Type;
		};

		template<typename T, typename... U>
		struct Array<T, 0, U...> final {
			using Type = std::tuple<U...>;
		};

	}

	template<typename T, std::size_t Size>
	struct Array final {
		using Type = Internal::Array<T, Size>::Type;
	};


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	namespace Internal {
		template<IsTuple Tuple, typename T, std::size_t Size, template<typename...>typename Bool, typename... U>
		struct ChangeIf final {
			using CurrentType = std::tuple_element_t<Size - 1, Tuple>;
			using PushType = std::conditional_t<std::derived_from<Bool<CurrentType>, std::true_type>, T, CurrentType>;
			using Type = ChangeIf<Tuple, T, Size - 1, Bool, PushType, U...>::Type;
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


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	namespace Internal {
		template<IsTuple Tuple, typename T, std::size_t Size, template<typename...>typename Bool, typename... U>
		struct ChangeIfNot final {
			using CurrentType = std::tuple_element_t<Size - 1, Tuple>;
			using PushType = std::conditional_t<std::derived_from<Bool<CurrentType>, std::false_type>, T, CurrentType>;
			using Type = ChangeIfNot<Tuple, T, Size - 1, Bool, PushType, U...>::Type;
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


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	namespace Internal {
		template<typename T, std::size_t Size, T Under, T Push, T... Index>
		struct SequenceHelper final {
			using Type = std::conditional_t<
				(Push < Under),
				SequenceHelper<T, 0, Under, 0, Index...>,
				SequenceHelper<T, Size - 1, Under, Size - 1, Push, Index...>
				>::Type;
		};
		template<typename T, T Under, T Push, T... Index>
		struct SequenceHelper<T, 0, Under, Push, Index...> final {
			using Type = std::integer_sequence<T, Index...>;
		};

		template<typename T, std::size_t Size, T Under, T Push, T... Index>
		struct ReverseSequenceHelper final {
			using Type = std::conditional_t<
				(Push < Under),
				ReverseSequenceHelper<T, 0, Under, 0, Index...>,
				ReverseSequenceHelper<T, Size - 1, Under, Size - 1, Index..., Push>
				>::Type;
		};
		template<typename T, T Under, T Push, T... Index>
		struct ReverseSequenceHelper<T, 0, Under, Push, Index...> final {
			using Type = std::integer_sequence<T, Index...>;
		};

	}


	//makeIntegerSequence<int,2,5> -> return std::integer_sequence<int,2,3,4,5>
	template<std::integral T, T Index1, T Index2>
	constexpr auto makeIntegerSequence() {
		if constexpr (Index1 < Index2)
			return Internal::SequenceHelper<T, Index2, Index1, Index2>::Type();
		else if constexpr (Index1 == Index2)
			return std::integer_sequence<T, Index1>();
		else
			return Internal::ReverseSequenceHelper<T, Index1, Index2, Index1>::Type();
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//std::array to std::tuple
	template<auto Array, template<auto>typename Correspondence>
	requires(Concept::IsStdArray<decltype(Array)>)
		class ArrayToTuple final {
		using ArrayType = decltype(Array);

		template<typename Type>
		struct Ty {
			using T = Type;
		};
		template<std::size_t...Index>
		static auto helper(std::index_sequence<Index...>) {
			return Ty<std::tuple<Correspondence<Array.at(Index)>...>>();
		}
		public:
			using Type = typename decltype(helper(std::make_index_sequence<std::tuple_size_v<ArrayType>>()))::T;
	};

	template<auto Array, template<auto>typename Correspondence>
	using ArrayToTupleType = ArrayToTuple<Array, Correspondence>::Type;



}