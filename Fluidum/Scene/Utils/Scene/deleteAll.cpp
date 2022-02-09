#include "deleteAll.h"
#include "delete.h"

FS::Utils::DeleteAllScenes::DeleteAllScenes() {
	FluidumScene_Log_Constructor(::FS::Utils::DeleteAllScenes);
	this->deleteAll();
}

FS::Utils::DeleteAllScenes::~DeleteAllScenes() noexcept {
	FluidumScene_Log_Destructor(::FS::Utils::DeleteAllScenes);
}

void FS::Utils::DeleteAllScenes::call() {
	assert(false && "Scene::callConstructor");
}

void FS::Utils::DeleteAllScenes::deleteAll() {
	GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "[Request] Call constructor ::FS::Utils::DeleteScenes<::FS::TextEditor,::FS::Flu::Node,::FS::Analysis::Overview,::FS::Genome::Overview,::FS::Animation,::FS::Project::Explorer,::FS::Console>.");

	Scene::callConstructor<DeleteScenes<
		TextEditor,
		Flu::Node,
		Analysis::Overview,
		Genome::Overview,
		Animation,
		Project::Explorer,
		Console
		>>();
}