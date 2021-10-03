#pragma once

#include "FDbinding.h"

//前方宣言
namespace FD::Piano {

	namespace Internal {
		struct Read;
	}

}

namespace FD {

	class PianoSound final {
	public:
		explicit PianoSound(Internal::ManagerKey) {};

#ifdef  FluidumData_Sound_Debug//debug用
		PianoSound() = default;
#endif 
		~PianoSound() = default;
		FluidumUtils_Class_NoCopyNoMove(PianoSound)

	private:
		friend struct ::FD::Piano::Internal::Read;

	private://using 
		using Key = Piano::Internal::Key;
		using KeyUT = std::underlying_type_t<Key>;//keyの基底型

	private:
		FluSound::FluidumSound fluSound;
		std::vector<Piano::Pitch> loaded = {};


	public://鳴らす

		template<Piano::Pitch... Pitch>
		void sound() requires (sizeof...(Pitch) > 0) {
			fluSound.sound(Piano::Internal::PitchKeys[static_cast<std::underlying_type_t<Piano::Pitch>>(Pitch)]...);
		}

		template<typename ...T>
		void sound(T...pitch) requires(Fluidum::Utils::Concept::IsSame<Piano::Pitch, T...>) {
			fluSound.sound(Piano::Internal::PitchKeys[static_cast<std::underlying_type_t<Piano::Pitch>>(pitch)]...);
		}

		void sound(const std::vector<Piano::Pitch>& pitch);

		template<typename ...T>
		void sound(const float volume, T...pitch) requires(Fluidum::Utils::Concept::IsSame<Piano::Pitch, T...>) {
			fluSound.sound(volume, Piano::Internal::PitchKeys[static_cast<std::underlying_type_t<Piano::Pitch>>(pitch)]...);
		}

		void sound(const std::vector<std::pair<float,Piano::Pitch>>& data);
		void sound(const std::pair<float,Piano::Pitch>& data);

		//音をやめる
		template<typename ...T>
		void stop(T...pitch) requires(Fluidum::Utils::Concept::IsSame<Piano::Pitch, T...>) {
			for (auto&& x : { pitch... })
				fluSound.tryPopFront(Piano::Internal::PitchKeys[static_cast<std::underlying_type_t<Piano::Pitch>>(x)]);
		}


	public:
		//Pitchesのwavファイルを読み込んでセットする．
		//被っていたら被っているものは読み込まない．
		template<Piano::Pitch... Pitches>
		void set() requires (sizeof...(Pitches) > 0)
		{

#ifndef FluidumData_FilePath
			const auto path = Fluidum::Utils::File::getFullPath(__FILE__, 3, "\\resource\\FluidumDataSound\\piano\\");
#else
			const auto path = FluidumData_FilePath "/resource/FluidumDataSound/piano/
#endif

				const std::string extension = ".wav";

			for (auto&& x : { Pitches... }) {
				if (std::find(loaded.begin(), loaded.end(), x) == loaded.end()) {
					const char* key = Piano::Internal::PitchKeys[static_cast<std::underlying_type_t<Piano::Pitch>>(x)];
					const std::string filePath = path + key + extension;

					fluSound.add(filePath.c_str(), key);
					loaded.emplace_back(x);
				}
			}
		}

		//全てのpitchをセット
		void setAll();

		//A0 A0S B0
		void set0();

		//C1 C1S D1 D1S E1 F1 F1S G1 G1S A1 A1S B1
		void set1();

		//C2 C2S D2 D2S E2 F2 F2S G2 G2S A2 A2S B2
		void set2();

		//C3 C3S D3 D3S E3 F3 F3S G3 G3S A3 A3S B3
		void set3();

		//C4 C4S D4 D4S E4 F4 F4S G4 G4S A4 A4S B4
		void set4();

		//C5 C5S D5 D5S E5 F5 F5S G5 G5S A5 A5S B5
		void set5();

		//C6 C6S D6 D6S E6 F6 F6S G6 G6S A6 A6S B6
		void set6();

		//C7 C7S D7 D7S E7 F7 F7S G7 G7S A7 A7S B7
		void set7();

		//C8
		void set8();

	};

}

namespace FD::Piano::Internal {

	struct Read final {
		Read(PianoSound* const piano) :piano(piano) {}
		~Read() = default;
	private:
		PianoSound* const piano;
	public:
		const FluSound::i16Data* read(const char* key) {
			return piano->fluSound.read(key);
		}
		const FluSound::i16Data* read(const Pitch pitch) {
			return piano->fluSound.read(PitchKeys[pitchToUT(pitch)]);
		}
	};

}

//補助
namespace FD::Piano {
	template<typename T>
	T cast(const Pitch pitch) {
		return static_cast<T>(pitch);
	}
}