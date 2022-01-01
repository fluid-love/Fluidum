#include "Scene/include.h"

//Delay the call to constructor due to the order of "Data" initialization.
namespace FluidumMain {
	std::unique_ptr<FS::MainFluidumScene> GScene = nullptr;

	void loop() {
		GScene->call();
	}

	void callback(const std::string& m) {
		std::cout << m << std::endl;
	}

}

int main() {
	FluidumMain::GScene = std::make_unique<FS::MainFluidumScene>();

	try {
		FluidumMain::GScene->setAddCallback(FD::Scene::CallBacks::addSceneCallback);
		FluidumMain::GScene->setDeleteCallback(FD::Scene::CallBacks::deleteSceneCallback);

		FluidumMain::GScene->addScene<FS::Title>();

		FD::WindowWrite* window = FluidumMain::GScene->getData<FD::WindowWrite>();

		FDR::setMessengerCallbackFunction(FluidumMain::callback);
		FDR::mainLoop("Fluidum", FluidumMain::loop, window->getCloseFlag());
	}
	catch (const std::exception& e) {
		std::cerr << e.what();
	}
	catch (...) {
		std::cerr << "Error" << std::endl;
	}
	FluidumMain::GScene.reset();
}