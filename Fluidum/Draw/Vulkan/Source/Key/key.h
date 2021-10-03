#pragma once

#include "type.h"

namespace FVK::Internal::Key {

	//StringKey‚É‚½‚¢‚µ‚Ä‚Í“ÁŽê‰»
	template<FvkType FType, IsKeyType KType>
	class Key final {
		KType key;
	public:
		using KeyType = KType;
	private:
		static constexpr inline FvkType Type = FType;
	public:
		constexpr explicit Key(const KType key) noexcept :key(key) {}
		FluidumUtils_Class_Default_CopyMove(Key)

	public:
		constexpr void set(const KType key)noexcept {
			this->key = key;
		}
		_NODISCARD constexpr KType get()const noexcept {
			return this->key;
		}
		constexpr void operator=(const KType key) noexcept {
			this->key = key;
		}
		constexpr operator KType() const noexcept {
			return key;
		}
	private:
		template<FvkType>
		friend class RequiredDataKeysBase;
		friend class Converter;
		friend class ::FVK::Internal::Manager::Manager;
	};

	//string‚É‘Î‚µ‚Ä“ÁŽê‰»
	template<FvkType FType>
	class Key<FType, std::string> final {
		std::string key;
	public:
		using KeyType = std::string;
	private:
		static constexpr inline FvkType Type = FType;
	public:
		FluidumUtils_Class_Default_CopyMove(Key)
			explicit Key(const std::string& key) :key(key) {}
		explicit Key(std::string&& key) noexcept :key(std::move(key)) {}

	public:
		void set(const std::string& key) {
			this->key = key;
		}
		_NODISCARD std::string get()const noexcept {
			return this->key;
		}
		void operator=(const std::string& key) {
			this->key = key;
		}
		void operator=(std::string&& key) noexcept {
			this->key = std::move(key);
		}

		operator const char* () const noexcept {
			return key.c_str();
		}
	private:
		template<FvkType>
		friend class RequiredDataKeysBase;
		friend class Converter;
		friend class ::FVK::Internal::Manager::Manager;
	};

}