/*
MIT License

Copyright (c) 2021- fluid-love

https://github.com/fluid-love/ImGuiAnimation

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once
/*
ImGui�̊����̊֐��𗘗p���Ă��邾����,ImGui�����̏��𒼐ڂ����邱�Ƃ͂Ȃ�.

���O��Ԃ�ImAnime��ImAnimeS��ImAnime::Internal.
���O���ImAnime::Internal�͌Ăяo���Ȃ�����.

*/

/*
����
	windows.h���C���N���[�h���Ă���ꍇ��#define max�����̂ŁC
	"numerical_limits::max"�ƏՓ˂���D
*/

/*
Define
ImGuiAnimation_Namespace_ImAnime
ImGuiAnimation_Namespace_ImAnimeS
*/

/*
Functions:

namespace ImAnime
ver1.0.0
	ImAnime::PushStyleColor
	ImAnime::PushStyleVar
	ImAnime::SetNextWindowBgAlpha
	ImAnime::SetNextWindowSize
	ImAnime::SetNextItemWidth
	ImAnime::Dummy
	ImAnimeS::Dummy

*/


#include <iostream>
#include <chrono>
#include <array>
#include <type_traits>
#include <limits>

#include <imgui.h>

#ifndef IMANIME_API
#define IMANIME_API
#endif

using ImAnimeTime = std::chrono::steady_clock::time_point;
using ImAnimeSecond = float;

//�O���錾
namespace ImAnime::Internal {
	class CounterManager;
}

//�񕉐����Ǝ��ԂɑΉ�
template<typename T>
concept ImAnimeCounterType = std::unsigned_integral<T> || std::same_as<T, ImAnimeTime>;

//T��ImAnimeSecond�Ȃ�float(����(s))���@�����łȂ����T�^(�񕉐���)
template<typename T>
using ImAnimeCountType = std::conditional_t<std::is_same_v<T, ImAnimeTime>, ImAnimeSecond, T>;

//ImAnime::Internal���̊֐����͗��p���Ȃ�����.
namespace ImAnime::Internal {

	//enum class(flags)�̊��	
	using FlagUnderlyingType = uint16_t;

	//time point
	using TimePoint = ImAnimeTime;
}

enum class ImCounterCond : ImAnime::Internal::FlagUnderlyingType {
	NONE = 0x0000,    //���̃A�j���[�V�������[�v�ŏI��� �I���������J�E���g�͐i�ݑ����� 
	REVERSE = 0x0001, //����:�J�E���g�͋t�ɂȂ�Ȃ�
	PAUSE = 0x0002,   //�ꎞ��~ �J�E���g�͒�~����
	REPEAT = 0x0004,  //�J��Ԃ� �J�E���g�̓��Z�b�g����閈��0�ɖ߂�
};

//���O���ImAnime�Œ�`�����֐���pop����K�v��������̂͂����Ԃ�.
struct ImAnimePop {
	uint16_t colorCount = 0;
	uint16_t varCount = 0;
};

enum class ImAnimeType : ImAnime::Internal::FlagUnderlyingType {
	LINEAR,//�����ɖړI�̒l�܂ŕω�������.
	SQUARE //y=x*x�ŕω�������.
};


namespace ImAnime::Internal {
	//���^
	template<typename T>
	using UnderlyingType = std::underlying_type_t<T>;

	//operator���`����enum���w��
	template<typename T>
	concept EnumType =
		std::is_same_v<UnderlyingType<T>, ImAnime::Internal::FlagUnderlyingType> &&//��ꂪFlagBaseType�ł��邱��.
		std::is_same_v<T, ImCounterCond>;//�w�肳�ꂽenum class�ȊO��NG.

	//operators
	template<EnumType EnumClass>
	constexpr EnumClass operator|(const EnumClass& left, const EnumClass& right) {
		return static_cast<EnumClass>(static_cast<UnderlyingType<EnumClass>>(left) | static_cast<UnderlyingType<EnumClass>>(right));
	}

	template<EnumType EnumClass>
	constexpr EnumClass operator^(const EnumClass& left, const EnumClass& right) {
		return static_cast<EnumClass>(static_cast<UnderlyingType<EnumClass>>(left) ^ static_cast<UnderlyingType<EnumClass>>(right));
	}

