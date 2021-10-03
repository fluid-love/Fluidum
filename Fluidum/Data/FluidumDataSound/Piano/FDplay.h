#pragma once

#include "FDcommand.h"
#include "FDstoppitch.h"
#include "FDdynamics.h"

namespace FD::Piano {

	//CppGA::Genome<>の値からピアノを演奏する．
	//初期値はBPM ♩=120 音符は四分音符．
	//設計としては適当な値を入れられても止めない 内部エラーは例外を出す
	class Play final {
	public:
		//PianoSoundはFluidumDataで固定されるのでポインタは安全(たぶん)
		Play(PianoSound* const piano);
		~Play() = default;

	private://using
		using Key = Internal::Key;
		using FunctionArray = std::array<void(Play::*)(), static_cast<UT>(Key::Count)>;


	private:
		PianoSound* const piano = nullptr;

	private:
		//四分音符一個のミリ秒数
		//初期値は♩=120
		std::size_t milliSecondPerQuarter = static_cast<std::size_t>(60.0 / 120.0 * 1000.0);

		//ここまで待機する
		std::chrono::system_clock::time_point sleepUntil{};

		//次に鳴らす音の時間
		//初期値は四分音符
		std::chrono::system_clock::duration duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::milliseconds(milliSecondPerQuarter));

		Internal::PitchCommand pitchCommand;
		Internal::Dynamics dynamics;
		Internal::TieCommand tie;

	private:
		void call(const std::size_t i);

		//終了処理
		void terminate();

	private:
		template<Key Pitch>
		void pitch() requires Internal::IsKeyRangePitch<Pitch> {
			const auto pitch = Internal::keyToPitch(Pitch);
			//tie
			if (pushTieCommand(pitch))
				return;

			//PushPitchが呼ばれていたらcommandを詰む
			if (pushPitchCommand(pitch))
				return;

			//ここから下は単音(tieと重音以外)のみ
			//適切なタイミングで止める
			this->wait();//待機して

			//鳴らす音は何ミリ秒後に消すかをセット
			stopPitch.add(pitch, sleepUntil);
			piano->sound(dynamics, pitch);
		}

		template<Key Tempo>
		void tempo() requires Internal::IsKeyRangeTempo<Tempo> {
			//Tempo == 60秒間の四分音符数
			//例：60なら四分音符の間は1秒=1000ミリ秒
			constexpr std::size_t milliSeconds = static_cast<std::size_t>((60.0 / static_cast<double>(Internal::Tempos[static_cast<UT>(Tempo) - static_cast<UT>(Key::TempoBegin)])) * 1000.0);
			this->milliSecondPerQuarter = milliSeconds;
		}

		template<Key TimeSignature>
		void setTimeSignature()requires Internal::IsKeyRangeTimeSignature<TimeSignature> {
			//実装
		}

