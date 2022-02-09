#include "file.h"

bool FU::File::empty(std::ifstream& ifs) {
	return (ifs.peek() == std::ifstream::traits_type::eof());
}