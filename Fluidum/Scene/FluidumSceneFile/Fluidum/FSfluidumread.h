/*
Head
	cameraのバイト数 4byte
	genomeのバイト数 4byte
Camera
	pos
	center
	up
	aspect
	speed
Genome
	genomeType -> 0 bin 1 dbl = 1byte
	populationSize 8byte
	genomeSize = 8byte * populationSize
	genomeの中身 -> ビットが立っている 1 立っていない 0 = 各ibyte
*/

#pragma once

#include "../../common/FSusing.h"

namespace FS {

	class FluidumFileRead final :public Scene {
	public:
		explicit FluidumFileRead(
			Manager* manager,
			FD::MainCamera* camera,
			const std::string& filePath
		);
		void Constructor(FD::MainCamera);

		~FluidumFileRead();

	public:
		virtual void update() override;
		virtual void draw() override;

	private://data

		FD::MainCamera* const camera = nullptr;
		const std::string filePath;
		std::once_flag once;

	private:
		std::ifstream ifs;

	private:
		void write();

		void head();
		void cam();
		void genome();
	};
}
