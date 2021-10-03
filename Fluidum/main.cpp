#include "Scene/include.h"

std::unique_ptr<FS::MainFluidumScene> GScene = nullptr;

void loop() {
	//ImGui::Begin("");
	//
	//ImGui::End();
	//GScene->call();
}

void callback(const std::string& m) {
	std::cout << m << std::endl;
}

int main() {

	GScene = std::make_unique<FS::MainFluidumScene>();
	try
	{
		GScene->setAddCallback(FD::Scene::CallBacks::addSceneCallback);
		GScene->setDeleteCallback(FD::Scene::CallBacks::deleteSceneCallback);

		GScene->addScene<FS::Title>();

		FD::WindowWrite* window = GScene->getData<FD::WindowWrite>();

		FDR::setMessengerCallbackFunction(callback);
		FDR::mainLoop("Fluidum", loop, window->getCloseFlag());
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what();
	}
}