		template<Key Note>
		void setNote() requires Internal::IsKeyRangeNote<Note> {
			if constexpr (Note == Key::Whole)
				duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::milliseconds(milliSecondPerQuarter * 4));
			else if constexpr (Note == Key::Half)
				duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::milliseconds(milliSecondPerQuarter * 2));
			else if constexpr (Note == Key::Quarter)
				duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::milliseconds(milliSecondPerQuarter));
			else if constexpr (Note == Key::Eighth)
				duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::milliseconds(milliSecondPerQuarter / 2));
			else if constexpr (Note == Key::Sixteenth)
				duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::milliseconds(milliSecondPerQuarter / 4));
		}

		template<Key Rest>
		void setRest() requires Internal::IsKeyRangeRest<Rest> {

			//PushPitch,PushTieのときは無視する
			if (pitchCommand || tie)
				return;

			//待機して次の待機時間をRestの分だけ変更する
			if constexpr (Rest == Key::WholeRest)
				this->wait(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::milliseconds(milliSecondPerQuarter * 4)));
			else if constexpr (Rest == Key::HalfRest)
				this->wait(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::milliseconds(milliSecondPerQuarter * 2)));
			else if constexpr (Rest == Key::QuarterRest)
				this->wait(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::milliseconds(milliSecondPerQuarter * 4)));
			else if constexpr (Rest == Key::EighthRest)
				this->wait(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::milliseconds(milliSecondPerQuarter / 2)));
			else if constexpr (Rest == Key::SixteenthRest)
				this->wait(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::milliseconds(milliSecondPerQuarter / 4)));
		}

		template<Key Dynamics>
		void setDynamics() requires Internal::IsKeyRangeDynamics<Dynamics> {
			if constexpr (Dynamics == Key::pppp)
				dynamics = Dynamics::pppp;
			else if constexpr (Dynamics == Key::ppp)
				dynamics = Dynamics::ppp;
			else if constexpr (Dynamics == Key::pp)
				dynamics = Dynamics::pp;
			else if constexpr (Dynamics == Key::p)
				dynamics = Dynamics::p;
			else if constexpr (Dynamics == Key::mp)
				dynamics = Dynamics::mp;
			else if constexpr (Dynamics == Key::n)
				dynamics = Dynamics::n;
			else if constexpr (Dynamics == Key::mf)
				dynamics = Dynamics::mf;
			else if constexpr (Dynamics == Key::f)
				dynamics = Dynamics::f;
			else if constexpr (Dynamics == Key::ff)
				dynamics = Dynamics::ff;
			else if constexpr (Dynamics == Key::fff)
				dynamics = Dynamics::fff;
			else if constexpr (Dynamics == Key::ffff)
				dynamics = Dynamics::ffff;
		}

		void pushPitch() noexcept;
		void popPitch();

		void pushTie() noexcept;
		void popTie();

	public:
		template<typename T>
		void play(const Pair<T>* const bind, const CppGA::Genome<T>* const genome) {

			piano->setAll();

			for (auto&& x : *genome) {
				//定義されていれば呼ぶそうでなければ無視
				if (bind->pairs.contains(x))
					call(Internal::keyToUT(bind->pairs.at(x)));
			}
			this->wait();

			terminate();
		}


	private://補助
		void wait();
		void wait(const std::chrono::system_clock::duration& dur);


		//pushされたreturn true
		_NODISCARD bool pushPitchCommand(const Pitch pitch);

		_NODISCARD bool pushTieCommand(const Pitch pitch);

	private://stop 
		//音を止める用　スレッドの安全は気にしなくてよい
		void stopFunction(const Internal::StopPitchStackType& stack);
		Internal::StopPitch<&Play::stopFunction> stopPitch{ this };


	private:
		//この関数を呼ぶ
		//Bindingで設定した値と対になっている
		static constexpr inline FunctionArray functions = []()->FunctionArray {
			//関数のアドレスを取得
			auto pitchFunctions = []<UT...Index>(std::integer_sequence<UT, Index...>)->std::array<void(Play::*)(), sizeof...(Index)> {
				return { &Play::pitch<static_cast<Key>(Index)>... };
			}(std::make_integer_sequence<UT, static_cast<UT>(Key::PitchCount)>());
			auto tempoFunctions = [&]<UT...Index>(std::integer_sequence<UT, Index...>)->std::array<void(Play::*)(), sizeof...(Index)> {
				return { &Play::tempo<static_cast<Key>(Index)>... };
			}(Fluidum::Utils::Tuple::makeIntegerSequence<UT, static_cast<UT>(Key::TempoBegin), static_cast<UT>(Key::TempoEnd)>());
			auto timeSignatureFunctions = [&]<UT...Index>(std::integer_sequence<UT, Index...>)->std::array<void(Play::*)(), sizeof...(Index)> {
				return { &Play::setTimeSignature<static_cast<Key>(Index)>... };
			}(Fluidum::Utils::Tuple::makeIntegerSequence<UT, static_cast<UT>(Key::TimeSignatureBegin), static_cast<UT>(Key::TimeSignatureEnd)>());
			auto noteFunctions = [&]<UT...Index>(std::integer_sequence<UT, Index...>)->std::array<void(Play::*)(), sizeof...(Index)> {
				return { &Play::setNote<static_cast<Key>(Index)>... };
			}(Fluidum::Utils::Tuple::makeIntegerSequence<UT, static_cast<UT>(Key::NoteBegin), static_cast<UT>(Key::NoteEnd)>());
			auto restFunctions = [&]<UT...Index>(std::integer_sequence<UT, Index...>)->std::array<void(Play::*)(), sizeof...(Index)> {
				return { &Play::setRest<static_cast<Key>(Index)>... };
			}(Fluidum::Utils::Tuple::makeIntegerSequence<UT, static_cast<UT>(Key::RestBegin), static_cast<UT>(Key::RestEnd)>());
			auto dynamicsFunctions = [&]<UT...Index>(std::integer_sequence<UT, Index...>)->std::array<void(Play::*)(), sizeof...(Index)> {
				return { &Play::setDynamics<static_cast<Key>(Index)>... };
			}(Fluidum::Utils::Tuple::makeIntegerSequence<UT, static_cast<UT>(Key::DynamicsBegin), static_cast<UT>(Key::DynamicsEnd)>());


			FunctionArray result{};
			auto itr = std::copy(pitchFunctions.begin(), pitchFunctions.end(), result.begin());
			itr = std::copy(tempoFunctions.begin(), tempoFunctions.end(), itr);
			itr = std::copy(timeSignatureFunctions.begin(), timeSignatureFunctions.end(), itr);
			itr = std::copy(noteFunctions.begin(), noteFunctions.end(), itr);
			itr = std::copy(restFunctions.begin(), restFunctions.end(), itr);
			itr = std::copy(dynamicsFunctions.begin(), dynamicsFunctions.end(), itr);

			*itr++ = &Play::pushPitch;
			*itr++ = &Play::popPitch;
			*itr++ = &Play::pushTie;
			*itr++ = &Play::popTie;

			assert(itr == result.end());
			return result;
		}();
	};

}