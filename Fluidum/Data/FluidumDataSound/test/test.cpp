#define FluidumData_Sound_Debug
#include "../FDsound.h"


int main() {

	FD::Piano::Pair<FD::Genome::OneByteType> bind;
	bind.setBindTemplate<decltype(bind)::TemplateType::All>();

	using enum FD::Piano::Pitch;
	FD::PianoSound piano;
	CppGA::Genome<uint8_t> genome;



	genome.emplace_back(125);



	genome.emplace_back(126);



	FD::Piano::Play play(&piano);
	play.play(&bind, &genome);

	std::this_thread::sleep_for(std::chrono::seconds(2));

	FD::Piano::SaveParameter param;
	param.fileName = "test";

	FD::Piano::Save save(param, &piano, &bind, &genome);

	std::cout << "end";

}