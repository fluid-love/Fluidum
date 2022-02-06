#include "mainloop.h"

namespace FDR::Internal {
	void loop(auto& commands, auto function) {
		using namespace FDR::Internal;

		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();

		{
			LockGuard lock(GMutex);
			function();
		}
		

		ImGui::Render();
		commands->call();
		glfwPollEvents();

	};

	void drawFrame(auto& data, void* null) {
		static std::vector<vk::Fence> imagesInFlight{};
		static I32 currentFrame = 0;

		vk::Device device = std::get<0>(data);
		vk::SwapchainKHR swapchain = std::get<1>(data);

		std::vector<vk::Fence> inFlightFences = std::get<2>(data);
		std::vector<vk::Semaphore> imageAvailableSemaphores = std::get<3>(data);
		std::vector<vk::CommandBuffer> commandBuffers = std::get<4>(data);
		std::vector<vk::Semaphore> renderFinishedSemaphores = std::get<5>(data);
		vk::Queue graphicsQueue = std::get<6>(data);
		vk::Queue presentQueue = std::get<7>(data);

		imagesInFlight.resize(commandBuffers.size());
		auto result = device.waitForFences(1, &inFlightFences[currentFrame], true, UINT64_MAX);
		if (result != vk::Result::eSuccess)
			throw std::runtime_error("Failed to wait for Fence");

		UI32 imageIndex;

		result = device.acquireNextImageKHR(swapchain, UINT64_MAX, imageAvailableSemaphores[currentFrame], nullptr, &imageIndex);

		if (result == vk::Result::eErrorOutOfDateKHR) {
			//recreateswapchain
			return;
		}
		else if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR) {
			throw std::runtime_error("Failed to acquire next image");
		}

		if (imagesInFlight[imageIndex]) {
			result = device.waitForFences(1, &imagesInFlight[imageIndex], true, UINT64_MAX);
			if (result != vk::Result::eSuccess) {
				throw std::runtime_error("Failed to wat fence");
			}
		}

		imagesInFlight[imageIndex] = inFlightFences[currentFrame];


		vk::Semaphore waitSemaphores[1] = { imageAvailableSemaphores[currentFrame] };
		vk::Semaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
		vk::PipelineStageFlags waitStages[1] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };


		vk::SubmitInfo submitInfo = {
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = waitSemaphores,
			.pWaitDstStageMask = waitStages,
			.commandBufferCount = 1,
			.pCommandBuffers = &commandBuffers[imageIndex],
			.signalSemaphoreCount = 1,
			.pSignalSemaphores = signalSemaphores,
		};

		if (result == vk::Result::eSuboptimalKHR)
			submitInfo.signalSemaphoreCount = 0;


		result = device.resetFences(1, &inFlightFences[currentFrame]);
		if (result != vk::Result::eSuccess)
			throw std::runtime_error("Failed to reset Fence");


		//Recreate the command buffers at the end of the fence.
		result = commandBuffers[imageIndex].reset(vk::CommandBufferResetFlagBits::eReleaseResources);
		FVK::exeCommands(MainCommands);


		//LockGuard lock(GMutex);//for:queue multi thread error
		result = graphicsQueue.submit(1, &submitInfo, inFlightFences[currentFrame]);

		if (result != vk::Result::eSuccess)
			throw std::runtime_error("Failed to submit Queue");


		vk::SwapchainKHR swapChainsVK[1] = { swapchain };
		vk::PresentInfoKHR presentInfo = {
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = signalSemaphores,
			.swapchainCount = 1,
			.pSwapchains = swapChainsVK,
			.pImageIndices = &imageIndex,
		};

		if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR/* || frameBufferResize*/) {
			//data.framebufferResized = false;
			//recreateswapchain
			return;
		}
		else if (result != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to submit Queue");
		}
		else {
			result = presentQueue.presentKHR(presentInfo);
			//Stacked commands
			Command::Internal::call();
		}

		currentFrame = (currentFrame + 1) % inFlightFences.size();
	}

}

void FDR::Internal::mainLoop(void(*function)(), const bool* const flag) {
	auto draw = FVK::makeAnyCommand(
		BaseDeviceKey, BaseSwapchainKey,
		BaseInFlightFenceKeys,
		BaseImageAvailableSemaphoreKeys,
		BaseCommandBuffersKey,
		BaseRenderFinishedSemaphoreKeys,
		BaseGraphicsQueueKey,
		BasePresentQueueKey
	);

	draw->setFunction(drawFrame);

	while (!(*flag)) {
		loop(draw, function);
	}
}
