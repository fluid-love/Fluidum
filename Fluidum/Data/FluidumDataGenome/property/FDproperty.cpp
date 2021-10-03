#include "FDproperty.h"

FD::Genome::Property::Property() {

	std::ifstream ifs(path,std::ios::binary);

	if (!ifs)
		throw std::runtime_error("");

	ifs.read(reinterpret_cast<char*>(&limit.populationSizeMax),sizeof(PopulationSize));
	ifs.read(reinterpret_cast<char*>(&limit.populationSizeMin),sizeof(PopulationSize));
	
	limit.populationSizeMaxW = static_cast<PopulationSize>(limit.populationSizeMax * 0.9);

	ifs.read(reinterpret_cast<char*>(&limit.genomeSizeMax),sizeof(GenomeSize));
	ifs.read(reinterpret_cast<char*>(&limit.genomeSizeMin),sizeof(GenomeSize));
	
	limit.genomeSizeMaxW = static_cast<GenomeSize>(limit.genomeSizeMax * 0.9);


}

FD::Genome::Property::~Property() noexcept {

	std::ofstream ofs(path,std::ios::binary);

	if (!ofs) {
		std::cerr << "Failed to write :" << this->path << std::endl;
		abort();
	}

	ofs.write(reinterpret_cast<char*>(&limit.populationSizeMax),sizeof(PopulationSize));
	ofs.write(reinterpret_cast<char*>(&limit.populationSizeMin),sizeof(PopulationSize));

	ofs.write(reinterpret_cast<char*>(&limit.genomeSizeMax),sizeof(GenomeSize));
	ofs.write(reinterpret_cast<char*>(&limit.genomeSizeMin),sizeof(GenomeSize));


}
