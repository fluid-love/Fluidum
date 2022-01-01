#pragma once

#include "exception.h"

namespace FKM::Internal {

	template<FU::Concept::IsScopedEnum Enum>
	struct Element final {
	public:
		Element(const Enum type, const OrderKey order, const char* key) noexcept :
			type(type),
			order(order),
			key(key)
		{}
		Element(const Enum type, const OrderKey order, const char* key, Connections&& connections) noexcept :
			type(type),
			order(order),
			key(key),
			connections(std::move(connections))
		{}
		Element(const Enum type, const OrderKey order, const char* key, const Connections& connections) :
			type(type),
			order(order),
			key(key),
			connections(connections)
		{}

		~Element() = default;

		FluidumUtils_Class_Default_Move(Element)
			FluidumUtils_Class_Delete_Copy(Element)

	public://data
		Enum type;
		OrderKey order; //serial number
		StrKey key;     //Duplicate key(StrKey) is not allowed.

	public:
		Connections connections{};
	};

	class OrderCounter final {
	private:
		OrderKey count = 1;
	public:
		//if (count == std::numeric_limits<OrderKey::KeyType>::max()) throw exception
		//strong
		void operator++(int);

		[[nodiscard]] OrderKey next() const noexcept;

	public:
		//get
		[[nodiscard]] operator OrderKey() const noexcept;
		[[nodiscard]] OrderKey current() const noexcept;


	};
}