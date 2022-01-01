#pragma once

#include "info.h"

namespace FKM {

	/*
	All public functions of KeyManager are not changed internally even if an exception is thrown.
	*/
	template<FU::Concept::IsScopedEnum Enum>
	class KeyManager final : protected std::vector<Internal::Element<Enum>> {
	public://assert
		//https://eel.is/c++draft/vector.modifiers
		static_assert(std::is_nothrow_move_constructible_v<Internal::Element<Enum>>);

		static_assert(std::is_move_constructible_v<Internal::Element<Enum>>);

	public:
		FluidumUtils_Class_Default_ConDestructor(KeyManager)
			FluidumUtils_Class_Delete_CopyMove(KeyManager)

	private://using
		using Element = Internal::Element<Enum>;
		using Elements = std::vector<Element>;

		using Iterator = Elements::iterator;
		using ConstIterator = Elements::const_iterator;

	private:
		Internal::OrderCounter counter{};

		AddCallbackType addCallback = [](const magic_enum::string_view&, const char*, OrderKey, const std::vector<OrderKey>&) {};
		EraseCallbackType eraseCallback = [](const magic_enum::string_view&, const char*, OrderKey) {};

	public:
		void setAddCallback(AddCallbackType callback) {
			this->addCallback = callback;
		}
		void setEraseCallback(EraseCallbackType callback) {
			this->eraseCallback = callback;
		}

	public://add
		/*
		Exception:
			std::exception
			CollisionOfKeys
			ReachIndexLimit

			callback
		*/
		//strong
		template<Enum Type>
		void add(const char* key) {
			//CollisionOfKeys
			this->checkKeyCollided(key);

			//noexcept
			const auto order = counter;

			this->emplaceBackElement(Element(Type, order, key));

			//ReachIndexLimit
			counter++;

			this->addCallback(magic_enum::enum_name<Type>(), key, order, {});
		}

		/*
		Exception:
			std::exception
			CollisionOfKeys
			ReachIndexLimit
			NotFound

			callback
		*/
		//strong
		template<Enum Type>
		void add_recursive(const char* key, const std::vector<std::pair<Enum, IndexKey>>& connections) {

			//strong
			this->add<Type, false>(key, connections);

			//vector::back never throws exceptions. (except that the vector is empty).
			assert(!Elements::empty());
			Connections result = Elements::back().connections;

			Size first = 0;
			Size last = static_cast<Size>(result.size());

			while (first != last) {
				for (Size i = first, size = result.size(); i < size; i++) {
					first = last;
					const Connections& temp = this->connectionsRef(result[i]);
					result.insert(result.end(), temp.begin(), temp.end());
					last = result.size();
				}
			}

			FU::Container::sortAscendingOrder_EraseDuplicateElms(result);

			Elements::back().connections = std::move(result);

			this->addCallback(magic_enum::enum_name<Type>(), key, counter.current() - 1, Elements::back().connections);
		}


		/*
		Exception:
			std::exception
			CollisionOfKeys
			ReachIndexLimit
			NotFound (size > 0)

			callback
		*/
		//strong
		template<Enum Type, auto TypeArray, FU::Concept::IsStdArrayParticularType<IndexKey> IndexArray>
		void add(const char* key, const IndexArray& connections)
			requires FU::Concept::IsStdArrayParticularType<decltype(TypeArray), Enum>
		{
			static_assert(TypeArray.size() == std::tuple_size_v<IndexArray>, "TypeArray.size() != IndexArray.size()");

			//CollisionOfKeys
			this->checkKeyCollided(key);

			std::vector<OrderKey> result(std::tuple_size_v<IndexArray>);

			if constexpr (std::tuple_size_v<decltype(TypeArray)> > 0) {
				for (Size i = 0, size = result.size(); i < size; i++) {
					//NotFound
					result[i] = this->toOrderKey(TypeArray[i], connections[i]);
				}
			}

			//noexception
			const auto order = counter.current();

			//StdException
			//Element::Emelent -> noexception
			this->emplaceBackElement(Element(Type, order, key, std::move(result)));

			//ReachIndexLimit
			counter++;

			this->addCallback(magic_enum::enum_name<Type>(), key, order, Elements::back().connections);
		}
		template<Enum Type, bool Log = true>
		void add(const char* key, const std::vector<std::pair<Enum, IndexKey>>& connections) {

			//CollisionOfKeys
			this->checkKeyCollided(key);

			//connections
			std::vector<OrderKey> result(connections.size());
			for (Size i = 0; auto & x : result) {
				x = this->toOrderKey(connections[i].first, connections[i].second);
				i++;
			}

			//noexception
			const auto order = counter.current();

			//StdException
			//Element::Emelent -> noexception
			this->emplaceBackElement(Element(Type, order, key, std::move(result)));

			//ReachIndexLimit
			counter++;

			if constexpr (Log)
				this->addCallback(magic_enum::enum_name<Type>(), key, order, Elements::back().connections);
		}

