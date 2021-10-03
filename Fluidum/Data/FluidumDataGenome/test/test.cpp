#define FluidumData_GenomeDebug
#include "../FDgenome.h"
//
//FD::GenomeRead read;
FD::GenomeWrite write;
//
//
//void init() {
//	const std::string path = Fluidum::Utils::File::getFullPath(__FILE__, 3, "\\resource\\FluidumDataGenome\\genome.prop");
//
//	std::ofstream ofs(path, std::ios::binary);
//
//	if (!ofs)
//		throw std::runtime_error("");
//
//	FD::Genome::PopulationSize populationSizeMax = 100000;
//	FD::Genome::PopulationSize populationSizeMin = 1;
//
//	ofs.write(reinterpret_cast<char*>(&populationSizeMax), sizeof(FD::Genome::PopulationSize));
//	ofs.write(reinterpret_cast<char*>(&populationSizeMin), sizeof(FD::Genome::PopulationSize));
//
//
//	FD::Genome::GenomeSize genomeSizeMax = 1000000;
//	FD::Genome::GenomeSize genomeSizeMin = 1;
//
//	ofs.write(reinterpret_cast<char*>(&genomeSizeMax), sizeof(FD::Genome::GenomeSize));
//	ofs.write(reinterpret_cast<char*>(&genomeSizeMin), sizeof(FD::Genome::GenomeSize));
//
//}

struct A {
	template<typename T>
	void hoge(int a) {
		std::cout << a;
	}
};

template<typename T>
void a(const FD::Genome::Genomes<T>* data, int a) {
	std::cout << data->size() << std::endl;
	std::cout << a;
}


int main() {
	//init();

	write.initType(FD::Genome::Type::BINARY);
	using enum FD::Genome::Type;
	A b;
	write.call<FluidumData_Genome_CallFunctions(&A::hoge)>(&b,1);
}