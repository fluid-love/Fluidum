#include "FDcommand.h"

FD::Piano::Internal::PitchCommand::operator bool() const noexcept {
	return this->flag;
}

void FD::Piano::Internal::PitchCommand::operator=(const bool flag) noexcept {
	this->flag = flag;
}

bool FD::Piano::Internal::PitchCommand::push(const float volume, const Pitch pitch, const std::chrono::system_clock::duration& duration) {
	//最大の重複を超えている->無視
	if (stack.size() > Internal::Limit::ChordMax)
		return false;

	stack.emplace_back(std::make_pair(volume, pitch));
	this->duration.emplace_back(duration);
	return true;
}

std::pair<std::vector<std::pair<float, FD::Piano::Pitch>>, std::vector<std::chrono::system_clock::time_point>> FD::Piano::Internal::PitchCommand::reset_get() {

	std::pair<std::vector<std::pair<float, Pitch>>, std::vector<std::chrono::system_clock::time_point>> result;
	assert(stack.size() == duration.size());
	const auto size = stack.size();
	result.second.resize(size);

	//std::vector<std::pair<float, Pitch> ならす音量とピッチ
	result.first = std::move(stack);

	//いつまで鳴らすか(time point)
	const auto now = std::chrono::system_clock::now();
	for (std::size_t i = 0; i < size; i++) {
		result.second[i] = now + duration[i];
	}

	reset();

	return result;
}

void FD::Piano::Internal::PitchCommand::reset() noexcept {
	duration.clear();
	duration.shrink_to_fit();
	stack.clear();
	stack.shrink_to_fit();
	flag = false;
}

bool FD::Piano::Internal::PitchCommand::isEmpty() const noexcept {
	return stack.empty();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FD::Piano::Internal::TieCommand::operator bool() const noexcept {
	return this->flag;
}

void FD::Piano::Internal::TieCommand::operator=(const bool flag) noexcept {
	this->flag = flag;
}

void FD::Piano::Internal::TieCommand::push(const float volume, const std::chrono::system_clock::duration& duration) {
	this->volume = volume;
	this->duration += duration;
}

bool FD::Piano::Internal::TieCommand::isPitchChanged(const Pitch pitch) {

	if (!this->pitch.has_value())
		this->pitch = pitch;

	//前と同じなら
	if (this->pitch == pitch)
		return false;

	return true;
}

std::pair<std::pair<float, FD::Piano::Pitch>, std::chrono::system_clock::time_point> FD::Piano::Internal::TieCommand::reset_get() {
	auto result = std::make_pair(std::make_pair(volume, this->pitch.value()), std::chrono::system_clock::now() + this->duration);
	reset();
	return result;
}

std::pair<std::pair<float, FD::Piano::Pitch>, std::chrono::system_clock::duration> FD::Piano::Internal::TieCommand::reset_getDuration() {
	auto result = std::make_pair(std::make_pair(volume, this->pitch.value()), this->duration);
	reset();
	return result;
}

void FD::Piano::Internal::TieCommand::reset() {
	volume = 0.0f;
	flag = false;
	duration = std::chrono::system_clock::duration();
	pitch = std::nullopt;
}

bool FD::Piano::Internal::TieCommand::isEmpty() const noexcept {
	return !pitch.has_value();
}
