#include "add.h"

#include "classcodes.h"

#include "../../Analysis/Overview/overview.h"

#include "../../Coding/Tab/tab.h"
#include "../../Coding/TextEditor/texteditor.h"

FS::Utils::AddScenes::AddScenes(
	const std::vector<FU::Class::ClassCode::CodeType>& codes
) : codes(codes)
{
	FluidumScene_Log_Constructor(::FS::Utils::AddScenes);
}

FS::Utils::AddScenes::~AddScenes() noexcept {
	FluidumScene_Log_Destructor(::FS::Utils::AddScenes);
}

void FS::Utils::AddScenes::call() {
	using namespace FU::Class;

	for (const auto x : this->codes) {

		if (x == ClassCode::GetClassCode<::FS::Analysis::Overview>()) {
			FluidumScene_Log_RequestAddScene(::FS::Analysis::Overview);
			Scene::addScene<::FS::Analysis::Overview>();
		}
		else if (x == ClassCode::GetClassCode<::FS::TextEditor>()) {
			FluidumScene_Log_RequestAddScene(::FS::TextEditor);
			Scene::addScene<::FS::TextEditor>();
		}
		else if (x == ClassCode::GetClassCode<::FS::Coding::Tab>()) {
			FluidumScene_Log_RequestAddScene(::FS::Coding::Tab);
			Scene::addScene<::FS::Coding::Tab>();
		}
		else {
			FluidumScene_Log_InternalWarning();
		}

	}

	FluidumScene_Log_RequestDeleteScene(::FS::Utils::AddScenes);
	Scene::deleteScene<AddScenes>();
}
