#include "deleteAll.h"
#include "delete.h"

FS::Utils::DeleteAllScenes::DeleteAllScenes() {
	FluidumScene_Log_Constructor("Utils::DeleteAllScenes");
	this->deleteAll();
}

FS::Utils::DeleteAllScenes::~DeleteAllScenes() noexcept {
	FluidumScene_Log_Destructor_("Utils::DeleteAllScenes")
}

void FS::Utils::DeleteAllScenes::call() {
	assert(false && "Scene::callConstructor");
}

void FS::Utils::DeleteAllScenes::deleteAll() {
	FluidumScene_Log_CallSceneConstructor(
		"Utils::DeleteScenes<TextEditor,"\
		"Flu::Node,"\
		"Analysis::Overview,"\
		"Genome::Overview,"\
		"Animation,"\
		"Project,"\
		"Console>");

	Scene::callConstructor<DeleteScenes<
		TextEditor,
		Flu::Node,
		Analysis::Overview,
		Genome::Overview,
		Animation,
		Project,
		Console
		>>();
}