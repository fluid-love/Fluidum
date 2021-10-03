#pragma once

#include <iostream>

/*
S        : 変えることのできない上限
W        : 警告
何もない : ユーザー変えることの出来る上限
*/

namespace FD::Genome {
	using PopulationSize = uint32_t;
	using GenomeSize = uint32_t;
}

namespace FD::Genome::Internal {

	class Limit final {
	public:
		static constexpr PopulationSize populationSizeMaxS = 100000;//個体数最大
		static constexpr PopulationSize populationSizeMinS = 0;//個体数最小

		PopulationSize populationSizeMax = 50000;
		PopulationSize populationSizeMin = 0;

		PopulationSize populationSizeMaxW = populationSizeMax;

	public:
		static constexpr PopulationSize genomeSizeMaxS = 1000000;//ゲノムサイズ最大
		static constexpr PopulationSize genomeSizeMinS = 0;//ゲノムサイズ最小

		PopulationSize genomeSizeMax = 100000;
		PopulationSize genomeSizeMin = 0;

		PopulationSize genomeSizeMaxW = genomeSizeMax;
	};


}