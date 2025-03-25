#include "manager.h"

FVK::Internal::Manager::Manager::Manager() noexcept {
	//no-throw
	GMessenger.add<FU::Log::Type::None>(__FILE__, __LINE__, "Construct FluidumVK Manager.");
}

FVK::Internal::Manager::Manager::~Manager() noexcept {
	GMessenger.add<FU::Log::Type::None>(__FILE__, __LINE__, "Destruct FluidumVK Manager.");
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
	//Don't delete when using commandbuffer
	for (auto& x : std::get<CorrespondenceAt<FvkType::LogicalDevice>()>(this->data)) {
		vk::Device dv = x->get().device;
		auto result = dv.waitIdle();
		if (result != vk::Result::eSuccess) {
			Exception::throwFailedToDestroy();
		}
	}

	try {
		this->terminate_recursive();
	}
	catch (...) {
		std::terminate();
	}

	if (GKeyManager.empty())
		return;

	//not empty
	GMessenger.add<FU::Log::Type::Warning>(__FILE__, __LINE__, "All items have been destroyed, but keys(size = {}) are still in the GKeyManager.", GKeyManager.size());

	//no-throw
	GKeyManager.clear();
}
