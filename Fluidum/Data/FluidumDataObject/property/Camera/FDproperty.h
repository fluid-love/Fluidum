#pragma once

#include "FDlimit.h"
#include <fstream>
#include <string>
#include <FluidumFile/FluidumFilePath.h>
#include "../../../resource/FDfilepath.h"

namespace FD::Object::Camera {

	/*
	メインカメラに適用される値
	*/
	class Property final {
	public:
		Property();
		~Property() noexcept;

	private://path

#ifndef FluidumData_FilePath
		const std::string path = Fluidum::Utils::File::getFullPath(__FILE__, 4, "\\resource\\FluidumDataObject\\camera.prop");
#else
		const std::string path = FluidumData_FilePath "/resource/FluidumDataObject/camera.prop";
#endif

	public:
		Limit limit;


	};


}