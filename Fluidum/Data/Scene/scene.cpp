#include "scene.h"

void FD::Internal::Scene::Data::addSceneCallback(bool async, FU::Class::ClassCode::CodeType code) {
	std::lock_guard<std::mutex> lock(mtx);

	const auto itr = std::find(codes.cbegin(), codes.cend(), code);
	if (itr != codes.cend())
		return;

	codes.emplace_back(code);
	save.store(true);
}

void FD::Internal::Scene::Data::deleteSceneCallback(bool async, FU::Class::ClassCode::CodeType code) {
	std::lock_guard<std::mutex> lock(mtx);
	const auto itr = std::find(codes.cbegin(), codes.cend(), code);
	assert((itr != codes.cend()) && "FluidumSceneÇÃê›åvÇ…ñ‚ëËÇ™Ç†ÇËÇ‹Ç∑ÅD");

	codes.erase(itr);
	save.store(true);
}

bool FD::SceneRead::exist(const FU::Class::ClassCode::CodeType code) const noexcept {
	using namespace Internal::Scene;
	std::lock_guard<std::mutex> lock(Data::mtx);
	const auto itr = std::find(Data::codes.cbegin(), Data::codes.cend(), code);
	return itr != Data::codes.cend();
}
