#include "FDpiano.h"

void FD::PianoSound::sound(const std::vector<Piano::Pitch>& pitch) {
	std::vector<const char*> toChar(pitch.size());
	for (std::size_t i = 0; i < pitch.size(); i++) {
		toChar[i] = Piano::Internal::PitchKeys[static_cast<Piano::UT>(pitch[i])];
	}
	fluSound.sound(toChar);
}

void FD::PianoSound::sound(const std::vector<std::pair<float, Piano::Pitch>>& data) {
	std::vector<std::pair<float, std::string>> pitch(data.size());
	for (std::size_t i = 0; i < data.size(); i++) {
		pitch[i].first = data[i].first;
		pitch[i].second = Piano::Internal::PitchKeys[static_cast<Piano::UT>(data[i].second)];
	}
	fluSound.sound(pitch);
}

void FD::PianoSound::sound(const std::pair<float, Piano::Pitch>& data) {
	fluSound.sound(data.first, Piano::Internal::PitchKeys[static_cast<Piano::UT>(data.second)]);
}


void FD::PianoSound::setAll() {
	using enum Piano::Pitch;
	this->set<
		A0, A0S, A1, A1S, A2, A2S, A3, A3S, A4, A4S, A5, A5S, A6, A6S, A7, A7S,
		B0, B1, B2, B3, B4, B5, B6, B7,
		C1, C1S, C2, C2S, C3, C3S, C4, C4S, C5, C5S, C6, C6S, C7, C7S, C8,
		D1, D1S, D2, D2S, D3, D3S, D4, D4S, D5, D5S, D6, D6S, D7, D7S,
		E1, E2, E3, E4, E5, E6, E7,
		F1, F1S, F2, F2S, F3, F3S, F4, F4S, F5, F5S, F6, F6S, F7, F7S,
		G1, G1S, G2, G2S, G3, G3S, G4, G4S, G5, G5S, G6, G6S, G7, G7S
	>();
}

void FD::PianoSound::set0() {
	using enum Piano::Pitch;
	this->set<A0, A0S, B0>();
}

void FD::PianoSound::set1() {
	using enum Piano::Pitch;
	this->set<C1, C1S, D1, D1S, E1, F1, F1S, G1, G1S, A1, A1S, B1>();
}

void FD::PianoSound::set2() {
	using enum Piano::Pitch;
	this->set<C2, C2S, D2, D2S, E2, F2, F2S, G2, G2S, A2, A2S, B2>();
}

void FD::PianoSound::set3() {
	using enum Piano::Pitch;
	this->set<C3, C3S, D3, D3S, E3, F3, F3S, G3, G3S, A3, A3S, B3>();
}

void FD::PianoSound::set4() {
	using enum Piano::Pitch;
	this->set<C4, C4S, D4, D4S, E4, F4, F4S, G4, G4S, A4, A4S, B4>();
}

void FD::PianoSound::set5() {
	using enum Piano::Pitch;
	this->set<C5, C5S, D5, D5S, E5, F5, F5S, G5, G5S, A5, A5S, B5>();
}

void FD::PianoSound::set6() {
	using enum Piano::Pitch;
	this->set<C6, C6S, D6, D6S, E6, F6, F6S, G6, G6S, A6, A6S, B6>();
}

void FD::PianoSound::set7() {
	using enum Piano::Pitch;
	this->set<C7, C7S, D7, D7S, E7, F7, F7S, G7, G7S, A7, A7S, B7>();
}

void FD::PianoSound::set8() {
	using enum Piano::Pitch;
	this->set<C8>();
}

