#include "FVKmanager.h"

FVK::Internal::Manager::Manager::Manager() {
	GMessenger.add<FU::Log::Type::None>("Construct FluidumVK Manager.");
}

FVK::Internal::Manager::Manager::~Manager() {
	GMessenger.add<FU::Log::Type::None>("Destruct FluidumVK Manager.");
}

namespace FVK::Internal {
	template<FVK::Internal::FvkType Type, typename T>
	void deleteItems(T& items) {
		for (std::size_t i = 0; auto & x : items) {
			x.destroy();
			GKeyManager.erase(Type, i);
			i++;
		}
	}
}

void FVK::Internal::Manager::Manager::terminate() {
	//waitIdle コマンドバッファで使用中に削除させない
	for (auto& x : std::get<CorrespondenceAt<FvkType::LogicalDevice>()>(this->data)) {
		vk::Device dv = x.get().device;
		auto result = dv.waitIdle();
		if (result != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to waitIdle.");
		}
	}

	this->terminate_recursive();
}
