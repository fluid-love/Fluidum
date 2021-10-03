#include "FSguitext.h"

#include <FluidumFile/FluidumFile.h>

FS::Text::Analysis::Analysis() {
#ifndef FluidumScene_FilePath
	const auto path = Fluidum::Utils::File::getFullPath(__FILE__, 3, "\\resource\\FluidumSceneAnalysis\\analysis.jpn");
#else 
	const auto path = FluidumScene_FilePath "/FluidumSceneAnalysis/analysis.jpn";
#endif
	std::ifstream ifs(path, std::ios::in);

	if (!ifs)
		throw std::runtime_error("Failed to open analysis.jpn.");

	std::string data = "";

	std::getline(ifs, data);
	this->plot = data;//ƒOƒ‰ƒt

	std::getline(ifs, data);
	this->function = data;//ŠÖ”

}
