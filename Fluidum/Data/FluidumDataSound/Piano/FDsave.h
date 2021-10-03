/*
wavファイルとして保存する
*/

#pragma once

#include "FDpiano.h"
#include "FDcommand.h"

namespace FD::Piano::Internal {
	class PitchCount final {
	public:
		operator bool()const noexcept;
		void operator=(const bool val)noexcept;
		void operator++(int) noexcept;
		_NODISCARD bool isEmpty()const noexcept;
		void reset()noexcept;
	private:
		bool flag = false;
		uint16_t size = 0;
	};

	class TieCount final {
	public:
		operator bool()const noexcept;
		void operator=(const bool val) noexcept;

		//初回のみvolumeとpitchはいれられる
		bool plusDuration(const float volume, const Pitch pitch, const std::size_t duration) noexcept;

		void reset() noexcept;
		std::tuple<float, Pitch, std::size_t> get_reset();
		_NODISCARD bool isEmpty()const noexcept;
	private:
		float volume = 0.0f;
		std::optional<Pitch> pitch = std::nullopt;
		bool flag = false;
		std::size_t duration = 0;
	};
}

namespace FD::Piano {

	struct SaveParameter final {
		std::string fileDirectory = "";
		std::string fileName = "";
	};

	//genomeからサウンドファイルを出力する
	template<typename GenomeType>
	requires(Genome::IsGenomeType<GenomeType>)
		class Save final {
		public:

			template<typename T> requires(std::same_as<std::remove_reference_t<T>, SaveParameter>)
				Save(T&& param, FD::PianoSound* const piano, const Piano::Pair<GenomeType>* const bind, const CppGA::Genome<GenomeType>* const genome)
				:param(std::forward<SaveParameter>(param)), piano(piano), bind(bind), genome(genome)
			{
				assert(this->bind && this->genome);
				this->piano->setAll();
				this->save();
			};

			~Save() = default;
			FluidumUtils_Class_NoCopyNoMove(Save)

		private://using
			using Key = Internal::Key;

		private://data
			FD::PianoSound* const piano = nullptr;
			const Piano::Pair<GenomeType>* const bind = nullptr;
			const CppGA::Genome<GenomeType>* const genome = nullptr;

			const SaveParameter param;

			std::unique_ptr<FluSound::SaveWav> saveWav = nullptr;
		private:
			//四分音符一個のミリ秒数
			//初期値は♩=120
			std::size_t tempoMilliSeconds = static_cast<std::size_t>(60.0 / 120.0 * 1000.0);

			//間隔(milli秒)
			//初期値は四分音符
			std::size_t duration = tempoMilliSeconds;

			//音量
			Dynamics dynamics = Dynamics::n;

			//重複
			Internal::PitchCount pitchCount;
			FluSound::SaveWav::Pos plusPos = 0;

			Internal::TieCount tieCount;
		private:

			void save() {
				FluSound::SaveWav::Parameter saveWavParam;
				saveWavParam.fileName = param.fileName;
				saveWavParam.fileDirectory = param.fileDirectory;
				saveWavParam.format = Piano::WavFormat;//固定値
				saveWav = std::make_unique<FluSound::SaveWav>(std::move(saveWavParam));

				for (auto&& x : *genome) {
					//定義されていれば呼ぶそうでなければ無視
					if (bind->pairs.contains(x))
						this->calc(bind->pairs.at(x));
				}
			}

		private:

			//Playクラスとは違い,リアルタイム性は求めない
			//時間はかかってもよいので動的にif分で分岐
			void calc(const Key key) {

				//pitchを意味する場合
				if (Internal::isKeyRangePitch(key)) {
					this->pitch(key);
				}
				else if (Internal::isKeyRangeTempo(key)) {
					this->tempo(key);
				}
				else if (Internal::isKeyRangeNote(key)) {
					this->note(key);
				}
				else if (Internal::isKeyRangeDynamics(key)) {
					this->setDynamics(key);
				}
				else if (key == Key::PushPitch) {
					pitchCount = true;
				}
				else if (key == Key::PopPitch) {
					pitchCount = false;
					pitchCount.reset();
				}
				else if (key == Key::PushTie) {
					tieCount = true;
				}
				else if (key == Key::PopTie) {
					this->popTie();
					tieCount = false;
					tieCount.reset();
				}

			}


