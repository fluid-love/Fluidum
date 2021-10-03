#pragma once

#include "info.h"

namespace FKM {

	template<FU::Concept::IsScopedEnum Enum>
	class KeyManager final : protected std::vector<Internal::Element<Enum>> {
	public:
		KeyManager() = default;
		~KeyManager() = default;

	private://using
		using Element = Internal::Element<Enum>;
		using Elements = std::vector<Element>;
		using Elms = Elements;
		using ConstIterator = Elements::const_iterator;
		using Iterator = Elements::iterator;

	private:
		Internal::OrderCounter counter = {};

		AddCallbackType addCallback = [](const magic_enum::string_view&, const char*, OrderKey::Type, const std::vector<OrderKey>&) {};
		EraseCallbackType eraseCallback = [](const magic_enum::string_view&, const char*, OrderKey::Type) {};

	public:
		void setAddCallback(AddCallbackType callback) {
			this->addCallback = callback;
		}
		void setEraseCallback(EraseCallbackType callback) {
			this->eraseCallback = callback;
		}

	public://Add 

		template<Enum Type>
		void add(const char* key) {

			this->checkCollisionKeys(key);

			const auto order = this->plusOrder();

			Elms::emplace_back(Element(Type, order, key));

			this->addCallback(magic_enum::enum_name<Type>(), key, order, {});
		}

		//runtime
		template<Enum Type>
		void add_recursive(const char* key, const std::vector<std::pair<Enum, IndexKey>>& connections) {

			this->add<Type, false>(key, connections);

			Connections result = Elms::back().connections;
			result.reserve(100);

			std::size_t first = 0;
			std::size_t last = result.size();

			while (first != last) {
				for (std::size_t i = first, size = result.size(); i < size; i++) {
					first = last;
					const Connections& temp = this->connectionsRef(result[i]);
					result.insert(result.end(), temp.begin(), temp.end());
					last = result.size();
				}
			}

			FU::Container::sortAscendingOrder_EraseDuplicateElms(result);

			result.shrink_to_fit();

			Elms::back().connections = std::move(result);
			this->addCallback(magic_enum::enum_name<Type>(), key, currentOrder() - 1, Elms::back().connections);
		}

		template<Enum Type, auto TypeArray, FU::Concept::IsStdArrayParticularType<IndexKey> IndexArray>
		void add(const char* key, const IndexArray& connections)
			requires FU::Concept::IsStdArrayParticularType<decltype(TypeArray), Enum>
		{
			static_assert(TypeArray.size() == std::tuple_size_v<IndexArray>, "TypeArray.size() != IndexArray.size()");

			this->checkCollisionKeys(key);

			std::vector<OrderKey> result(std::tuple_size_v<IndexArray>);

			FluidumUtils_Debug_BeginDisableWarning(28020)
				if constexpr (std::tuple_size_v<decltype(TypeArray)> > 0) {
					for (std::size_t i = 0, size = result.size(); i < size; i++) {
						result[i] = toOrderKey(TypeArray[i], connections[i]);
					}
				}
			FluidumUtils_Debug_EndDisableWarning

				const auto order = this->plusOrder();

			Elms::emplace_back(Element(Type, order, key, std::move(result)));
			this->addCallback(magic_enum::enum_name<Type>(), key, order, Elms::back().connections);
		}

		template<Enum Type, bool Log = true>
		void add(const char* key, const std::vector<std::pair<Enum, IndexKey>>& connections) {

			this->checkCollisionKeys(key);

			//connections
			std::vector<OrderKey> result(connections.size());
			for (std::size_t i = 0; auto & x : result) {
				x = toOrderKey(connections[i].first, connections[i].second);
				i++;
			}

			const auto order = this->plusOrder();

			Elms::emplace_back(Element(Type, order, key, std::move(result)));
			if constexpr (Log)
				this->addCallback(magic_enum::enum_name<Type>(), key, order, Elms::back().connections);
		}

	public://Erase

		void erase(CharKey key) {
			const auto type = toType(key);
			const auto order = toOrderKey(key);

			const auto itr = find(key);
			this->checkNotFound(itr);

			//if (this->isKeyConnected(key)) {
			//	Exception::Internal::throwFailedToErase(key);
			//}

			Elms::erase(itr);

			this->eraseCallback(magic_enum::enum_name(type), key, order);
		}

		void erase(const StrKey& key) {
			this->erase(key.c_str());
		}

		void erase(const Enum type, const IndexKey key) {
			const StrKey strKey = toStrKey(type, key);
			this->erase(strKey);
		}



