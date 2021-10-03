/*
アプリ全体を保存する
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
		std::atomic_bool* const isFinish = nullptr;//nullptrでなければ終了されたことを通知
	private:
		std::ofstream ofs;

	private:
		void write();

		void head();
		void cam();
		void genome();
	};
}