	private:
		//strong
		void emplaceBackElement(Element&& elm) {
			//Element satisfies is_nothrow_move_constructible_v
			//https://eel.is/c++draft/vector.modifiers
			Elements::emplace_back(std::move(elm));
		}

	public://remove
		/*
		Exception:
			std::exception
			NotFound
			FailedToRemove

			callback
		*/
		//strong
		void remove(const char* key) {
			//NotFound
			const auto type = this->toType(key);
			//NotFound
			const auto order = this->toOrderKey(key);

			const auto itr = this->find(key);
			assert(itr != Elements.cend());

			if (this->isKeyConnected(key))
				Exception::Internal::throwFailedToRemove();//FailedToRemove

			//This function(erase) does not throw an exception
			//because Element satisfies is_nothrow_move_constructible_v and is_move_constructible.
			Elements::erase(itr);

			this->eraseCallback(magic_enum::enum_name(type), key, order);
		}
		void remove(const StrKey& key) {
			//c_str() no-throw
			this->remove(key.c_str());
		}
		void remove(const Enum type, const IndexKey key) {
			const StrKey strKey = this->toStrKey(type, key);
			this->remove(strKey);
		}

	public:
		/*
		Exception:
			std::exception
			NotFound
		*/
		//strong
		[[nodiscard]] OrderKey toOrderKey(CharKey key) const {
			const auto itr = this->find(key);
			this->checkNotFound(itr);//NotFound
			return itr->order;
		}
		[[nodiscard]] OrderKey toOrderKey(const StrKey& key) const {
			return this->toOrderKey(key.c_str());//NotFound
		}
		[[nodiscard]] OrderKey toOrderKey(const Enum type, const IndexKey key) const {
			const ConstIterator itr = this->find(type, key);
			this->checkNotFound(itr);//NotFound
			return itr->order;
		}
		[[nodiscard]] OrderKey toOrderKey(const OrderKey key) const {
			const ConstIterator itr = this->find(key);
			this->checkNotFound(itr);//NotFound
			return key;
		}
		[[nodiscard]] OrderKey toOrderKey(const Enum type, const SelectKey key) const {
			const ConstIterator itr = this->find(type, key);
			this->checkNotFound(itr);//NotFound
			return itr->order;
		}

	public:
		/*
		Exception:
			std::exception
			NotFound
		*/
		//strong
		[[nodiscard]] IndexKey toIndexKey(CharKey key) const {
			const auto order = toOrderKey(key);//NotFound
			return toIndexKey(order);
		}
		[[nodiscard]] IndexKey toIndexKey(const StrKey& key) const {
			return this->toIndexKey(key.c_str());//NotFound
		}
		[[nodiscard]] IndexKey toIndexKey(const Enum type, const IndexKey key) const {
			const auto count = std::count_if(Elements::cbegin(), Elements::cend(), [=](const Element& x) {return x.type == type; });
			if (count <= key)
				Exception::Internal::throwNotFound();//NotFound
			return key;
		}
		[[nodiscard]] IndexKey toIndexKey(const OrderKey key) const {
			const ConstIterator itr = this->find(key);
			this->checkNotFound(itr);//NotFound

			const Enum type = itr->type;

			const auto count = std::count_if(Elements::cbegin(), itr, [=](const Element& x) {return x.type == type; });

			return IndexKey(static_cast<IndexKey::KeyType>(count));
		}
		[[nodiscard]] IndexKey toIndexKey(const Enum type, const SelectKey key) const {
			const ConstIterator itr = this->find(type, key);
			return this->toIndexKey(itr->order);//NotFound
		}

