#pragma once

#include "type.h"

namespace FD::Calc {
	class ArrayWrite;
	class ArrayRead;
}

namespace FD::Calc::Array::Internal {
	struct Data final {
		FluidumUtils_Class_Delete_ConDestructor(Data)
	private:
		static inline std::vector<std::vector<LuAssist::Num>> numbers{};
		static inline std::vector<std::vector<LuAssist::String>> strings{};
		static inline std::vector<std::vector<bool>> bits{};//bit

		static inline std::mutex mtx{};
	private:
		friend class ArrayWrite;
		friend class ArrayRead;
	};
}

namespace FD::Calc {

	class ArrayWrite final {
	public:
		explicit ArrayWrite(Internal::PassKey) noexcept {};
		~ArrayWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(ArrayWrite)

	public:
		enum class Exception : uint8_t {
			MaxSize
		};
	public:
		/*
		Exception:
			MaxSize -> (Data::xxx.size() >= Limits::MaxSize) 
		*/
		[[nodiscard]] Array::Key make(const Array::ValueType type);

		template<Array::ValueType T>
		void front(const Array::Key key, const Array::ElmType<T> val) const {
			using namespace Array::Internal;
			using enum Array::ValueType;
			std::lock_guard<std::mutex> lock(Data::mtx);

			if constexpr (T == Number)
				Data::numbers[key].front() = val;
			else if constexpr (T == String)
				Data::strings[key].front() = val;
			else if constexpr (T == Bit)
				Data::bits[key].front() = val;
		}

		template<Array::ValueType T>
		void back(const Array::Key key, const Array::ElmType<T> val) const {
			using namespace Array::Internal;
			using enum Array::ValueType;
			std::lock_guard<std::mutex> lock(Data::mtx);

			if constexpr (T == Number)
				Data::numbers[key].back() = val;
			else if constexpr (T == String)
				Data::strings[key].back() = val;
			else if constexpr (T == Bit)
				Data::bits[key].back() = val;
		}


	public:
		void reset();

	private:
		Array::Key generateKey(const Array::ValueType type);
	private:
		struct GarbageManager final {
		public:
			//not found -> return nullopt
			std::optional<Array::Index> findEmpty(const Array::ValueType type) const;
			void reuse(const Array::ValueType type, const Array::Index index);
			void erase(const Array::ValueType type, const Array::Index index);
			void reset();
		private:
			std::vector<std::pair<Array::ValueType, Array::Key>> garbage{};
		}garbage;
	};

	class ArrayRead final {
	public:
		explicit ArrayRead(Internal::PassKey) noexcept {};
		~ArrayRead() = default;
		FluidumUtils_Class_Delete_CopyMove(ArrayRead)

	public:
		template<Array::ValueType T>
		Array::ElmType<T> front(const Array::Key key) const {
			using namespace Array::Internal;
			using enum Array::ValueType;
			std::lock_guard<std::mutex> lock(Data::mtx);

			if constexpr (T == Number)
				return Data::numbers[key].front();
			else if constexpr (T == String)
				return Data::strings[key].front();
			else if constexpr (T == Bit)
				return Data::bits[key].front();
		}

		template<Array::ValueType T>
		Array::ElmType<T> back(const Array::Key key) const {
			using namespace Array::Internal;
			using enum Array::ValueType;
			std::lock_guard<std::mutex> lock(Data::mtx);

			if constexpr (T == Number)
				return Data::numbers[key].back();
			else if constexpr (T == String)
				return Data::strings[key].back();
			else if constexpr (T == Bit)
				return Data::bits[key].back();
		}
	};

}