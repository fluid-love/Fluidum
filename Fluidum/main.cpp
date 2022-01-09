#include "Scene/include.h"

namespace FluidumMain {

	//Delay the call to constructor due to the order of "Data" initialization.
	std::unique_ptr<FS::MainFluidumScene> GScene{};

	void loop() {
		GScene->call();
	}

}

namespace FluidumMain {

	::FD::LogWrite GLogWrite{};

	//messenger callback
	void messengerCallback(const std::string& message) noexcept {

		try {
			GLogWrite.add(message);
		}
		catch (const FD::LogWrite::Exception val) {
			assert(val == FD::LogWrite::Exception::FailedToAdd);
			try {
				std::cerr << "Failed to log. The message is [" << message << "]." << std::endl;
			}
			catch (...) {
				;
			}
		}
		catch (...) {
			try {
				std::cerr << "[" << __FILE__ << __LINE__ << "] " << "Internal Error." << std::endl;
			}
			catch (...) {
				;
			}
		}
	}

}

namespace FluidumMain {

	void terminate() noexcept {
		try {
			//no-throw
			GLogWrite.requestStop();//The remaining logs are output to a file.

			while (true) {
				const bool joinable = GLogWrite.threadJoinable();
				if (joinable)
					break;
			}

			FD::ProjectWrite* project = FluidumMain::GScene->getData<FD::ProjectWrite>();
			project->requestStop();
			while (true) {
				const bool joinable = GLogWrite.threadJoinable();
				if (joinable)
					break;
			}

		}
		catch (...) {

		}
		std::abort();
	}

	//FU terminate
}

int main() {
	//terminate
	std::set_terminate(FluidumMain::terminate);

	FluidumMain::GScene = std::make_unique<FS::MainFluidumScene>();

	try {
		FluidumMain::GScene->setAddCallback(FD::Scene::CallBacks::addSceneCallback);
		FluidumMain::GScene->setDeleteCallback(FD::Scene::CallBacks::deleteSceneCallback);

		FluidumMain::GScene->addScene<FS::Title>();

		FD::WindowWrite* window = FluidumMain::GScene->getData<FD::WindowWrite>();

		FDR::setMessengerCallbackFunction(FluidumMain::messengerCallback);
		FD::Internal::GMessenger.setCallback(FluidumMain::messengerCallback);
		FS::GLog.setCallback(FluidumMain::messengerCallback);
		FS::Calc::setMessengerCallback(FluidumMain::messengerCallback);
		FDR::mainLoop("Fluidum", FluidumMain::loop, window->getCloseFlag());
	}
	catch (const std::exception& e) {
		std::cerr << e.what();
	}
	catch (...) {
		std::cerr << "Error" << std::endl;
	}

	FluidumMain::GScene.reset();

	FluidumMain::GLogWrite.requestStop();
}