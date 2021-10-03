#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <variant>
#include <chrono>
#include <algorithm>
#include <filesystem>
#include <thread>
#include <functional>

#include <FluidumSound.h>

#include <FluidumFile/FluidumFile.h>

#include "../../FluidumDataGenome/FDgenome.h"
#include "../../common/FDcommon.h"
#include "FDexception.h"


namespace FD::Piano {

	//underlying type
	using UT = uint16_t;

	namespace Internal {
		//変えない　enum class Pitchと連動
		constexpr inline const char* PitchKeys[] = {
			"A0","A0S","A1","A1S","A2","A2S","A3","A3S","A4","A4S","A5","A5S","A6","A6S","A7","A7S",
			"B0","B1","B2","B3","B4","B5","B6","B7",
			"C1","C1S","C2","C2S","C3","C3S","C4","C4S","C5","C5S","C6","C6S","C7","C7S","C8",
			"D1","D1S","D2","D2S","D3","D3S","D4","D4S","D5","D5S","D6","D6S","D7","D7S",
			"E1","E2","E3","E4","E5","E6","E7",
			"F1","F1S","F2","F2S","F3","F3S","F4","F4S","F5","F5S","F6","F6S","F7","F7S",
			"G1","G1S","G2","G2S","G3","G3S","G4","G4S","G5","G5S","G6","G6S","G7","G7S",
		};
	}
	enum class Pitch : UT {
		A0, A0S, A1, A1S, A2, A2S, A3, A3S, A4, A4S, A5, A5S, A6, A6S, A7, A7S,
		B0, B1, B2, B3, B4, B5, B6, B7,
		C1, C1S, C2, C2S, C3, C3S, C4, C4S, C5, C5S, C6, C6S, C7, C7S, C8,
		D1, D1S, D2, D2S, D3, D3S, D4, D4S, D5, D5S, D6, D6S, D7, D7S,
		E1, E2, E3, E4, E5, E6, E7,
		F1, F1S, F2, F2S, F3, F3S, F4, F4S, F5, F5S, F6, F6S, F7, F7S,
		G1, G1S, G2, G2S, G3, G3S, G4, G4S, G5, G5S, G6, G6S, G7, G7S,
	};
	namespace Internal {
		inline constexpr Pitch PitchEnd = Pitch::G7S;
	}


	//連動
	enum class Tempo : UT {
		Largo,//45,
		Adagio,//50,
		Lento,//55,
		Andante,//70,
		Andantino,//80,
		Moderate,//90,
		AllegroModerate,//100,
		Allegretto,//110,
		Allegro,//130,
		Vivace,//150,
		Presto,//180,
		Prestissimo,//200
	};
	namespace Internal {
		constexpr inline UT Tempos[] = {
			45,	50,	55,	70,	80,	90, 100,110,130,150,180,200
		};

		inline constexpr Tempo TempoEnd = Tempo::Prestissimo;
	}


	enum class TimeSignature : UT {
		FourFour, // 4/4
		ThreeFour // 3/4
	};
	namespace Internal {
		inline constexpr TimeSignature TimeSignatureEnd = TimeSignature::ThreeFour;
	}


	enum class Note : UT {
		Whole,    //全音符
		Quarter,  //四分音符
		Half,     //二分音符
		Eighth,   //八分音符
		Sixteenth //十六分音符
	};
	namespace Internal {
		inline constexpr Note NoteEnd = Note::Sixteenth;
	}


	enum class Rest : UT {
		Whole,    //全休符
		Quarter,  //四分休符
		Half,     //二分休符
		Eighth,   //八分休符
		Sixteenth //十六分休符
	};
	namespace Internal {
		inline constexpr Rest RestEnd = Rest::Sixteenth;
	}


	enum class Dynamics : UT {
		pppp,
		ppp,
		pp,
		p,
		mp,
		n,	//普通　これより↑にいけば強く　↓は弱く
		mf,
		f,
		ff,
		fff,
		ffff
	};
	namespace Internal {
		constexpr inline float DynamicsRate[] = {
			1.2f,
			1.1f,
			1.0f,
			0.85f,
			0.7f,
			0.5f,//n
			0.4f,
			0.35f,
			0.3f,
			0.2f,
			0.1f,
		};

		inline constexpr ::FD::Piano::Dynamics DynamicsEnd = ::FD::Piano::Dynamics::ffff;
	}

	enum class Command {
		//音を重ねる
		//既にPushされていたら何もしない
		PushPitch,
		PopPitch,

		//同じピッチを入れて長さを変える
		//途中でRest,PushPitchを入れても無視
		//ピッチを変えると自動的にPopPitchが呼ばれる
		//既にPushされていたら何もしない
		PushTie,

		//PushTieされていないときは何も起きない
		PopTie
	};
	namespace Internal {
		inline constexpr Command CommandEnd = Command::PopTie;
	}

	namespace Internal {
		enum class Key : UT {
			//pitch
			A0 = 0, A0S, A1, A1S, A2, A2S, A3, A3S, A4, A4S, A5, A5S, A6, A6S, A7, A7S,
			B0, B1, B2, B3, B4, B5, B6, B7,
			C1, C1S, C2, C2S, C3, C3S, C4, C4S, C5, C5S, C6, C6S, C7, C7S, C8,
			D1, D1S, D2, D2S, D3, D3S, D4, D4S, D5, D5S, D6, D6S, D7, D7S,
			E1, E2, E3, E4, E5, E6, E7,
			F1, F1S, F2, F2S, F3, F3S, F4, F4S, F5, F5S, F6, F6S, F7, F7S,
			G1, G1S, G2, G2S, G3, G3S, G4, G4S, G5, G5S, G6, G6S, G7, G7S,

