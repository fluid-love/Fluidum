#include "FDtype.h"

[[noreturn]] void throwOutOfRange() {
	throw FD::Sound::InternalError(FD::Piano::Internal::ExceptionMessageOutOfRangeKey);
}

FD::Piano::UT FD::Piano::Internal::pitchToUT(const Pitch pitch) {
	if (pitch > PitchEnd)
		throwOutOfRange();
	return static_cast<UT>(pitch);
}

FD::Piano::UT FD::Piano::Internal::dynamicsToUT(const Dynamics dynamics) {
	if (dynamics > DynamicsEnd)
		throwOutOfRange();
	return static_cast<UT>(dynamics);
}

FD::Piano::UT FD::Piano::Internal::keyToUT(const Key key) {
	if (key > Key::KeyEnd)
		throwOutOfRange();
	return static_cast<UT>(key);
}

FD::Piano::Internal::Key FD::Piano::Internal::pitchToKey(const Pitch pitch) {
	const auto result = static_cast<Key>(pitch);
	if (!isKeyRangePitch(result))
		throwOutOfRange();
	return result;
}

FD::Piano::Internal::Key FD::Piano::Internal::tempoToKey(const Tempo tempo) {
	const auto result = static_cast<Key>(static_cast<UT>(tempo) + static_cast<UT>(Key::TempoBegin));
	if (!isKeyRangeTempo(result))
		throwOutOfRange();
	return result;
}

FD::Piano::Internal::Key FD::Piano::Internal::timeSignatureToKey(const TimeSignature timeSignature) {
	const auto result = static_cast<Key>(static_cast<UT>(timeSignature) + static_cast<UT>(Key::TimeSignatureBegin));
	if (!isKeyRangeTimeSignature(result))
		throwOutOfRange();
	return result;
}

FD::Piano::Internal::Key FD::Piano::Internal::noteToKey(const Note note) {
	const auto result = static_cast<Key>(static_cast<UT>(note) + static_cast<UT>(Key::NoteBegin));
	if (!isKeyRangeNote(result))
		throwOutOfRange();
	return result;
}

FD::Piano::Internal::Key FD::Piano::Internal::restToKey(const Rest rest) {
	const auto result = static_cast<Key>(static_cast<UT>(rest) + static_cast<UT>(Key::RestBegin));
	if (!isKeyRangeRest(result))
		throwOutOfRange();
	return result;
}

FD::Piano::Internal::Key FD::Piano::Internal::dynamicsToKey(const Dynamics dynamics) {
	const auto result = static_cast<Key>(static_cast<UT>(dynamics) + static_cast<UT>(Key::DynamicsBegin));
	if (!isKeyRangeDynamics(result))
		throwOutOfRange();
	return result;
}

FD::Piano::Internal::Key FD::Piano::Internal::commandToKey(const Command command) {
	const auto result = static_cast<Key>(static_cast<UT>(command) + static_cast<UT>(Key::CommandBegin));
	if (!isKeyRangeCommand(result))
		throwOutOfRange();
	return result;
}

FD::Piano::Pitch FD::Piano::Internal::keyToPitch(const Key key) {
	const auto result = static_cast<Pitch>(key);
	if (result > PitchEnd)
		throwOutOfRange();
	return result;
}

FD::Piano::Tempo FD::Piano::Internal::keyToTempo(const Key key) {
	const auto result = static_cast<Tempo>(static_cast<UT>(key) - static_cast<UT>(Key::TempoBegin));
	if (result > TempoEnd)
		throwOutOfRange();
	return result;
}

FD::Piano::TimeSignature FD::Piano::Internal::keyToTimeSignature(const Key key) {
	const auto result = static_cast<TimeSignature>(static_cast<UT>(key) - static_cast<UT>(Key::TimeSignatureBegin));
	if (result > TimeSignatureEnd)
		throwOutOfRange();
	return result;
}

FD::Piano::Note FD::Piano::Internal::keyToNote(const Key key) {
	const auto result = static_cast<Note>(static_cast<UT>(key) - static_cast<UT>(Key::NoteBegin));
	if (result > NoteEnd)
		throwOutOfRange();
	return result;
}

FD::Piano::Rest FD::Piano::Internal::keyToRest(const Key key) {
	const auto result = static_cast<Rest>(static_cast<UT>(key) - static_cast<UT>(Key::RestBegin));
	if (result > RestEnd)
		throwOutOfRange();
	return result;
}

FD::Piano::Dynamics FD::Piano::Internal::keyToDynamics(const Key key) {
	const auto result = static_cast<Dynamics>(static_cast<UT>(key) - static_cast<UT>(Key::DynamicsBegin));
	if (result > DynamicsEnd)
		throwOutOfRange();
	return result;
}

FD::Piano::Command FD::Piano::Internal::keyToCommand(const Key key) {
	const auto result = static_cast<Command>(static_cast<UT>(key) - static_cast<UT>(Key::CommandBegin));
	if (result > CommandEnd)
		throwOutOfRange();
	return result;
}

float FD::Piano::Internal::dynamicsToVolume(const Dynamics dynamics) {
	const UT index = dynamicsToUT(dynamics);
	return DynamicsRate[index];
}

bool FD::Piano::Internal::isKeyRangePitch(const Key key) noexcept {
	return (key <= Key::PitchEnd);
}

bool FD::Piano::Internal::isKeyRangeTempo(const Key key) noexcept {
	return (key >= Key::TempoBegin && key <= Key::TempoEnd);
}

bool FD::Piano::Internal::isKeyRangeTimeSignature(const Key key) noexcept {
	return (key >= Key::TimeSignatureEnd && key <= Key::TimeSignatureEnd);;
}

bool FD::Piano::Internal::isKeyRangeNote(const Key key) noexcept {
	return (key >= Key::NoteBegin && key <= Key::NoteEnd);
}

bool FD::Piano::Internal::isKeyRangeRest(const Key key) noexcept {
	return (key >= Key::RestBegin && key <= Key::RestEnd);
}

bool FD::Piano::Internal::isKeyRangeDynamics(const Key key) noexcept {
	return (key >= Key::DynamicsBegin && key <= Key::DynamicsEnd);
}

bool FD::Piano::Internal::isKeyRangeCommand(const Key key) noexcept {
	return (key >= Key::CommandBegin && key <= Key::CommandEnd);
}


