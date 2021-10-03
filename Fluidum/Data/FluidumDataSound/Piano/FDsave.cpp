#include "FDsave.h"

void FD::Piano::Internal::PitchCount::operator=(const bool val) noexcept {
	flag = val;
}

FD::Piano::Internal::PitchCount::operator bool()const noexcept {
	return flag;
}

void FD::Piano::Internal::PitchCount::operator++(int) noexcept {
	//�ő�l�𒴂��Ă���Ɖ������Ȃ�
	if (size > Limit::ChordMax)
		return;
	size++;
}

bool FD::Piano::Internal::PitchCount::isEmpty() const noexcept {
	return (size == 0);
}

void FD::Piano::Internal::PitchCount::reset() noexcept {
	size = 0;
	flag = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FD::Piano::Internal::TieCount::operator bool()const noexcept {
	return this->flag;
}

bool FD::Piano::Internal::TieCount::plusDuration(const float volume, const Pitch pitch, const std::size_t duration) noexcept {
	//����
	if (!this->pitch.has_value()) {
		this->pitch = pitch;
		this->volume = volume;
	}

	//�����s�b�`
	if (this->pitch.value() == pitch) {
		this->duration += duration;
		return false;
	}
	//�Ⴄ�s�b�`���n���ꂽ->�I��
	else {
		return true;
	}
}

void FD::Piano::Internal::TieCount::operator=(const bool val) noexcept {
	flag = val;
}

void FD::Piano::Internal::TieCount::reset() noexcept {
	duration = 0;
	pitch = std::nullopt;
	flag = false;
}

std::tuple<float, FD::Piano::Pitch, std::size_t> FD::Piano::Internal::TieCount::get_reset() {
	if (!pitch.has_value())
		throw Sound::InternalError("");
	const auto result = std::make_tuple(volume,pitch.value(),duration);
	reset();
	return result;
}

bool FD::Piano::Internal::TieCount::isEmpty()const noexcept {
	return !pitch.has_value();
}