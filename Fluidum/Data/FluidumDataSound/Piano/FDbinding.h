#pragma once

#include "../common/FDtype.h"

//前方宣言
namespace FD::Piano {
	class Play;
}

namespace FD::Piano::Internal {

	template<typename T>
	struct PairTemplate {
	private:
		PairTemplate() = delete;
	};

	template<>
	struct PairTemplate<Genome::OneByteType> {
		enum class PairTemplateType : uint16_t {
			All
		};
	};


}

namespace FD::Piano {

	//整数とその意味を結ぶ
	template<typename T>
	requires (std::integral<T>&& Genome::IsGenomeType<T>)
		class Pair final : protected Internal::PairTemplate<T> {
		private://using
			using Key = Internal::Key;

		public:
			//雛形
			using TemplateType = Internal::PairTemplate<T>::PairTemplateType;

		public:
			Pair() = default;
			~Pair() = default;

			//雛形から選択して構築
			template<TemplateType Type>
			Pair() {
				this->setBindTemplate<Type>();
			}

			FluidumUtils_Class_Default_CopyMove(Pair)

		private:
			template<typename GenomeType> requires(Genome::IsGenomeType<GenomeType>)
				friend class Save;

			friend class ::FD::Piano::Play;

		private:
			//数字(整数)とKeyを結びつける．
			std::map<T, Key> pairs;

		public:
			//templateをセット
			template<TemplateType Type>
			void setBindTemplate() {
				if constexpr (std::same_as<T, Genome::OneByteType>) {
					if constexpr (Type == TemplateType::All) {
						this->templateOneByte_All();
					}
				}
			}

			//新たなペアを入れる
			//既にあれば例外が出る
			void insert(const T integer, const Pitch pitch) {
				const auto key = Internal::pitchToKey(pitch);
				pairs.insert(std::make_pair(integer, key));
			}
			void insert(const T integer, const Tempo tempo) {
				const auto key = Internal::tempoToKey(tempo);
				pairs.insert(std::make_pair(integer, key));
			}
			void insert(const T integer, const TimeSignature timeSignature) {
				const auto key = Internal::timeSignatureToKey(timeSignature);
				pairs.insert(std::make_pair(integer, key));
			}
			void insert(const T integer, const Note note) {
				const auto key = Internal::noteToKey(note);
				pairs.insert(std::make_pair(integer, key));
			}
			void insert(const T integer, const Rest rest) {
				const auto key = Internal::restToKey(rest);
				pairs.insert(std::make_pair(integer, key));
			}
			void insert(const T integer, const Dynamics dynamics) {
				const auto key = Internal::dynamicsToKey(dynamics);
				pairs.insert(std::make_pair(integer, key));
			}
			void insert(const T integer, const Command command) {
				const auto key = Internal::commandToKey(command);
				pairs.insert(std::make_pair(integer, key));
			}


			//新たなペアを入れる
			//既にあれば置き換える
			void forceInsert(const T integer, const Pitch pitch) {
				const auto key = Internal::pitchToKey(pitch);
				pairs.insert_or_assign(std::make_pair(integer, key));
			}
			void forceInsert(const T integer, const Tempo tempo) {
				const auto key = Internal::tempoToKey(tempo);
				pairs.insert_or_assign(std::make_pair(integer, key));
			}
			void forceInsert(const T integer, const TimeSignature timeSignature) {
				const auto key = Internal::timeSignatureToKey(timeSignature);
				pairs.insert_or_assign(std::make_pair(integer, key));
			}
			void forceInsert(const T integer, const Note note) {
				const auto key = Internal::noteToKey(note);
				pairs.insert_or_assign(std::make_pair(integer, key));
			}
			void forceInsert(const T integer, const Rest rest) {
				const auto key = Internal::restToKey(rest);
				pairs.insert_or_assign(std::make_pair(integer, key));
			}
			void forceInsert(const T integer, const Dynamics dynamics) {
				const auto key = Internal::dynamicsToKey(dynamics);
				pairs.insert_or_assign(std::make_pair(integer, key));
			}
			void forceInsert(const T integer, const Command command) {
				const auto key = Internal::commandToKey(command);
				pairs.insert_or_assign(std::make_pair(integer, key));
			}


			//新たなペアを入れる
			//既にあれば変更しない
			bool tryInsert(const T integer, const Pitch pitch) {
				const auto key = Internal::pitchToKey(pitch);
				return pairs.try_emplace(integer, key).second;
			}
			bool tryInsert(const T integer, const Tempo tempo) {
				const auto key = Internal::tempoToKey(tempo);
				return pairs.try_emplace(integer, key).second;
			}
			bool tryInsert(const T integer, const TimeSignature timeSignature) {
				const auto key = Internal::timeSignatureToKey(timeSignature);
				return pairs.try_emplace(integer, key).second;
			}
			bool tryInsert(const T integer, const Note note) {
				const auto key = Internal::noteToKey(note);
				return pairs.try_emplace(integer, key).second;
			}
			bool tryInsert(const T integer, const Rest rest) {
				const auto key = Internal::restToKey(rest);
				return pairs.try_emplace(integer, key).second;
			}
			bool tryInsert(const T integer, const Dynamics dynamics) {
				const auto key = Internal::dynamicsToKey(dynamics);
				return pairs.try_emplace(integer, key).second;
			}
			bool tryInsert(const T integer, const Command command) {
				const auto key = Internal::commandToKey(command);
				return pairs.try_emplace(integer, key).second;
			}





			//異常がないか確認する
			bool check(const CppGA::Genome<T>* const genome)const {

			}

		private://internal

			void templateOneByte_All() {
				static_assert(std::same_as<T, Genome::OneByteType>);
				static_assert(std::numeric_limits<T>::max() > static_cast<std::underlying_type_t<Internal::Key>>(Internal::Key::Count));

				for (T i = 0; i < static_cast<T>(Internal::Key::Count); i++) {
					pairs.insert(std::make_pair(i, static_cast<Internal::Key>(i)));
				}
			}

	};
}