	template<EnumType EnumClass>
	constexpr EnumClass operator&(const EnumClass& left, const EnumClass& right) {
		return static_cast<EnumClass>(static_cast<UnderlyingType<EnumClass>>(left) & static_cast<UnderlyingType<EnumClass>>(right));
	}

	template<EnumType EnumClass>
	constexpr EnumClass& operator|=(EnumClass& left, const EnumClass& right) {
		return left = left | right;
	}

	template<EnumType EnumClass>
	constexpr EnumClass& operator^=(EnumClass& left, const EnumClass& right) {
		return left = left ^ right;
	}

	template<EnumType EnumClass>
	constexpr EnumClass& operator&=(EnumClass& left, const EnumClass& right) {
		return left = left & right;
	}

	template<EnumType EnumClass>
	constexpr bool operator==(const EnumClass& left, const EnumClass& right) {
		return (static_cast<UnderlyingType<EnumClass>>(left) == static_cast<UnderlyingType<EnumClass>>(right));
	}
}

//namaespace 
using ImAnime::Internal::operator|;
using ImAnime::Internal::operator^;
using ImAnime::Internal::operator&;
using ImAnime::Internal::operator|=;
using ImAnime::Internal::operator^=;
using ImAnime::Internal::operator&=;
using ImAnime::Internal::operator==;



//counter
/*
���[�U�͂����ێ�����K�v������.
count�͂��̌^�ŕ\���ł���ő�l��菬�����ꍇ�̓J�E���g���ꑱ����.//��
��{�I�ɓ����̃J�E���g�͒��ڂ�����Ȃ��悤�ɂ��Ă���D

�^�����͎���(s)���f�t�H���g�Ŏw��
*/
template<ImAnimeCounterType Type = ImAnimeTime>
class ImCounter final {
public:
	//constructor
	ImCounter() = default;
	explicit ImCounter(const ImCounterCond condition) : condition(condition) {}//set cond

	//destructor
	~ImCounter() = default;


public:
	//�J�E���g���擾 ���ԂŊǗ�->�b(s)
	ImAnimeCountType<Type> getCurrentCount() {
		ImAnimeSecond time = 0.0f;

		using Duration = std::chrono::duration<ImAnimeSecond, std::chrono::seconds::period>;

		//pause��
		if (isUp(ImCounterCond::PAUSE)) {
			if (isPause) {
				time = Duration(count - ImAnime::Internal::TimePoint()).count();
			}
			else {
				ImAnimeTime currentTime = std::chrono::high_resolution_clock::now();
				time = Duration(currentTime - count).count();
			}
		}
		else if (count == ImAnime::Internal::TimePoint()) {
			;
		}
		else if (isPause) {
			time = Duration(count - ImAnime::Internal::TimePoint()).count();
		}
		else {
			ImAnimeTime currentTime = std::chrono::high_resolution_clock::now();
			time = Duration(currentTime - count).count();
		}


		return time;
	}

	//reset count��������.
	void reset() {
		count = ImAnime::Internal::TimePoint();
	}

	//condition���Z�b�g.
	void changeCondition(const ImCounterCond condition) {
		this->condition = condition;
	}

	//�グ��
	void up(const ImCounterCond condition) {
		this->condition |= condition;
	}

	//������
	void down(const ImCounterCond condition) {
		constexpr ImCounterCond all = static_cast<ImCounterCond>(std::numeric_limits<std::underlying_type_t<ImCounterCond>>::max());
		this->condition &= all ^ condition;
	}

	//condition���擾.
	ImCounterCond getCondition()const {
		return this->condition;
	}

	//�����l���ǂ���
	//ImCounterCond::Repeat�̏ꍇ��1���[�v���I��閈�ɏ���������邱�Ƃɒ���.
	bool isInitialValue() {
		return (count == std::chrono::steady_clock::time_point());
	}

	//�����Ă��邩 �����̏ꍇ�͑S�ė����Ă��邩
	bool isUp(const ImCounterCond condition) const {
		return ((this->condition & condition) == condition);
	}

	//�~��Ă��邩 �����̏ꍇ�͑S�č~��Ă��邩
	bool isDown(const ImCounterCond condition) const {
		return ((this->condition & condition) == ImCounterCond::NONE);
	}


public://operators
	void operator=(const ImCounterCond condition) {
		this->condition = condition;
	}

	void operator|=(const ImCounterCond condition) {
		this->condition |= condition;
	}

	void operator^=(const ImCounterCond condition) {
		this->condition ^= condition;
	}

