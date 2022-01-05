﻿#pragma once

#include "../Type/type.h"
#include <type_traits>

namespace FU::Concept {

	//remove all const
	template<typename T>
	struct remove_all_const : std::remove_const<T> {};
	template<typename T>
	struct remove_all_const<T*> {
		using type = remove_all_const<T>::type*;
	};
	template<typename T>
	struct remove_all_const<T* const> {
		using type = remove_all_const<T>::type*;
	};

	template<typename T>
	using remove_all_const_t = remove_all_const<T>::type;

}

namespace FU::Concept {

	template<typename T>
	struct remove_all_pointer : std::remove_pointer<T> {};
	template<typename T>
	struct remove_all_pointer<T*> {
		using type = remove_all_pointer<T>::type;
	};
	template<typename T>
	struct remove_all_pointer<T* const> {
		using type = remove_all_pointer<T>::type;
	};

	template<typename T>
	using remove_all_pointer_t = remove_all_pointer<T>::type;

}

namespace FU::Concept {

	//remove all const && v && ref && p
	template<typename T>
	struct remove_all {
		using type = remove_all_const<remove_all_pointer_t<std::remove_cvref_t<T>>>::type;
	};

	template<typename T>
	using remove_all_t = remove_all<T>::type;

}

namespace FU::Concept {

	template<typename T>
	concept IsEnum = std::is_enum_v<T>;

	template<typename T>
	concept IsClass = std::is_class_v<T>;

}

namespace FU::Concept {

	namespace Internal {
		template<typename T>
		class is_std_array final :public std::false_type {};
		template<typename T, std::size_t Size>
		class is_std_array<std::array<T, Size>> final :public std::true_type {};
	}

	template<typename T>
	concept IsStdArray = Internal::is_std_array<remove_all_t<T>>::value;

	template<typename T, typename U>
	concept IsStdArrayParticularType = IsStdArray<T> && std::same_as<U, typename remove_all_t<T>::value_type>;

}

namespace FU::Concept {

	namespace Internal {
		template<typename T>
		class is_std_vector final :public std::false_type {};
		template<typename T>
		class is_std_vector<std::vector<T>> final :public std::true_type {};
	}

	template<typename T>
	concept IsStdVector = Internal::is_std_vector<remove_all_t<T>>::value;

	template<typename T, typename U>
	concept IsStdVectorParticularType = IsStdVector<T> && std::same_as<U, typename remove_all_t<T>::value_type>;

}

namespace FU::Concept {

	namespace Internal {
		template<typename T>
		class is_unique_ptr : public std::false_type {};
		template<typename T>
		class is_unique_ptr<std::unique_ptr<T>> :public std::true_type {};
	}

	template<typename T>
	concept IsUniquePtr = Internal::is_unique_ptr<remove_all_t<T>>::value;

}

namespace FU::Concept {

	template<typename T>
	concept IsStdString = std::same_as<remove_all_t<T>, std::string>;

}

namespace FU::Concept {

	namespace Internal {
		template<typename T>
		class is_std_tuple : public std::false_type {};
		template<typename... T>
		class is_std_tuple<std::tuple<T...>> :public std::true_type {};
	}

	template<typename... T>
	concept IsTuple = Internal::is_std_tuple<remove_all_t<T...>>::value;

}

namespace FU::Concept {

	template<typename T>
	concept IsScopedEnum = std::is_enum_v<T> && !std::is_convertible_v<T, std::underlying_type_t<T>>;

}

namespace FU::Concept {

	//https://teratail.com/questions/334431#reply-461376
	template<typename T, template <typename ...> typename U>
	class is_instance_of {
	private:
		template<typename V> class helper : public std::false_type {};
		template<typename... V> class helper<U<V...>> : public std::true_type {};
	public:
		static constexpr bool value = helper<T>::value;
	};

	template<typename T, template <typename ...> typename U>
	concept IsInstanceOf = is_instance_of<T, U>::value;


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//non-type template is_instance_of
	template<typename T, template <auto ...> typename U>
	class is_instance_of_nontype {
	private:
		template<typename V> class helper : public std::false_type {};
		template<auto... V> class helper<U<V...>> : public std::true_type {};
	public:
		static constexpr bool value = helper<T>::value;
	};

	template<typename T, template <auto ...> typename U>
	concept IsInstanceOfNonType = is_instance_of_nontype<T, U>::value;


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//non-type type template  is_instance_of
	template<typename T, template <auto, typename> typename U>
	class is_instance_of_nontype_type {
	private:
		template<typename V> class helper : public std::false_type {};
		template<auto V1, typename V2> class helper<U<V1, V2>> : public std::true_type {};
	public:
		static constexpr bool value = helper<T>::value;
	};

	template<typename T, template <auto, typename> typename U>
	concept IsInstanceOfNonTypeType = is_instance_of_nontype_type<T, U>::value;

}

namespace FU::Concept {

	//class member function -> true
	template<auto Func>
	concept IsMemberFunctionPointer = std::is_member_function_pointer_v<decltype(Func)>;

	//non class member function -> true
	template<auto Func>
	concept IsNonMenberFunctionPointer = std::is_function_v<std::remove_pointer_t<decltype(Func)>>;

	//is function
	template<auto Func>
	concept IsFunction = IsMemberFunctionPointer<Func> || IsNonMenberFunctionPointer<Func>;

}

namespace FU::Concept {

	//&&...
	template<typename T1, typename ...T2>
	[[nodiscard]] constexpr bool isSame() {
		return (std::same_as<T1, T2> && ...);
	}

	template<typename T1, typename ...T2>
	concept IsSame = isSame<T1, T2...>();

}

namespace FU::Concept {

	template<typename T>
	concept IsNothrowMoveConstructibleAssignable =
		std::is_nothrow_move_constructible_v<T> &&
		std::is_nothrow_move_assignable_v<T>;

	template<typename T>
	concept IsNothrowCopyConstructibleAssignable =
		std::is_nothrow_copy_constructible_v<T> &&
		std::is_nothrow_copy_assignable_v<T>;

	template<typename T>
	concept IsNothrowCopyMoveConstructibleAssignable =
		IsNothrowMoveConstructibleAssignable<T> &&
		IsNothrowCopyConstructibleAssignable<T>;

}

namespace FU::Concept {

	template <typename T>
	constexpr bool DelayAssert_T = false;
	template <auto N>
	constexpr bool DelayAssert_N = false;

}

