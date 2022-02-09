#pragma once

#include "type.h"

namespace FD::Calc {
	class ArrayWrite;
	class ArrayRead;
}

namespace FD::Calc::Array::Internal {
	struct Data final {
		FluidumUtils_Class_Delete_ConDestructor(Data);
	private:
		static inline std::vector<std::vector<LuAssist::Num>> numbers{};
		static inline std::vector<std::vector<std::string>> strings{};
		static inline std::vector<std::vector<bool>> bits{};//bit

		static inline std::mutex mtx{};
	private:
		template<Array::ValueType T>
		static inline auto& arrayRef(const Key key) {
			using enum ValueType;
			if constexpr (T == Number)
				return numbers[key];
			else if constexpr (T == String)
				return strings[key];
			else if constexpr (T == Bit)
				return bits[key];
			else
				FU::Concept::DelayAssert_N<T>;
		}
	private:
		friend class ArrayWrite;
		friend class ArrayRead;
	};

}

namespace FD::Calc {

	class ArrayWrite final {
	public:
		explicit ArrayWrite(::FD::Internal::PassKey) noexcept {};
		~ArrayWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(ArrayWrite);

	public:
		enum class Exception : uint8_t {
			MaxSize,
			Range
		};

	public:
		/*
		Exception:
			MaxSize -> (Data::xxx.size() >= Limits::MaxSize)
		*/
		[[nodiscard]] Array::Key make(const Array::ValueType type);

	public:
		/*
		Exception:
			Range -> (Data::xxx.size() >= index)
		*/
		template<Array::ValueType T>
		void at(const Array::Key key, const std::size_t index, const Array::ElmType<T>& val) const {
			using namespace Array::Internal;
			std::lock_guard<std::mutex> lock(Data::mtx);

			try {
				Data::arrayRef<T>(key).at(index) = val;
			}
			catch (const std::out_of_range&) {
				throw Exception::Range;
			}
		}

		template<Array::ValueType T>
		void front(const Array::Key key, const Array::ElmType<T>& val) const {
			using namespace Array::Internal;
			std::lock_guard<std::mutex> lock(Data::mtx);

			Data::arrayRef<T>(key).front() = val;
		}

		template<Array::ValueType T>
		void back(const Array::Key key, const Array::ElmType<T>& val) const {
			using namespace Array::Internal;
			std::lock_guard<std::mutex> lock(Data::mtx);

			Data::arrayRef<T>(key).back() = val;
		}

		template<Array::ValueType T>
		void push_front(const Array::Key key, const Array::ElmType<T>& val) const {
			using namespace Array::Internal;
			std::lock_guard<std::mutex> lock(Data::mtx);

			auto& ref = Data::arrayRef<T>(key);
			ref.insert(ref.begin(), val);
		}

		template<Array::ValueType T>
		void push_front(const Array::Key key, std::vector<Array::ElmType<T>>&& vals) const {
			using namespace Array::Internal;
			std::lock_guard<std::mutex> lock(Data::mtx);

			auto& ref = Data::arrayRef<T>(key);
			ref.insert(ref.begin(), std::make_move_iterator(vals.begin()), std::make_move_iterator(vals.end()));
		}

		template<Array::ValueType T>
		void push_back(const Array::Key key, const Array::ElmType<T>& val) const {
			using namespace Array::Internal;
			std::lock_guard<std::mutex> lock(Data::mtx);

			Data::arrayRef<T>(key).emplace_back(val);
		}

		template<Array::ValueType T>
		void push_back(const Array::Key key, std::vector<Array::ElmType<T>>&& vals) const {
			using namespace Array::Internal;
			std::lock_guard<std::mutex> lock(Data::mtx);

			auto& ref = Data::arrayRef<T>(key);
			ref.insert(ref.end(), std::make_move_iterator(vals.begin()), std::make_move_iterator(vals.end()));
		}

