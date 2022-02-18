#include "scene.h"
#include "../../Scene/Utils/Scene/include.h"

void FD::Scene::Internal::Data::addSceneCallback(bool async, FU::Class::ClassCode::CodeType code) {
	std::lock_guard<std::mutex> lock(mtx);

	const auto itr = std::find(codes.crbegin(), codes.crend(), code);
	if (itr != codes.crend())
		return;

	codes.emplace_back(code);
	erase.store(false);
	save.store(true);
}

void FD::Scene::Internal::Data::deleteSceneCallback(bool async, FU::Class::ClassCode::CodeType code) {
	std::lock_guard<std::mutex> lock(mtx);
	const auto itr = std::find(codes.crbegin(), codes.crend(), code);
	assert((itr != codes.crend()) && "FluidumSceneÇÃê›åvÇ…ñ‚ëËÇ™Ç†ÇËÇ‹Ç∑ÅD");

	codes.erase(std::next(itr).base());
	erase.store(true);
	save.store(true);
}

bool FD::SceneRead::exist(const FU::Class::ClassCode::CodeType code) const noexcept {
	using namespace ::FD::Scene::Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);
	const auto itr = std::find(Data::codes.crbegin(), Data::codes.crend(), code);
	return itr != Data::codes.crend();
}

bool FD::SceneRead::running() const noexcept {
	constexpr auto code = FU::Class::ClassCode::GetClassCode<FS::Calc::Lua::Run>();
	const bool exist = this->exist<code>();
	return exist;
}