			//tempo
			Largo,
			Adagio,
			Lento,
			Andante,
			Andantino,
			Moderate,
			AllegroModerate,
			Allegretto,
			Allegro,
			Vivace,
			Presto,
			Prestissimo,

			//TimeSignature
			FourFour,  // 4/4
			ThreeFour, // 3/4


			Whole,     //全音符
			Half,      //二分音符
			Quarter,   //四分音符
			Eighth,    //八分音符
			Sixteenth, //十六分音符


			WholeRest,     //全休符
			HalfRest,      //二分休符
			QuarterRest,   //四分休符
			EighthRest,    //八分休符
			SixteenthRest, //十六分休符


			pppp,
			ppp,
			pp,
			p,
			mp,
			n,
			mf,
			f,
			ff,
			fff,
			ffff,


			//Command
			PushPitch,
			PopPitch,
			PushTie,
			PopTie,


			/*******************************************************/

			Count,
			KeyBegin = A0,
			KeyEnd = Count - 1,

			PitchBegin = A0,
			PitchEnd = G7S,
			PitchCount = PitchEnd + 1,

			TempoBegin = PitchCount,
			TempoEnd = Prestissimo,
			TempoCount = TempoEnd + 1,

			TimeSignatureBegin = TempoCount,
			TimeSignatureEnd = ThreeFour,
			TimeSignatureCount = TimeSignatureEnd + 1,

			NoteBegin = TimeSignatureCount,
			NoteEnd = Sixteenth,
			NoteCount = NoteEnd + 1,

			RestBegin = NoteCount,
			RestEnd = SixteenthRest,
			RestCount = RestEnd + 1,

			DynamicsBegin = RestCount,
			DynamicsEnd = ffff,
			DynamicsCount = DynamicsEnd + 1,

			CommandBegin = DynamicsCount,
			CommandEnd = PopTie,
			CommandCount = CommandEnd + 1,

		};


	}

	//固定値
	constexpr FluSound::SaveFormat WavFormat = {
		FluSound::FormatType::PCM,
		1,
		44100,
		16
	};
}

namespace FD::Piano::Internal {
	//本当は静的に片づけたいが余りに多すぎるとコンパイルが通らない

	//基底の型に変換する
	//範囲を超えていたらInernalException
	UT pitchToUT(const Pitch pitch);
	UT dynamicsToUT(const Dynamics dynamics);
	UT keyToUT(const Key key);

	//keyに変換する
	//範囲を超えていたらInernalException
	Key pitchToKey(const Pitch pitch);
	Key tempoToKey(const Tempo tempo);
	Key timeSignatureToKey(const TimeSignature timeSignature);
	Key noteToKey(const Note note);
	Key restToKey(const Rest rest);
	Key dynamicsToKey(const Dynamics dynamics);
	Key commandToKey(const Command command);

	//keyから変換する
	//範囲を超えていたらInernalException
	Pitch keyToPitch(const Key key);
	Tempo keyToTempo(const Key key);
	TimeSignature keyToTimeSignature(const Key key);
	Note keyToNote(const Key key);
	Rest keyToRest(const Key key);
	Dynamics keyToDynamics(const Key key);
	Command keyToCommand(const Key key);

	
	//各値を取得する
	float dynamicsToVolume(const Dynamics dynamics);

}

namespace FD::Piano::Internal {

	template<Key Pitch>
	concept IsKeyRangePitch = Pitch >= Key::PitchBegin && Pitch <= Key::PitchEnd;
	_NODISCARD bool isKeyRangePitch(const Key key) noexcept;

	template<Key Tempo>
	concept IsKeyRangeTempo = Tempo >= Key::TempoBegin && Tempo <= Key::TempoEnd;
	_NODISCARD bool isKeyRangeTempo(const Key key) noexcept;

	template<Key TimeSignature>
	concept IsKeyRangeTimeSignature = TimeSignature >= Key::TimeSignatureBegin && TimeSignature <= Key::TimeSignatureEnd;
	_NODISCARD bool isKeyRangeTimeSignature(const Key key) noexcept;

	template<Key Note>
	concept IsKeyRangeNote = Note >= Key::NoteBegin && Note <= Key::NoteEnd;
	_NODISCARD bool isKeyRangeNote(const Key key) noexcept;

	template<Key Rest>
	concept IsKeyRangeRest = Rest >= Key::RestBegin && Rest <= Key::RestEnd;
	_NODISCARD bool isKeyRangeRest(const Key key) noexcept;

	template<Key Dynamics>
	concept IsKeyRangeDynamics = Dynamics >= Key::DynamicsBegin && Dynamics <= Key::DynamicsEnd;
	_NODISCARD bool isKeyRangeDynamics(const Key key) noexcept;

	template<Key Command>
	concept IsKeyRangeCommand = Command >= Key::CommandBegin && Command <= Key::CommandEnd;
	_NODISCARD bool isKeyRangeCommand(const Key key) noexcept;



}

namespace FD::Piano::Internal {
	//安全のため
	struct Limit final {
		constexpr static inline uint8_t ChordMax = 15;//15以上の音の重なりは許さない
	};
}