		template<Array::ValueType T>
		void set(const Array::Key key, std::vector<Array::ElmType<T>>&& vals) const {
			using namespace Array::Internal;
			std::lock_guard<std::mutex> lock(Data::mtx);

			auto& ref = Data::arrayRef<T>(key);
			ref = std::move(vals);
		}

		template<Array::ValueType T>
		void clear(const Array::Key key) const {
			using namespace Array::Internal;
			std::lock_guard<std::mutex> lock(Data::mtx);

			Data::arrayRef<T>(key).clear();
		}

		template<Array::ValueType T>
		void erase(const Array::Key key, const std::size_t index) const {
			using namespace Array::Internal;
			std::lock_guard<std::mutex> lock(Data::mtx);

			auto& ref = Data::arrayRef<T>(key);
			ref.erase(ref.begin() + index);
		}

		template<Array::ValueType T>
		void erase(const Array::Key key, const std::size_t index1, const std::size_t index2) const {
			using namespace Array::Internal;
			std::lock_guard<std::mutex> lock(Data::mtx);

			auto& ref = Data::arrayRef<T>(key);
			ref.erase(ref.begin() + index1, ref.begin() + index2);
		}

		template<Array::ValueType T>
		void pop_back(const Array::Key key) const {
			using namespace Array::Internal;
			std::lock_guard<std::mutex> lock(Data::mtx);

			Data::arrayRef<T>(key).pop_back();
		}

		template<Array::ValueType T>
		void pop_front(const Array::Key key) const {
			using namespace Array::Internal;
			std::lock_guard<std::mutex> lock(Data::mtx);

			auto& ref = Data::arrayRef<T>(key);
			ref.erase(ref.begin());
		}

	public:
		template<Array::ValueType T>
		void reusable(const Array::Key key) {
			using namespace Array::Internal;
			using enum Array::ValueType;
			std::lock_guard<std::mutex> lock(Data::mtx);
			garbage.reuse(T, key);
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
		explicit ArrayRead(::FD::Internal::PassKey) noexcept {};
		~ArrayRead() = default;
		FluidumUtils_Class_Delete_CopyMove(ArrayRead);
	public:
		enum class Exception : uint8_t {
			Range
		};

	public:
		template<Array::ValueType T>
		Array::ElmType<T> front(const Array::Key key) const {
			using namespace Array::Internal;
			std::lock_guard<std::mutex> lock(Data::mtx);

			return Data::arrayRef<T>(key).front();
		}

		template<Array::ValueType T>
		Array::ElmType<T> back(const Array::Key key) const {
			using namespace Array::Internal;
			std::lock_guard<std::mutex> lock(Data::mtx);

			return Data::arrayRef<T>(key).back();
		}

		/*
		Exception:
			Range -> (Data::xxx.size() >= index)
		*/
		template<Array::ValueType T>
		Array::ElmType<T> at(const Array::Key key, const std::size_t index) const {
			using namespace Array::Internal;
			std::lock_guard<std::mutex> lock(Data::mtx);

			try {
				return Data::arrayRef<T>(key).at(index);
			}
			catch (const std::out_of_range&) {
				throw Exception::Range;
			}
		}


	public:

		template<Array::ValueType T>
		[[nodiscard]] bool empty(const Array::Key key) const {
			using namespace Array::Internal;
			std::lock_guard<std::mutex> lock(Data::mtx);

			return Data::arrayRef<T>(key).empty();
		}

		template<Array::ValueType T>
		[[nodiscard]] std::size_t size(const Array::Key key) const {
			using namespace Array::Internal;
			std::lock_guard<std::mutex> lock(Data::mtx);

			return 	Data::arrayRef<T>(key).size();
		}

	public:

		template<Array::ValueType T>
		[[nodiscard]] bool eq(const Array::Key key1, const Array::Key key2) const {
			using namespace Array::Internal;
			std::lock_guard<std::mutex> lock(Data::mtx);

			if (key1 == key2)
				return true;

			return Data::arrayRef<T>(key1) == Data::arrayRef<T>(key2);
		}
	};

}