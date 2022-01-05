#pragma once

#include "type.h"

namespace FVK::Internal::Key {

	//Specialization for StrKey(std::string)
	template<FvkType FType, ::FKM::Concept::IsKeyType KType>
	class Key final {
	private:
		KType key;
	public:
		using KeyType = KType;
		static constexpr inline FvkType Type = FType;

	public:
		constexpr explicit Key(const KType key) noexcept : key(key) {}
		FluidumUtils_Class_Default_CopyMove(Key)

	public:
		//no-throw
		constexpr void set(const KType key) noexcept {
			this->key = key;
		}
		constexpr void operator=(const KType key) noexcept {
			this->key = key;
		}
		[[nodiscard]] constexpr KType get() const noexcept {
			return this->key;
		}
		[[nodiscard]] constexpr operator KType() const noexcept {
			return this->key;
		}

	private:
		template<FvkType>
		friend class RequiredDataKeys;
		friend class Converter;
		friend class ::FVK::Internal::Manager::Manager;
	};

	template<FvkType FType>
	class Key<FType, std::string> final {
		std::string key;
	public:
		using KeyType = StrKey;
		static constexpr inline FvkType Type = FType;

	public:
		explicit Key(const std::string& key) : key(key) {}
		explicit Key(std::string&& key) noexcept : key(std::move(key)) {}
		FluidumUtils_Class_Default_CopyMove(Key)

	public:
		//std::exception
		//strong
		void set(const std::string& key) {
			//https://eel.is/c++draft/string.classes#string.require-2
			//operator= is strong.
			this->key = key;
		}
		void operator=(const std::string& key) {
			this->key = key;
		}

		//strong
		[[nodiscard]] std::string get() const {
			return this->key;
		}

		//no-throw
		void operator=(std::string&& key) noexcept {
			this->key = std::move(key);
		}
		operator const char* () const noexcept {
			return key.c_str();
		}

	private:
		template<FvkType>
		friend class RequiredDataKeys;
		friend class Converter;
		friend class ::FVK::Internal::Manager::Manager;
	};

}