		_NODISCARD OrderKey toOrderKey(CharKey key) const {
			const auto itr = this->find(key);
			this->checkNotFound(itr);
			return itr->order;
		}
		_NODISCARD OrderKey toOrderKey(const StrKey& key) const {
			return this->toOrderKey(key.c_str());
		}
		_NODISCARD OrderKey toOrderKey(const Enum type, const IndexKey key) const {
			const ConstIterator itr = this->find(type, key);
			this->checkNotFound(itr);
			return itr->order;
		}
		_NODISCARD OrderKey toOrderKey(const OrderKey key) const {
			return key;
		}
		_NODISCARD OrderKey toOrderKey(const Enum type, const SelectKey key) const {
			const ConstIterator itr = this->find(type, key);
			this->checkNotFound(itr);
			return itr->order;
		}





		_NODISCARD IndexKey toIndexKey(CharKey key) const {
			const auto order = toOrderKey(key);
			return toIndexKey(order);
		}
		_NODISCARD IndexKey toIndexKey(const StrKey& key) const {
			return this->toIndexKey(key.c_str());
		}
		_NODISCARD IndexKey toIndexKey(const Enum type, const IndexKey key) const {
			return key;
		}
		_NODISCARD IndexKey toIndexKey(const OrderKey key) const {
			const ConstIterator itr = this->find(key);
			this->checkNotFound(itr);

			const Enum type = itr->type;

			const auto count = std::count_if(Elms::cbegin(), itr, [=](const Element& x) {return x.type == type; });

			return IndexKey(static_cast<IndexKey::Type>(count));
		}
		_NODISCARD IndexKey toIndexKey(const Enum type, const SelectKey key) const {
			const ConstIterator itr = this->find(type, key);
			this->checkNotFound(itr);
			return this->toIndexKey(itr->order);
		}




		_NODISCARD StrKey toStrKey(CharKey key) const {
			return std::string(key);
		}
		_NODISCARD StrKey toStrKey(const StrKey& key) const {
			return key;
		}
		_NODISCARD StrKey toStrKey(const Enum type, const IndexKey key) const {
			const ConstIterator itr = find(type, key);
			this->checkNotFound(itr);
			return itr->key;
		}
		_NODISCARD StrKey toStrKey(const OrderKey key) const {
			const ConstIterator itr = find(key);
			this->checkNotFound(itr);
			return itr->key;
		}
		_NODISCARD StrKey toStrKey(const Enum type, const SelectKey key) const {
			const ConstIterator itr = this->find(type, key);
			this->checkNotFound(itr);
			return itr->key;
		}



		_NODISCARD Enum toType(CharKey key) const {
			const ConstIterator itr = find(key);
			this->checkNotFound(itr);
			return itr->type;
		}
		_NODISCARD Enum toType(const StrKey& key) const {
			return this->toType(key.c_str());
		}
		_NODISCARD Enum toType(const Enum type, const IndexKey key) const {
			const ConstIterator itr = find(type, key);
			this->checkNotFound(itr);
			return itr->type;
		}
		_NODISCARD Enum toType(const OrderKey key) const {
			const ConstIterator itr = find(key);
			this->checkNotFound(itr);
			return itr->type;
		}
		_NODISCARD Enum toType(const Enum type, const SelectKey key) const {
			const ConstIterator itr = this->find(type, key);
			this->checkNotFound(itr);
			return itr->type;
		}



		_NODISCARD bool isKeyCollision(CharKey key) const {
			const ConstIterator itr = this->find(key);
			return itr != Elms::cend();
		}
		_NODISCARD bool isKeyCollision(const Enum type, const IndexKey key) const {
			const ConstIterator itr = this->find(type, key);
			return itr != Elms::cend();
		}
		_NODISCARD bool isKeyCollision(const StrKey& key) const {
			return this->isKeyCollision(key.c_str());
		}
		_NODISCARD bool isKeyCollision(const OrderKey key) const {
			const ConstIterator itr = this->find(key);
			return itr != Elms::cend();
		}
		_NODISCARD bool isKeyCollision(const Enum type, const SelectKey key) const {
			const ConstIterator itr = this->find(type, key);
			return itr != Elms::cend();
		}





		_NODISCARD bool isKeyConnected(CharKey key, CharKey find) const {
			const OrderKey order = toOrderKey(find);
			const Connections& connections = connectionsRef(key);
			const auto itr = std::find_if(connections.cbegin(), connections.cend(), [=](const auto& x) {return x == order; });
			if (itr != connections.cend())
				return true;
			return false;
		}

		_NODISCARD bool isKeyConnected(CharKey key) const {
			const OrderKey order = toOrderKey(key);
			for (std::size_t i = 0, size = Elms::size(); i < size; i++) {
				const Connections& connections = connectionsRef(Elms::operator[](i).order);
				const auto itr = std::find_if(connections.cbegin(), connections.cend(), [=](const auto& x) {return x == order; });

				if (itr != connections.cend())
					return true;
			}

			return false;
		}


