#include "FSfluidumwrite.h"

FS::FluidumFileWrite::FluidumFileWrite(
	Manager* manager,
	FD::MainCamera* camera,
	const std::string& filePath,
	std::atomic_bool* isFinish
)
	:Scene(manager), camera(camera), filePath(filePath), isFinish(isFinish)
{
#ifndef NDEBUG
	if (isFinish) {
		assert(isFinish->load() == false);
	}
#endif

	FluidumUtils_ConsoleLog_Constructor(FluidumFileWrite)
}

FS::FluidumFileWrite::~FluidumFileWrite() {
	FluidumUtils_ConsoleLog_Destructor(FluidumFileWrite)
}

void FS::FluidumFileWrite::update() {
	this->write();
	Scene::deleteAsyncScene<FS::FluidumFileWrite>();

	if (isFinish)
		isFinish->store(true);
}

void FS::FluidumFileWrite::draw() {

}

void FS::FluidumFileWrite::write() {
	this->ofs.open(this->filePath, std::ios::binary | std::ios::trunc);

	this->head();
	this->cam();
	this->genome();
}

void FS::FluidumFileWrite::head() {
	std::size_t dummy = 0;
	ofs.write(reinterpret_cast<char*>(&dummy), 4);
	ofs.write(reinterpret_cast<char*>(&dummy), 4);

}

void FS::FluidumFileWrite::cam() {

	////main camera
	//ofs.write(reinterpret_cast<const char*>(&camera->camera.pos), sizeof(camera->camera.pos));
	//ofs.write(reinterpret_cast<const char*>(&camera->camera.center), sizeof(camera->camera.center));
	//ofs.write(reinterpret_cast<const char*>(&camera->camera.up), sizeof(camera->camera.up));
	//ofs.write(reinterpret_cast<const char*>(&camera->camera.aspect), sizeof(camera->camera.aspect));

	//ofs.write(reinterpret_cast<const char*>(&camera->speed), sizeof(camera->speed));




}

void FS::FluidumFileWrite::genome() {
	//const FD::GenomeRead* const data = Scene::getData<FD::GenomeRead>();

	//const auto currentType = data->currentType();


	//if (currentType == FD::Genome::Type::BINARY) {
	//	ofs.write(reinterpret_cast<const char*>(0), 1);

	//	const FD::Genome::Genomes<FD::Genome::BinType>* const genomes = data->get<FD::Genome::BinType>();

	//	const auto populationSize = data->populationSize();
	//	static_assert(sizeof(decltype(populationSize)) >= 8);
	//	ofs.write(reinterpret_cast<const char*>(&populationSize), 8);

	//	for (auto i = 0; i < populationSize; i++) {

	//		std::vector<char> temp(genomes[i].size());
	//		for (std::vector<char>::size_type i = 0; const auto & x : (*genomes)[i]) {
	//			temp[i] = x ? 1 : 0;
	//			i++;
	//		}

	//		ofs.write(reinterpret_cast<char*>(temp.data()), sizeof(decltype(temp)::value_type) * temp.size());

	//	}

	//}
	//else {
	//	abort();//çÏê¨íÜ
	//}




}



























