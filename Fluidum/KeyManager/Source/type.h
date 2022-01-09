#pragma once

#include "../../Utils/include.h"
#include "../../../External/magic_enum/include/magic_enum.hpp"

namespace FKM {

	//Assign a serial number to all items.
	class OrderKey final {
	public:
		using KeyType = UIF32;
	public:
		OrderKey() = default;
		OrderKey(const KeyType val) noexcept;
		FluidumUtils_Class_Default_CopyMove(OrderKey);

	private:
		KeyType index = 0;

	public:
		[[nodiscard]] operator KeyType() const noexcept;
		void operator=(const KeyType val) noexcept;
		[[nodiscard]] bool operator==(const OrderKey key) const noexcept;

		void operator++(int) noexcept;

		[[nodiscard]] bool operator==(KeyType val) const noexcept;

	};

	//Each item type is assigned a serial number.
	class IndexKey final {
	public:
		using KeyType = UIF32;
	public:
		IndexKey() = default;
		IndexKey(const KeyType val) noexcept;
		FluidumUtils_Class_Default_CopyMove(IndexKey);
	private:
		KeyType order = 0;
	public:
		[[nodiscard]] operator KeyType() const noexcept;
		void operator=(const KeyType val) noexcept;
		[[nodiscard]] bool operator==(const IndexKey key) const noexcept;
		[[nodiscard]] bool operator!=(const IndexKey key) const noexcept;
		[[nodiscard]] bool operator==(const KeyType val) const noexcept;

	};

}

namespace FKM {

	using CharKey = const char*;

	using StrKey = std::string;

	enum class SelectKey : UIF8 {
		Front,
		Back
	};

	using Connections = std::vector<OrderKey>;

}

namespace FKM::Concept {

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

	using AddCallbackType = void(*)(const magic_enum::string_view&, const char*, OrderKey, const std::vector<OrderKey>&) noexcept;
	using RemoveCallbackType = void(*)(const magic_enum::string_view&, const char*, OrderKey) noexcept;

}

