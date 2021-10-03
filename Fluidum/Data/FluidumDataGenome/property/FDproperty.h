#pragma once

#include "FDlimit.h"
#include <fstream>
#include <string>
#include <FluidumFile/FluidumFile.h>
#include "../../resource/FDfilepath.h"

namespace FD::Genome {

	class Property final {
	public:
		Property();
		~Property() noexcept;

	private://path

#ifndef FluidumData_FilePath
		const std::string path = Fluidum::Utils::File::getFullPath(__FILE__, 3, "\\resource\\FluidumDataGenome\\genome.prop");
#else
		const std::string path = FluidumData_FilePath "/resource/FluidumDataGenome/genome.prop";
#endif

	public:
		Internal::Limit limit;


	};


}