	void operator&=(const ImCounterCond condition) {
		this->condition &= condition;
	}



public:
	//count�����[�U��������Ȃ��悤�ɂ���.
	friend class ImAnime::Internal::CounterManager;

private:
	//�{��
	Type count = {};
	ImCounterCond condition = ImCounterCond::NONE;
	bool isPause = false;
};

template<std::unsigned_integral Type>
class ImCounter<Type> final {
public:
	//constructor
	ImCounter() = default;
	explicit ImCounter(const ImCounterCond condition) : condition(condition) {}//set cond

	//destructor
	~ImCounter() = default;

public:
	//���݂̃J�E���g���擾
	Type getCurrentCount() {
		return count;
	}

	//reset count��0�ɂ���.
	void reset() {
		count = 0;
	}

	//condition���Z�b�g.
	void changeCondition(const ImCounterCond condition) {
		this->condition = condition;
	}

	//�グ��
	void up(const ImCounterCond condition) {
		this->condition |= condition;
	}

	//������
	void down(const ImCounterCond condition) {
		constexpr ImCounterCond all = static_cast<ImCounterCond>(std::numeric_limits<std::underlying_type_t<ImCounterCond>>::max());
		this->condition &= all ^ condition;
	}

	//condition���擾.
	ImCounterCond getCondition()const {
		return this->condition;
	}

	//�����l���ǂ��� �܂�count��0��
	bool isInitialValue() {
		return (count == 0);
	}

	//�����Ă��邩 �����̏ꍇ�͑S�ė����Ă��邩
	bool isUp(const ImCounterCond condition) const {
		return ((this->condition & condition) == condition);
	}

	//�~��Ă��邩 �����̏ꍇ�͑S�č~��Ă��邩
	bool isDown(const ImCounterCond condition) const {
		return ((this->condition & condition) == ImCounterCond::NONE);
	}


public://operators
	void operator=(const ImCounterCond condition) {
		this->condition = condition;
	}

	void operator|=(const ImCounterCond condition) {
		this->condition |= condition;
	}

	void operator^=(const ImCounterCond condition) {
		this->condition ^= condition;
	}

	void operator&=(const ImCounterCond condition) {
		this->condition &= condition;
	}



public:
	//count�����[�U��������Ȃ��悤�ɂ���.
	friend class ImAnime::Internal::CounterManager;

private:
	//�{��
	Type count = 0;
	ImCounterCond condition = ImCounterCond::NONE;
};



//ImCounter�𑀍삷��.
namespace ImAnime::Internal {

	//�J�E���^�[�ɂ͂�������A�N�Z�X����.
	class CounterManager {
	public:
		template<ImAnimeCounterType T>
		static auto get(const ImCounter<T>& counter) { return counter.count; }

		template<ImAnimeCounterType T>
		static void add(ImCounter<T>& counter) {
			static_assert(!std::is_same_v<T, ImAnimeSecond>, "ERROR: add");
			constexpr T max = std::numeric_limits<T>::max();
			if (counter.count < max)
				counter.count++;
		}

		template<ImAnimeCounterType T>
		static void sub(ImCounter<T>& counter) {
			static_assert(!std::is_same_v<T, ImAnimeSecond>, "ERROR: sub");
			constexpr T min = std::numeric_limits<T>::min();
			if (counter.count > min)
				counter.count--;
		}

		template<ImAnimeCounterType T>
		static void set(ImCounter<T>& counter, T val) { counter.count = val; }

		template<ImAnimeCounterType T>
		static bool getPause(ImCounter<T>& counter) { return counter.isPause; }

		template<ImAnimeCounterType T>
		static void setPause(ImCounter<T>& counter, const bool val) { counter.isPause = val; }

	};

}


//��{�I�ȉ��Z�Ȃ�
namespace ImAnime::Internal {

	//��������
	template<typename T>
	concept FloatingPoint = std::floating_point<T>;

	//����
	template<typename T>
	concept Integral = std::integral<T>;

	//����or��������
	template<typename T>
	concept IntOrFloat = Integral<T> || FloatingPoint<T>;

	//���Z���\��
	template<typename T, typename U>
	concept IsAddAble = requires (T left, U right) {
		{left + right}->std::common_with<T>;
	};

	//���Z���\��
	template<typename T, typename U>
	concept IsSubAble = requires (T left, U right) {
		{left - right}->std::common_with<T>;
	};

