#include "FSfluidumread.h"

FS::FluidumFileRead::FluidumFileRead(
	Manager* manager,
	FD::MainCamera* camera,
	const std::string& filePath
)
	:Scene(manager), camera(camera), filePath(filePath)
{
	FluidumUtils_ConsoleLog_Constructor(FluidumFileRead)
}

FS::FluidumFileRead::~FluidumFileRead() {
	FluidumUtils_ConsoleLog_Destructor(FluidumFileRead)
}

void FS::FluidumFileRead::update() {
	std::call_once(this->once, &FluidumFileRead::write, this);
}

void FS::FluidumFileRead::draw() {

}

void FS::FluidumFileRead::write() {
	this->ifs.open(this->filePath, std::ios::binary | std::ios::trunc);

	this->head();
	this->cam();
	this->genome();
}

void FS::FluidumFileRead::head() {
	std::size_t dummy = 0;
	ifs.read(reinterpret_cast<char*>(&dummy), 4);
	ifs.read(reinterpret_cast<char*>(&dummy), 4);

}

void FS::FluidumFileRead::cam() {

	////main camera
	//ifs.read(reinterpret_cast<char*>(&camera->camera.pos), sizeof(camera->camera.pos));
	//ifs.read(reinterpret_cast<char*>(&camera->camera.center), sizeof(camera->camera.center));
	//ifs.read(reinterpret_cast<char*>(&camera->camera.up), sizeof(camera->camera.up));
	//ifs.read(reinterpret_cast<char*>(&camera->camera.aspect), sizeof(camera->camera.aspect));

	//ifs.read(reinterpret_cast<char*>(&camera->speed), sizeof(camera->speed));
}

void FS::FluidumFileRead::genome() {
/*
		std::ifstream ifs("test.txt", std::ios::binary);

		std::vector<char> temp;
		char size = 0;
		ifs.read(&size, 1);
		temp.resize(size);
		std::vector<bool> data(size);


		ifs.read(reinterpret_cast<char*>(temp.data()), size);

		for (std::vector<bool>::size_type i = 0; const auto & x : temp) {
			data[i] = (x == 1) ? true : false;
			i++;
		}

		for (const auto& x : data) {
			std::cout << std::boolalpha << x.operator bool() << "\n";
		}

*/



}



























