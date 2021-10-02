#pragma once

#include "exception.h"

namespace Fluidum::KeyManager::Internal {

	template<Utils::Concept::IsScopedEnum Enum>
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
		FluidumUtils_Class_Default_CopyMove(Element)

	public://data
		Enum type = Enum();
		OrderKey order = 0;//serial number
		StrKey key = "";//Duplicate key(StrKey) is not allowed

	public:
		Connections connections = {};
	};

	class OrderCounter final {
	private:
		OrderKey counter = 1;
	public:
		//if (counter == std::numeric_limits<OrderKey::Type>::max()) throw exception
		void operator++(int);

		//get
		operator OrderKey()const noexcept;
	};
}