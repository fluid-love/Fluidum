#include "FDplay.h"

FD::Piano::Play::Play(PianoSound* const piano) :piano(piano) {
	assert(this->piano);

}

void FD::Piano::Play::call(const std::size_t i) {
	(this->*functions.at(i))();
}

void FD::Piano::Play::terminate() {
	pitchCommand.reset();
	tie.reset();
}

void FD::Piano::Play::pushPitch() noexcept {
	pitchCommand = true;
}

void FD::Piano::Play::popPitch() {
	//pushしてないのにpopしようとした->無視
	//pushしてすぐpop->無視
	if (!pitchCommand || pitchCommand.isEmpty())
		return;

	this->wait();

	const auto [pitch, time] = pitchCommand.reset_get();
	assert(pitch.size() == time.size());

	//鳴らす音は何ミリ秒後に消すかをセット
	for (std::size_t i = 0; i < pitch.size(); i++) {
		stopPitch.add(pitch[i].second, time[i]);
	}

	piano->sound(pitch);
}

void FD::Piano::Play::pushTie() noexcept {
	tie = true;
}

void FD::Piano::Play::popTie() {
	//pushしてないのにpopしようとした->無視 
	//pushしてすぐpop-> 無視
	if (!tie || tie.isEmpty())
		return;

	//pitchを詰むならPopPitchで鳴らす
	//単発ならそのまま鳴らす
	if (pitchCommand) {
		const auto [pitvol, dur] = tie.reset_getDuration();
		pitchCommand.push(pitvol.first, pitvol.second, dur);
		return;
	}

	this->wait();

	const auto [pitch, time] = tie.reset_get();

	//鳴らす音は何ミリ秒後に消すかをセット
	stopPitch.add(pitch.second, time);

	//鳴らす
	piano->sound(pitch);
}

void FD::Piano::Play::wait() {
	//待機する
	std::this_thread::sleep_until(this->sleepUntil);
	//次の時間をセット
	this->sleepUntil = std::chrono::system_clock::now() + this->duration;
}

void FD::Piano::Play::wait(const std::chrono::system_clock::duration& dur) {
	//待機する
	std::this_thread::sleep_until(this->sleepUntil);
	//次の時間をセット
	this->sleepUntil += dur;
}

void FD::Piano::Play::stopFunction(const Internal::StopPitchStackType& stack) {
	const auto now = std::chrono::system_clock::now();

	for (const auto& x : stack) {
		if (now >= x.second) {
			piano->stop(x.first);
			stopPitch.erase(x.first);
		}
	}
}

bool FD::Piano::Play::pushPitchCommand(const Pitch pitch) {
	if (pitchCommand) {
		pitchCommand.push(dynamics, static_cast<Pitch>(pitch), duration);
		return true;
	}
	return false;
}

bool FD::Piano::Play::pushTieCommand(const Pitch pitch) {

	if (!tie)
		return false;

	//前とピッチが変わっている->tie終了 tieをpop
	const bool changed = tie.isPitchChanged(pitch);

	if (changed) {
		popTie();
		//変わっていたら変わった音を鳴らさないといけない
		return false;
	}
	else
		tie.push(dynamics, duration);

	return true;

}