	//�|�Z���\��
	template<typename T, typename U>
	concept IsMultiAble = requires (T left, U right) {
		{left* right}->std::common_with<T>;
	};

	//���Z���\��
	template<typename T, typename U>
	concept IsDivAble = requires (T left, U right) {
		{left / right}->std::common_with<T>;
	};

	//�l�����Z���\
	template<typename T>
	concept ValueType = IsAddAble<T, T> && IsSubAble<T, T> && IsMultiAble<T, T> && IsDivAble<T, T>;



}

//ImGui�̊֐��Ăяo��
namespace ImAnime::Internal {

	//�Ăяo��ImGui�̊֐�
	enum class ImGuiFunctionType : FlagUnderlyingType {
		NONE,
		DUMMY,
		PUSH_STYLE_COLOR,
		PUSH_STYLE_VAR,
		SET_NEXT_WINDOW_BG_ALPHA,
		SET_NEXT_WINDOW_SIZE,
		SET_NEXT_ITEM_WIDTH,
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	namespace Internal {
		template<uint16_t Size, typename... T, typename U>
		void pushStyleColor(const std::tuple<T...>& indices, const U& value) {
			if constexpr (Size == 0)
				return;
			else {
				ImGui::PushStyleColor(std::get<Size - 1>(indices), value);
				pushStyleColor<Size - 1>(indices, value);
			}
		}
	}