		//return first key
		_NODISCARD IndexKey toConnectionIndexKey(CharKey key, const Enum type) const {
			return this->toConnectionIndexKey(this->toOrderKey(key), type);
		}
		_NODISCARD IndexKey toConnectionIndexKey(const StrKey& key, const Enum type) const {
			return this->toConnectionIndexKey(key.c_str(), type);
		}
		_NODISCARD IndexKey toConnectionIndexKey(const IndexKey key, const Enum type) const {
			return key;
		}
		_NODISCARD IndexKey toConnectionIndexKey(const OrderKey key, const Enum type) const {
			const Connections& connections = this->connectionsRef(key);
			for (auto& x : connections) {
				const ConstIterator itr = this->find(x);
				this->checkNotFound(itr);
				if (itr->type == type)
					return this->toIndexKey(x);
			}
			Exception::Internal::throwNotFound();
		}
		_NODISCARD IndexKey toConnectionIndexKey(const SelectKey key, const Enum sType, const Enum type) const {
			return this->toConnectionIndexKey(this->toOrderKey(key, sType), type);
		}


		_NODISCARD bool isEmpty() const noexcept {
			return Elms::empty();
		}


	private://find
		_NODISCARD ConstIterator find(const StrKey& key) const {
			return find(key.c_str());
		}

		_NODISCARD ConstIterator find(CharKey key) const {
			const auto func = [=](const Element& x)->bool {
				//basic_string operator==
				return operator ==(x.key, key);
			};
			return std::find_if(Elms::cbegin(), Elms::cend(), func);
		}

		_NODISCARD ConstIterator find(const OrderKey key) const {
			const auto func = [=](const Element& x)->bool {
				return x.order == key;
			};
			return std::find_if(Elms::cbegin(), Elms::cend(), func);
		}

		_NODISCARD ConstIterator find(const Enum type, const IndexKey key) const {
			const auto func = [=](const Element& x)->bool {
				return x.type == type;
			};

			IndexKey::Type count = 0;
			ConstIterator itr = Elms::cbegin();
			for (auto end = Elms::cend(); itr != end; itr++) {
				if (itr->type == type) {
					if (count == key)
						break;
					count++;
				}
			}

			return itr;
		}

		_NODISCARD ConstIterator find(const Enum type, const SelectKey key) const {
			const auto func = [=](const Element& x)->bool {
				return x.type == key;
			};
			if (key == SelectKey::Front) {
				return std::find_if(Elms::cbegin(), Elms::cend(), func);
			}
			else if (key == SelectKey::Back) {
				const auto itr = std::find_if(Elms::crbegin(), Elms::crend(), func).base();
				if (itr == Elms::cbegin())
					return Elms::cend();//無効表現をendに統一
				return itr;
			}
			assert(false);
		}




		//get Connection
		_NODISCARD Connections connections(const StrKey& key) const {
			const ConstIterator itr = this->find(key.c_str());
			return itr->connections;
		}
		_NODISCARD Connections connections(CharKey key) const {
			const ConstIterator itr = this->find(key);
			return itr->connections;
		}
		_NODISCARD Connections connections(const Enum type, const IndexKey key) const {
			const ConstIterator itr = this->find(type, key);
			return itr->connections;
		}
		_NODISCARD Connections connections(const OrderKey key) const {
			const ConstIterator itr = this->find(key);
			return itr->connections;
		}
		_NODISCARD Connections connections(const Enum type, const SelectKey key) const {
			const ConstIterator itr = this->find(type, key);
			return itr->connections;
		}



		//get const Connection reference
		_NODISCARD const Connections& connectionsRef(const StrKey& key) const {
			const ConstIterator itr = find(key.c_str());
			return itr->connections;
		}
		_NODISCARD const Connections& connectionsRef(CharKey key) const {
			const ConstIterator itr = find(key);
			return itr->connections;
		}
		_NODISCARD const Connections& connectionsRef(const Enum type, const IndexKey key) const {
			const ConstIterator itr = find(type, key);
			return itr->connections;
		}
		_NODISCARD const Connections& connectionsRef(const OrderKey key) const {
			const ConstIterator itr = this->find(key);
			return itr->connections;
		}
		_NODISCARD const Connections& connectionsRef(const Enum type, const SelectKey key) const {
			const ConstIterator itr = this->find(type, key);
			return itr->connections;
		}



	private:
		void checkNotFound(const ConstIterator itr) const {
			if (itr == Elms::cend())
				Exception::Internal::throwNotFound();
		}

		void checkCollisionKeys(const char* key) const {
			if (this->isKeyCollision(key))
				Exception::Internal::throwCollisionOfKeys(key);
		}
 
		_NODISCARD OrderKey nextOrder() const {
			return counter + 1;
		}

		_NODISCARD OrderKey currentOrder() const {
			return counter;
		}

		//get current and plus 1
		OrderKey plusOrder() {
			const auto result = this->currentOrder();
			counter++;
			return result;
		}

	};



}