	public:
		/*
		Exception:
			std::exception
			NotFound
		*/
		//strong
		[[nodiscard]] StrKey toStrKey(CharKey key) const {
			return this->toStrKey(key);
		}
		[[nodiscard]] StrKey toStrKey(const StrKey& key) const {
			const ConstIterator itr = find(key);
			this->checkNotFound(itr);
			return key;
		}
		[[nodiscard]] StrKey toStrKey(const Enum type, const IndexKey key) const {
			const ConstIterator itr = find(type, key);
			this->checkNotFound(itr);
			return itr->key;
		}
		[[nodiscard]] StrKey toStrKey(const OrderKey key) const {
			const ConstIterator itr = find(key);
			this->checkNotFound(itr);
			return itr->key;
		}
		[[nodiscard]] StrKey toStrKey(const Enum type, const SelectKey key) const {
			const ConstIterator itr = this->find(type, key);
			this->checkNotFound(itr);
			return itr->key;
		}

	public:
		/*
		Exception:
			std::exception
			NotFound
		*/
		//strong
		[[nodiscard]] Enum toType(CharKey key) const {
			const ConstIterator itr = find(key);
			this->checkNotFound(itr);//NotFound
			return itr->type;
		}
		[[nodiscard]] Enum toType(const StrKey& key) const {
			return this->toType(key.c_str());//NotFound
		}
		[[nodiscard]] Enum toType(const Enum type, const IndexKey key) const {
			const ConstIterator itr = find(type, key);
			this->checkNotFound(itr);//NotFound
			return itr->type;
		}
		[[nodiscard]] Enum toType(const OrderKey key) const {
			const ConstIterator itr = find(key);
			this->checkNotFound(itr);//NotFound
			return itr->type;
		}
		[[nodiscard]] Enum toType(const Enum type, const SelectKey key) const {
			const ConstIterator itr = this->find(type, key);
			this->checkNotFound(itr);//NotFound
			return itr->type;
		}

	public:
		/*
		Exception:
			std::exception
		*/
		//strong
		[[nodiscard]] bool isKeyCollided(CharKey key) const {
			const ConstIterator itr = this->find(key);
			return itr != Elements::cend();
		}
		[[nodiscard]] bool isKeyCollided(const StrKey& key) const {
			return this->isKeyCollided(key.c_str());
		}
		[[nodiscard]] bool isKeyCollided(const OrderKey key) const {
			const ConstIterator itr = this->find(key);
			return itr != Elements::cend();
		}
		[[nodiscard]] bool isKeyCollided(const Enum type, const IndexKey key) const {
			const ConstIterator itr = this->find(type, key);
			return itr != Elements::cend();
		}
		[[nodiscard]] bool isKeyCollided(const Enum type, const SelectKey key) const {
			const ConstIterator itr = this->find(type, key);
			return itr != Elements::cend();
		}

	public:
		[[nodiscard]] bool isKeyConnected(CharKey key, CharKey find) const {
			const OrderKey order = toOrderKey(find);
			const Connections& connections = connectionsRef(key);
			const auto itr = std::find_if(connections.cbegin(), connections.cend(), [=](const auto& x) {return x == order; });
			if (itr != connections.cend())
				return true;
			return false;
		}

		[[nodiscard]] bool isKeyConnected(CharKey key) const {
			const OrderKey order = toOrderKey(key);
			for (Size i = 0, size = Elements::size(); i < size; i++) {
				const Connections& connections = connectionsRef(Elements::operator[](i).order);
				const auto itr = std::find_if(connections.cbegin(), connections.cend(), [=](const auto& x) {return x == order; });

				if (itr != connections.cend())
					return true;
			}

			return false;
		}

	public:
		//return first key
		[[nodiscard]] IndexKey toConnectionIndexKey(CharKey key, const Enum type) const {
			return this->toConnectionIndexKey(this->toOrderKey(key), type);
		}
		[[nodiscard]] IndexKey toConnectionIndexKey(const StrKey& key, const Enum type) const {
			return this->toConnectionIndexKey(key.c_str(), type);
		}
		[[nodiscard]] IndexKey toConnectionIndexKey(const IndexKey key, const Enum type) const {
			return key;
		}
		[[nodiscard]] IndexKey toConnectionIndexKey(const OrderKey key, const Enum type) const {
			const Connections& connections = this->connectionsRef(key);
			for (auto& x : connections) {
				const ConstIterator itr = this->find(x);
				this->checkNotFound(itr);
				if (itr->type == type)
					return this->toIndexKey(x);
			}
			Exception::Internal::throwNotFound();
		}
		[[nodiscard]] IndexKey toConnectionIndexKey(const SelectKey key, const Enum sType, const Enum type) const {
			return this->toConnectionIndexKey(this->toOrderKey(key, sType), type);
		}