	template<typename... T, typename U>
	void pushStyleColor(const std::tuple<T...>& indices, const U& value) {
		constexpr uint16_t index = static_cast<uint16_t>(sizeof...(T)) - 1;
		ImGui::PushStyleColor(std::get<index>(indices), value);
		Internal::pushStyleColor<index>(indices, value);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	namespace Internal {
		template<uint16_t Size, typename... T, typename U>
		void pushStyleVar(const std::tuple<T...>& indices, const U& value) {
			if constexpr (Size == 0)
				return;
			else {
				ImGui::PushStyleVar(std::get<Size - 1>(indices), value);
				pushStyleVar<Size - 1>(indices, value);
			}
		}
	}

	template<typename... T, typename U>
	void pushStyleVar(const std::tuple<T...>& indices, const U& value) {
		constexpr uint16_t index = static_cast<uint16_t>(sizeof...(T)) - 1;
		ImGui::PushStyleVar(std::get<index>(indices), value);
		Internal::pushStyleVar<index>(indices, value);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

//operator
namespace ImAnime::Internal {

	inline ImVec2 operator+(const ImVec2& l, const ImVec2& r) {
		return ImVec2(l.x + r.x, l.y + r.y);
	}
	inline ImVec2 operator-(const ImVec2& l, const ImVec2& r) {
		return ImVec2(l.x - r.x, l.y - r.y);
	}
	inline ImVec2 operator*(const ImVec2& l, const ImVec2& r) {
		return ImVec2(l.x * r.x, l.y * r.y);
	}
	inline ImVec2 operator/(const ImVec2& l, const ImVec2& r) {
		return ImVec2(l.x / r.x, l.y / r.y);
	}

	inline ImVec2 operator+(const ImVec2& l, const float r) {
		return ImVec2(l.x + r, l.y + r);
	}
	inline ImVec2 operator-(const ImVec2& l, const float r) {
		return ImVec2(l.x - r, l.y - r);
	}
	inline ImVec2 operator*(const ImVec2& l, const float r) {
		return ImVec2(l.x * r, l.y * r);
	}
	inline ImVec2 operator/(const ImVec2& l, const float r) {
		return ImVec2(l.x / r, l.y / r);
	}


	inline ImVec4 operator+(const ImVec4& l, const ImVec4& r) {
		return ImVec4(l.x + r.x, l.y + r.y, l.z + r.z, l.w + r.w);
	}
	inline ImVec4 operator-(const ImVec4& l, const ImVec4& r) {
		return ImVec4(l.x - r.x, l.y - r.y, l.z - r.z, l.w - r.w);
	}
	inline ImVec4 operator*(const ImVec4& l, const ImVec4& r) {
		return ImVec4(l.x * r.x, l.y * r.y, l.z * r.z, l.w * r.w);
	}
	inline ImVec4 operator/(const ImVec4& l, const ImVec4& r) {
		return ImVec4(l.x / r.x, l.y / r.y, l.z / r.z, l.w / r.w);
	}

	inline ImVec4 operator+(const ImVec4& l, const float r) {
		return ImVec4(l.x + r, l.y + r, l.z + r, l.w + r);
	}
	inline ImVec4 operator-(const ImVec4& l, const float r) {
		return ImVec4(l.x - r, l.y - r, l.z - r, l.w - r);
	}
	inline ImVec4 operator*(const ImVec4& l, const float r) {
		return ImVec4(l.x * r, l.y * r, l.z * r, l.w * r);
	}
	inline ImVec4 operator/(const ImVec4& l, const float r) {
		return ImVec4(l.x / r, l.y / r, l.z / r, l.w / r);
	}

}


//dynamic
namespace ImAnime::Internal {

	template<ImGuiFunctionType FuncType, ValueType T, ImAnimeCounterType Counter, typename ...Params>
	class Anime final {
	private:
		const T& begin;
		const T& end;
		const T diff = end - begin;
		ImCounter<Counter>& counter;
		const ImAnimeCountType<Counter> count;
		ImAnimeCountType<Counter> oldCount;
		const ImAnimeType animeType = ImAnimeType::LINEAR;
		const std::tuple<Params...> params;

	public:


		Anime(
			ImCounter<Counter>& counter,
			const ImAnimeCountType<Counter> count,
			const T& begin,
			const T& end,
			const ImAnimeType animeType = ImAnimeType::LINEAR,
			const Params... params
		) :
			begin(begin),
			end(end),
			counter(counter),
			count(count),
			animeType(animeType),
			params(std::make_tuple(params...))
		{
			this->call();
		}

		~Anime() = default;

	private:

		//�񐔂ŃJ�E���g����ꍇ
		template<typename U = Counter>
		std::enable_if_t<!std::is_same_v<U, ImAnimeTime>> call() {
			const auto counterCond = counter.getCondition();//��Ԃ��擾
			this->oldCount = CounterManager::get(counter);

			if (oldCount >= count) {//�I�����Ă���

				//Repeat��ԂȂ�J�E���g�����Z�b�g����
				if (counter.isUp(ImCounterCond::REPEAT))
					counter.reset();

				if (counter.isUp(ImCounterCond::REVERSE))
					callImGuiFunction(begin);
				else
					callImGuiFunction(end);

			}
			else {//�͂���&�ω���

				if (counter.isUp(ImCounterCond::REVERSE))
					oldCount = count - oldCount;


				auto result = this->calc();

				callImGuiFunction(result);

			}

			//pause��ԂłȂ��̂ł���ΐi�߂�
			if (counter.isDown(ImCounterCond::PAUSE)) {
				ImAnime::Internal::CounterManager::add(counter);//�J�E���g��i�߂�
			}
		}

		//���ԂŃJ�E���g����ꍇ
		template<typename U = Counter>
		std::enable_if_t<std::is_same_v<U, ImAnimeTime>> call() {
			const auto counterCond = counter.getCondition();

			ImAnimeTime currentCount = std::chrono::high_resolution_clock::now();
			const ImAnimeTime oldTime = CounterManager::get(counter);
			this->oldCount = std::chrono::duration<ImAnimeSecond, std::chrono::seconds::period>(currentCount - oldTime).count();

			//pause
			auto elapsed = [](const TimePoint& left, const TimePoint& right)->std::chrono::milliseconds const {
				return std::chrono::duration_cast<std::chrono::milliseconds>(left - right);
			};
			auto getElapsedTime = [](const TimePoint& left, const TimePoint& right)->ImAnimeSecond {
				return std::chrono::duration<ImAnimeSecond, std::chrono::seconds::period>(left - right).count();
			};

			if (counter.isUp(ImCounterCond::PAUSE)) {
				//����̂�
				if (!CounterManager::getPause(counter)) {
					//�o�ߎ��Ԃ�count��set
					CounterManager::set(counter, TimePoint() + std::chrono::milliseconds(elapsed(currentCount, oldTime)));
					CounterManager::setPause(counter, true);
					oldCount = getElapsedTime(currentCount, oldTime);
				}
				else {
					oldCount = getElapsedTime(oldTime, TimePoint());
				}
			}
			else {
				//pause���~�낳�ꂽ����̏����i��񂾂��j
				if (CounterManager::getPause(counter)) {
					oldCount = getElapsedTime(oldTime, TimePoint());
					CounterManager::set(counter, currentCount - std::chrono::milliseconds(elapsed(oldTime, TimePoint())));
					CounterManager::setPause(counter, false);//�~�낷
				}
			}

			//����
			if (oldTime == TimePoint()) {
				if (counter.isDown(ImCounterCond::PAUSE)) {
					CounterManager::set(counter, currentCount);
				}

				callImGuiFunction(begin);
			}
			else if (oldCount > count) {//�I��
				if (counter.isUp(ImCounterCond::REPEAT)) {
					counter.reset();
				}


				if (counter.isUp(ImCounterCond::REVERSE))
					callImGuiFunction(begin);
				else
					callImGuiFunction(end);

			}
			else { //�ω���

				if (counter.isUp(ImCounterCond::REVERSE))
					oldCount = count - oldCount;

				auto result = this->calc();
				callImGuiFunction(result);
			}

		}

	private://�v�Z

		auto calc() {
			if (this->animeType == ImAnimeType::LINEAR)
				return begin + this->linear();
			else if (this->animeType == ImAnimeType::SQUARE)
				return begin + this->square();

			assert(true);
		}

		//���`
		T linear() {
			T result = (this->diff / this->count) * oldCount;
			return result;
		}

		//���� y=x*x
		T square() {
			auto per = this->oldCount / this->count;
			auto mul = per * per;
			T result = this->diff * mul;
			return result;
		}

	private://ImGui Function

		void callImGuiFunction(const T& val) {
			using Type = ImGuiFunctionType;
			if constexpr (FuncType == Type::DUMMY)
				ImGui::Dummy(val);
			else if constexpr (FuncType == Type::PUSH_STYLE_COLOR) {
				static_assert(sizeof...(Params) > 0);
				pushStyleColor(this->params, val);
			}
			else if constexpr (FuncType == Type::PUSH_STYLE_VAR) {
				static_assert(sizeof...(Params) > 0);
				pushStyleVar(this->params, val);
			}
			else if constexpr (FuncType == Type::SET_NEXT_WINDOW_BG_ALPHA)
				ImGui::SetNextWindowBgAlpha(val);
			else if constexpr (FuncType == Type::SET_NEXT_WINDOW_SIZE)
				ImGui::SetNextWindowSize(val);
			else if constexpr (FuncType == Type::SET_NEXT_ITEM_WIDTH)
				ImGui::SetNextItemWidth(val);

		}

	};
}


//static

struct ImAnimeVec2 final {
	float x = 0.0f;
	float y = 0.0f;

	constexpr ImAnimeVec2() = default;
	explicit constexpr ImAnimeVec2(const float x, const float y) noexcept :x(x), y(y) {}

	ImAnimeVec2 operator+(const ImAnimeVec2& r) const {
		return ImAnimeVec2(x + r.x, y + r.y);
	}

	ImAnimeVec2 operator-(const ImAnimeVec2& r) const {
		return ImAnimeVec2(x - r.x, y - r.y);
	}

	ImAnimeVec2 operator*(const ImAnimeVec2& r) const {
		return ImAnimeVec2(x * r.x, y * r.y);
	}

	ImAnimeVec2 operator/(const ImAnimeVec2& r) const {
		return ImAnimeVec2(x / r.x, y / r.y);
	}

	ImAnimeVec2 operator+(const float r) const {
		return ImAnimeVec2(x + r, y + r);
	}

	ImAnimeVec2 operator-(const float r) const {
		return ImAnimeVec2(x - r, y - r);
	}

	ImAnimeVec2 operator*(const float r) const {
		return ImAnimeVec2(x * r, y * r);
	}

	ImAnimeVec2 operator/(const float r) const {
		return ImAnimeVec2(x / r, y / r);
	}


	operator ImVec2()const noexcept {
		return ImVec2(x, y);
	}

};

namespace ImAnime::Internal {


}


namespace ImAnime::Internal {

	template<
		ImGuiFunctionType FuncType,
		ImAnimeCounterType Counter,
		ImAnimeCountType<Counter> Count,
		typename T,
		T Begin,
		T End,
		ImAnimeType AnimeType = ImAnimeType::LINEAR,
		typename... Params
	>
		class AnimeS final {
		private:
			const T diff = End - Begin;
			ImCounter<Counter>& counter;
			ImAnimeCountType<Counter> oldCount;
			const std::tuple<Params...> params;

		public:
			AnimeS(ImCounter<Counter>& counter) :counter(counter) {
				this->call();
			}

			~AnimeS() = default;

		private:

			//�񐔂ŃJ�E���g����ꍇ
			template<typename U = Counter>
			std::enable_if_t<!std::is_same_v<U, ImAnimeTime>> call() {
				const auto counterCond = counter.getCondition();//��Ԃ��擾
				this->oldCount = CounterManager::get(counter);

				if (oldCount >= Count) {//�I�����Ă���

					//Repeat��ԂȂ�J�E���g�����Z�b�g����
					if (counter.isUp(ImCounterCond::REPEAT))
						counter.reset();

					if (counter.isUp(ImCounterCond::REVERSE))
						callImGuiFunction(Begin);
					else
						callImGuiFunction(End);

				}
				else {//�͂���&�ω���

					if (counter.isUp(ImCounterCond::REVERSE))
						oldCount = Count - oldCount;


					auto result = this->calc();

					callImGuiFunction(result);

				}

				//pause��ԂłȂ��̂ł���ΐi�߂�
				if (counter.isDown(ImCounterCond::PAUSE)) {
					ImAnime::Internal::CounterManager::add(counter);//�J�E���g��i�߂�
				}
			}

			//���ԂŃJ�E���g����ꍇ
			template<typename U = Counter>
			std::enable_if_t<std::is_same_v<U, ImAnimeTime>> call() {
				const auto counterCond = counter.getCondition();

				ImAnimeTime currentCount = std::chrono::high_resolution_clock::now();
				const ImAnimeTime oldTime = CounterManager::get(counter);
				this->oldCount = std::chrono::duration<ImAnimeSecond, std::chrono::seconds::period>(currentCount - oldTime).count();

				//pause
				auto elapsed = [](const TimePoint& left, const TimePoint& right)->std::chrono::milliseconds const {
					return std::chrono::duration_cast<std::chrono::milliseconds>(left - right);
				};
				auto getElapsedTime = [](const TimePoint& left, const TimePoint& right)->ImAnimeSecond {
					return std::chrono::duration<ImAnimeSecond, std::chrono::seconds::period>(left - right).count();
				};

				if (counter.isUp(ImCounterCond::PAUSE)) {
					//����̂�
					if (!CounterManager::getPause(counter)) {
						//�o�ߎ��Ԃ�count��set
						CounterManager::set(counter, TimePoint() + std::chrono::milliseconds(elapsed(currentCount, oldTime)));
						CounterManager::setPause(counter, true);
						oldCount = getElapsedTime(currentCount, oldTime);
					}
					else {
						oldCount = getElapsedTime(oldTime, TimePoint());
					}
				}
				else {
					//pause���~�낳�ꂽ����̏����i��񂾂��j
					if (CounterManager::getPause(counter)) {
						oldCount = getElapsedTime(oldTime, TimePoint());
						CounterManager::set(counter, currentCount - std::chrono::milliseconds(elapsed(oldTime, TimePoint())));
						CounterManager::setPause(counter, false);//�~�낷
					}
				}

				//����
				if (oldTime == TimePoint()) {
					if (counter.isDown(ImCounterCond::PAUSE)) {
						CounterManager::set(counter, currentCount);
					}

					callImGuiFunction(Begin);
				}
				else if (oldCount > Count) {//�I��
					if (counter.isUp(ImCounterCond::REPEAT)) {
						counter.reset();
					}


					if (counter.isUp(ImCounterCond::REVERSE))
						callImGuiFunction(Begin);
					else
						callImGuiFunction(End);

				}
				else { //�ω���

					if (counter.isUp(ImCounterCond::REVERSE))
						oldCount = Count - oldCount;

					auto result = this->calc();
					callImGuiFunction(result);
				}

			}

		private://�v�Z

			auto calc() {
				if constexpr (AnimeType == ImAnimeType::LINEAR)
					return Begin + this->linear();
				else if constexpr (AnimeType == ImAnimeType::SQUARE)
					return Begin + this->square();

				assert(true);
			}

			//���`
			T linear() {
				T result = (this->diff / Count) * oldCount;
				return result;
			}

			//���� y=x*x
			T square() {
				auto per = this->oldCount / Count;
				auto mul = per * per;
				T result = this->diff * mul;
				return result;
			}

		private://ImGui Function

			void callImGuiFunction(const T& val) {
				using Type = ImGuiFunctionType;
				if constexpr (FuncType == Type::DUMMY)
					ImGui::Dummy(val);
				else if constexpr (FuncType == Type::PUSH_STYLE_COLOR) {
					static_assert(sizeof...(Params) > 0);
					pushStyleColor(this->params, val);
				}
				else if constexpr (FuncType == Type::PUSH_STYLE_VAR) {
					static_assert(sizeof...(Params) > 0);
					pushStyleVar(this->params, val);
				}
				else if constexpr (FuncType == Type::SET_NEXT_WINDOW_BG_ALPHA)
					ImGui::SetNextWindowBgAlpha(val);
				else if constexpr (FuncType == Type::SET_NEXT_WINDOW_SIZE)
					ImGui::SetNextWindowSize(val);
				else if constexpr (FuncType == Type::SET_NEXT_ITEM_WIDTH)
					ImGui::SetNextItemWidth(val);

			}


	};


}


namespace ImAnime {

	/*
	Args:
		1. ImCounter<Counter>          //�쐬����ImCounter
		2. ImAnimeCountType<Counter>   //ImCounter<ImAnimeTime>�Ȃ�A�j���[�V�����I���܂ł̕b��(float)���w��C����ȊO�Ȃ�I���܂ł̉񐔂��w��
		3. T                           //�J�n���̒l �^�͂��ꂼ��ɑΉ�������(��FDummy -> ImVec2)
		4. T                           //�I�����̒l �^�͂��ꂼ��ɑΉ�������(��FDummy -> ImVec2)
		5. ImAnimeType                 //�ǂ̂悤�ɃA�j���[�V����������̂����w�� �f�t�H���g�Ő��`==ImAnimeType::LINEAR

	Exception:
		�Ăяo��ImGui�̊֐��ɉ�����
	*/


	template<ImAnimeCounterType Counter>
	using PushStyleColor = ImAnime::Internal::Anime<ImAnime::Internal::ImGuiFunctionType::PUSH_STYLE_COLOR, ImVec4, Counter, ImGuiCol>;

	template<typename T, ImAnimeCounterType Counter>
	using PushStyleVar = ImAnime::Internal::Anime<ImAnime::Internal::ImGuiFunctionType::PUSH_STYLE_VAR, T, Counter, ImGuiStyleVar>;

	template<ImAnimeCounterType Counter>
	using Dummy = ImAnime::Internal::Anime<ImAnime::Internal::ImGuiFunctionType::DUMMY, ImVec2, Counter>;

	template<ImAnimeCounterType Counter>
	using SetNextWindowBgAlpha = ImAnime::Internal::Anime<ImAnime::Internal::ImGuiFunctionType::SET_NEXT_WINDOW_BG_ALPHA, float, Counter>;

	template<ImAnimeCounterType Counter>
	using SetNextWindowSize = ImAnime::Internal::Anime<ImAnime::Internal::ImGuiFunctionType::SET_NEXT_WINDOW_SIZE, float, Counter>;

	template<ImAnimeCounterType Counter>
	using SetNextItemSize = ImAnime::Internal::Anime<ImAnime::Internal::ImGuiFunctionType::SET_NEXT_ITEM_WIDTH, float, Counter>;

	inline void PopStyleColor(int count = 1) {
		ImGui::PopStyleColor(count);
	}

	inline void PopStyleVar(int count = 1) {
		ImGui::PopStyleVar(count);
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




namespace ImAnimeS {


	//template<ImAnimeVec2 Begin, ImAnimeVec2 End, ImAnimeCounterType Counter, ImAnimeCountType<Counter> Count, ImAnimeType AnimeType = ImAnimeType::LINEAR>
	//using Dummy = ImAnime::Internal::SCommon<ImAnime::Internal::ImGuiFunctionType::DUMMY, Counter, Count, ImAnimeVec2, Begin, End, AnimeType>;

	namespace Internal {
		template<typename T>
		using CountTypeToCounterType = std::conditional_t<std::is_same_v<T, ImAnimeSecond>, ImAnimeTime, T>;
	}


	template<ImAnimeVec2 Begin, ImAnimeVec2 End, auto Count, ImAnimeType AnimeType = ImAnimeType::LINEAR>
	using Dummy = ImAnime::Internal::AnimeS<ImAnime::Internal::ImGuiFunctionType::DUMMY, Internal::CountTypeToCounterType<decltype(Count)>, Count, ImAnimeVec2, Begin, End, AnimeType>;



}


//ImAnimeDmeo.cpp
namespace ImAnime {
	void ShowDemoWindow();
}