			//Piano::WavFormat::SamplingRate == 44100
			//1000ミリ秒あたり44100 
			//欲しいミリ秒数はduration
			void pitch(const Internal::Key val) {

				//tie
				const bool fin = this->tie(Internal::keyToPitch(val));
				if (!fin)
					return;


				FluSound::i16Data data = this->getCopiedData(Internal::keyToPitch(val));

				//音量の調整
				this->changeVolume(Internal::dynamicsToVolume(dynamics), data);

				const std::size_t count = calcDataCount(duration);
				const auto itr = count >= data.size() ? data.cend() : (data.cbegin() + count);

				//詰む かつ 空じゃない==初回じゃない
				//空の場合は普通にpushする
				if (!pitchCount.isEmpty())
					saveWav->plus(plusPos, data.cbegin(), itr);
				else
					this->plusPos = saveWav->push(data.cbegin(), itr);

				if (pitchCount)
					pitchCount++;
			}

			void tempo(const Internal::Key val) {
				//Tempo == 60秒間の四分音符数
				//60なら四分音符の間は1秒=1000ミリ秒
				using TempoUT = std::underlying_type_t<Tempo>;
				const std::size_t milliSeconds = static_cast<std::size_t>((60.0 / static_cast<double>(Internal::Tempos[static_cast<UT>(val) - static_cast<UT>(Internal::Key::TempoBegin)])) * 1000.0);
				this->tempoMilliSeconds = milliSeconds;
			}

			//durationを変える　つまりpitchで鳴らす時間を変更
			//tempoMilliSecondsは四分音符一個の時間
			void note(const Internal::Key val) {
				if (val == Key::Whole)
					this->duration = tempoMilliSeconds * 4;
				else if (val == Key::Half)
					this->duration = tempoMilliSeconds * 2;
				else if (val == Key::Quarter)
					this->duration = tempoMilliSeconds;
				else if (val == Key::Eighth)
					this->duration = tempoMilliSeconds / 2;
				else if (val == Key::Sixteenth)
					this->duration = tempoMilliSeconds / 4;
			}


			//休符
			//0のvectorをプッシュ
			void rest(const Key val) {

				//四分音符1つ分のサイズ
				const std::size_t size = static_cast<std::size_t>(((double)WavFormat.samplingRate / 1000.0) * (double)tempoMilliSeconds);
				std::vector<int16_t> data(1000);

				if (val == Key::WholeRest)
					saveWav->push(data.cbegin(), data.cend(), size * 4);
				else if (val == Key::HalfRest)
					saveWav->push(data.cbegin(), data.cend(), size * 2);
				else if (val == Key::QuarterRest)
					saveWav->push(data.cbegin(), data.cend(), size);
				else if (val == Key::EighthRest)
					saveWav->push(data.cbegin(), data.cend(), size / 2);
				else if (val == Key::SixteenthRest)
					saveWav->push(data.cbegin(), data.cend(), size / 4);
			}

			void setDynamics(const Key val) {
				this->dynamics = Internal::keyToDynamics(val);
			}

			//データを入れたらtrue
			_NODISCARD bool tie(const Pitch pitch) {
				if (!tieCount)
					return true;

				const bool fin = tieCount.plusDuration(Internal::dynamicsToVolume(dynamics), pitch, duration);

				//前と違うピッチ -> データを入れる
				if (!fin)
					return false;

				popTie();
				return false;
			}

			void popTie() {
				if (!tieCount|| tieCount.isEmpty())
					return;

				const auto [vol, pit, dur] = tieCount.get_reset();

				/*dataを入れる*/
				//どれだけ必要か
				const std::size_t count = calcDataCount(dur);

				//生データ
				FluSound::i16Data data = getCopiedData(pit);

				//音量
				changeVolume(vol,data);


				//詰む　かつ　空じゃない
				//空の場合は普通にpushする
				if (pitchCount && !pitchCount.isEmpty()) {
					//もし範囲を超えていたら最後まで
					saveWav->plus(plusPos, data.cbegin(), count >= data.size() ? data.cend() : (data.cbegin() + count));
				}
				else {
					this->plusPos = saveWav->push(data.cbegin(), count >= data.size() ? data.cend() : (data.cbegin() + count));
				}
			}

		private://補助
			std::size_t calcDataCount(const std::size_t dur) {
				const double samplingRate = static_cast<double>(WavFormat.samplingRate);
				const double result = (samplingRate / 1000.0) * static_cast<double>(dur);
				return static_cast<std::size_t>(result);
			}

			_NODISCARD FluSound::i16Data getCopiedData(const Pitch pitch) const {
				Internal::Read read(piano);
				const FluSound::i16Data* const ptr = read.read(pitch);
				return FluSound::i16Data(*ptr);
			}

			void changeVolume(const float volume, FluSound::i16Data& data) {
				for (auto& x : data)
					x = static_cast<FluSound::i16Data::value_type>(x * volume);
			}
	};

}