/*
Head
	camera�̃o�C�g�� 4byte
	genome�̃o�C�g�� 4byte
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
	genome�̒��g -> �r�b�g�������Ă��� 1 �����Ă��Ȃ� 0 = �eibyte
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
