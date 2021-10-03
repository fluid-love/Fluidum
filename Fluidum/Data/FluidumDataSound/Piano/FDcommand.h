#pragma once

#include "../common/FDtype.h"
#include "FDpiano.h"

namespace FD::Piano::Internal {
	class PitchCommand final {
	public:

		//volume pitch duration(どれだけ鳴らすか)
		using StackVector = std::vector<std::pair<float, Pitch>>;
	public:

		//flagを返す
		_NODISCARD operator bool() const noexcept;

		//flagをセット
		void operator =(const bool flag) noexcept;

		//最大値を超えている->return false
		bool push(const float volume, const Pitch pitch, const std::chrono::system_clock::duration& duration);


		_NODISCARD std::pair<std::vector<std::pair<float, Pitch>>, std::vector<std::chrono::system_clock::time_point>> reset_get();

		void reset() noexcept;

		_NODISCARD bool isEmpty() const noexcept;
	private:
		bool flag = false;

		StackVector stack = {};
		std::vector<std::chrono::system_clock::duration> duration = {};
	};


	class TieCommand final {

	public:
		//flagを返す
		_NODISCARD operator bool() const noexcept;

		//flagをセット
		void operator =(const bool flag) noexcept;


		void push(const float volume, const std::chrono::system_clock::duration& duration);

		_NODISCARD bool isPitchChanged(const Pitch pitch);

		//時間を取得してリセット
		_NODISCARD std::pair<std::pair<float, Pitch>, std::chrono::system_clock::time_point> reset_get();
		_NODISCARD std::pair<std::pair<float, Pitch>, std::chrono::system_clock::duration> reset_getDuration();

		void reset();

		_NODISCARD bool isEmpty() const noexcept;
	private:
		float volume = 0.0f;
		std::optional<Pitch> pitch = std::nullopt;
		bool flag = false;
		std::chrono::system_clock::duration duration = {};
	};



}
