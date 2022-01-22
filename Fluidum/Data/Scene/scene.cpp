#include "scene.h"
#include "../../Scene/Utils/Scene/include.h"

void FD::Scene::Internal::Data::addSceneCallback(bool async, FU::Class::ClassCode::CodeType code) {
	std::lock_guard<std::mutex> lock(mtx);

	const auto itr = std::find(codes.cbegin(), codes.cend(), code);
	if (itr != codes.cend())
		return;

	codes.emplace_back(code);
	save.store(true);
}

void FD::Scene::Internal::Data::deleteSceneCallback(bool async, FU::Class::ClassCode::CodeType code) {
	std::lock_guard<std::mutex> lock(mtx);
	const auto itr = std::find(codes.cbegin(), codes.cend(), code);
	assert((itr != codes.cend()) && "FluidumSceneÇÃê›åvÇ…ñ‚ëËÇ™Ç†ÇËÇ‹Ç∑ÅD");

	codes.erase(itr);
	save.store(true);
}

bool FD::SceneRead::exist(const FU::Class::ClassCode::CodeType code) const noexcept {
	using namespace ::FD::Scene::Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);
	const auto itr = std::find(Data::codes.cbegin(), Data::codes.cend(), code);
	return itr != Data::codes.cend();
}

bool FD::SceneRead::running() const noexcept {
	constexpr auto code = FU::Class::ClassCode::GetClassCode<FS::Calc::Lua::Run>();
	const bool exist = this->exist<code>();
	return exist;
}