	private://find
		/*
		Exception:
			std::exception
		*/
		//strong
		//If not found, return cend.
		[[nodiscard]] ConstIterator find(CharKey key) const {
			const auto func = [=](const Element& x)->bool {
				//basic_string operator==
				return operator ==(x.key, key);
			};
			return std::find_if(Elements::cbegin(), Elements::cend(), func);
		}
		[[nodiscard]] ConstIterator find(const StrKey& key) const {
			return find(key.c_str());
		}
		[[nodiscard]] ConstIterator find(const OrderKey key) const {
			const auto func = [=](const Element& x)->bool {
				return x.order == key;//no-throw
			};
			return std::find_if(Elements::cbegin(), Elements::cend(), func);
		}
		[[nodiscard]] ConstIterator find(const Enum type, const IndexKey key) const {
			IndexKey::KeyType count = 0;
			ConstIterator itr = Elements::cbegin();
			for (auto end = Elements::cend(); itr != end; itr++) {
				if (itr->type == type) {
					if (count == key)
						break;
					count++;
				}
			}
			return itr;
		}
		[[nodiscard]] ConstIterator find(const Enum type, const SelectKey key) const {
			const auto func = [=](const Element& x)->bool {
				return x.type == key;//no-throw
			};
			if (key == SelectKey::Front) {
				return std::find_if(Elements::cbegin(), Elements::cend(), func);
			}
			else if (key == SelectKey::Back) {
				const auto itr = std::find_if(Elements::crbegin(), Elements::crend(), func).base();
				if (itr == Elements::cbegin())
					return Elements::cend();
				return itr;
			}
			assert(false);
			return Elements::cend();
		}

	public:
		/*
		Exception:
			std::exception
			NotFound
		*/
		//strong
		[[nodiscard]] Connections connections(const StrKey& key) const {
			const ConstIterator itr = this->find(key.c_str());
			this->checkNotFound(itr);
			return itr->connections;
		}
		[[nodiscard]] Connections connections(CharKey key) const {
			const ConstIterator itr = this->find(key);
			this->checkNotFound(itr);
			return itr->connections;
		}
		[[nodiscard]] Connections connections(const Enum type, const IndexKey key) const {
			const ConstIterator itr = this->find(type, key);
			this->checkNotFound(itr);
			return itr->connections;
		}
		[[nodiscard]] Connections connections(const OrderKey key) const {
			const ConstIterator itr = this->find(key);
			this->checkNotFound(itr);
			return itr->connections;
		}
		[[nodiscard]] Connections connections(const Enum type, const SelectKey key) const {
			const ConstIterator itr = this->find(type, key);
			this->checkNotFound(itr);
			return itr->connections;
		}

	public:
		/*
		Exception:
			std::exception
			NotFound
		*/
		//strong
		//return "const" ref.
		[[nodiscard]] const Connections& connectionsRef(const StrKey& key) const {
			const ConstIterator itr = find(key.c_str());
			this->checkNotFound(itr);
			return itr->connections;
		}
		[[nodiscard]] const Connections& connectionsRef(CharKey key) const {
			const ConstIterator itr = find(key);
			this->checkNotFound(itr);
			return itr->connections;
		}
		[[nodiscard]] const Connections& connectionsRef(const Enum type, const IndexKey key) const {
			const ConstIterator itr = find(type, key);
			this->checkNotFound(itr);
			return itr->connections;
		}
		[[nodiscard]] const Connections& connectionsRef(const OrderKey key) const {
			const ConstIterator itr = this->find(key);
			this->checkNotFound(itr);
			return itr->connections;
		}
		[[nodiscard]] const Connections& connectionsRef(const Enum type, const SelectKey key) const {
			const ConstIterator itr = this->find(type, key);
			this->checkNotFound(itr);
			return itr->connections;
		}

	private:
		/*
		Exception:
			std::exception
			NotFound
		*/
		//strong
		void checkNotFound(const ConstIterator itr) const {
			if (itr == Elements::cend())
				Exception::Internal::throwNotFound();
		}

		/*
		Exception:
			std::exception
			CollisionOfKeys
		*/
		//strong
		void checkKeyCollided(const char* key) const {
			if (this->isKeyCollided(key))
				Exception::Internal::throwCollisionOfKeys();
		}
	};
}