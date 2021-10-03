/*
�A�v���S�̂�ۑ�����
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

	class FluidumFileWrite final :public Scene {
	public:
		explicit FluidumFileWrite(
			Manager* manager,
			FD::MainCamera* camera,
			const std::string& filePath,
			std::atomic_bool* isFinish = nullptr
		);
		void Constructor(FD::MainCamera);

		~FluidumFileWrite();

	public:
		virtual void update() override;
		virtual void draw() override;

	private://data

		const FD::MainCamera* const camera = nullptr;
		const std::string filePath;
		std::atomic_bool* const isFinish = nullptr;//nullptr�łȂ���ΏI�����ꂽ���Ƃ�ʒm
	private:
		std::ofstream ofs;

	private:
		void write();

		void head();
		void cam();
		void genome();
	};
}