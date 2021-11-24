#pragma once

#include "../../../../Utils/include.h"
#include "../../../External/magic_enum/include/magic_enum.hpp"

namespace FKM {

	class OrderKey final {
	public:
		using Type = uint32_t;
	public:
		OrderKey() = default;
		OrderKey(const Type val) noexcept;
		FluidumUtils_Class_Default_CopyMove(OrderKey)
	private:
		Type index = 0;
	public:
		[[nodiscard]] operator Type()const noexcept;
		void operator=(const Type val)noexcept;
		[[nodiscard]] bool operator==(const OrderKey key)const noexcept;

		void operator++(int) noexcept;

		[[nodiscard]] bool operator==(Type val)const noexcept;

	};


	class IndexKey final {
	public:
		using Type = uint32_t;
	public:
		IndexKey() = default;
		IndexKey(const Type val) noexcept;
		FluidumUtils_Class_Default_CopyMove(IndexKey)
	private:
		Type order = 0;
	public:
		[[nodiscard]] operator Type()const noexcept;
		void operator=(const Type val)noexcept;
		[[nodiscard]] bool operator==(const IndexKey key)const noexcept;
		[[nodiscard]] bool operator!=(const IndexKey key)const noexcept;
		[[nodiscard]] bool operator==(const Type val)const noexcept;

	};

}

namespace FKM {

	using CharKey = const char*;

	using StrKey = std::string;

	enum class SelectKey : uint16_t {
		Front,
		Back
	};

	using Connections = std::vector<OrderKey>;

}

namespace FKM::Internal {

	template<typename T>
	concept IsIndexKey = std::same_as<T, IndexKey>;

	//serial number
	template<typename T>
	concept IsOrderKey = std::same_as<T, OrderKey>;

	template<typename T>
	concept IsStrKey = std::same_as<T, StrKey>;

	template<typename T>
	concept IsCharKey = std::same_as<T, const char*>;

	template<typename T>
	concept IsSelectKey = std::same_as<T, SelectKey>;

	template<typename T>
	concept IsKeyType = IsIndexKey<T> || IsOrderKey<T> || IsStrKey<T> || IsCharKey<T> || IsSelectKey<T>;

}

//callbacks
namespace FKM {

	using AddCallbackType = void(*)(const magic_enum::string_view&, const char*, OrderKey::Type, const std::vector<OrderKey>&);
	using EraseCallbackType = void(*)(const magic_enum::string_view&, const char*, OrderKey::Type);

}

