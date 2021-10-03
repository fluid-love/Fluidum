#include "draw.h"

FVK::Internal::Draw::Draw(ManagerPassKey, const Data::DrawData data, const Parameter& parameter) {
	this->create(data, parameter);
}

const FVK::Internal::Data::DrawInfo& FVK::Internal::Draw::get() const noexcept {
	return this->info;
}

void FVK::Internal::Draw::create(const Data::DrawData data, const Parameter& parameter) {
	//this->data.inFlightFences = data.get<>.inFlightFences;
	//this->data.commandBuffers = dData.commandBuffers;
	//this->data.device = dData.device;
	//this->data.graphicsQueue = dData.logicalDeviceSavedParameter->graphicsQueue;
	//this->data.presentQueue = dData.logicalDeviceSavedParameter->presentQueue;
	//this->data.imageAvailableSemaphores = dData.imageAvailableSemaphores;
	//this->data.renderFinishedSemaphores = dData.renderFinishedSemaphores;
	//this->data.imagesInFlight.resize(dData.swapchainSavedParameter->imageCount);

	//data.swapchain = dData.swapchain;

}

void FVK::Internal::Draw::drawFrame() {

	auto result = data.device.waitForFences(1, &data.inFlightFences[data.currentFrame], true, UINT64_MAX);
	if (result != vk::Result::eSuccess)
		throw std::runtime_error("Failed to wait for Fence");

	uint32_t imageIndex = 0;

	result = data.device.acquireNextImageKHR(data.swapchain, UINT64_MAX, data.imageAvailableSemaphores[data.currentFrame], nullptr, &imageIndex);

	if (result == vk::Result::eErrorOutOfDateKHR) {
		//recreateswapchain
		return;
	}
	else if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR) {
		throw std::runtime_error("Failed to acquire next image");
	}

	if (data.imagesInFlight[imageIndex]) {
		result = data.device.waitForFences(1, &data.imagesInFlight[imageIndex], true, UINT64_MAX);
		if (result != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to wat fence");
		}
	}

	data.imagesInFlight[imageIndex] = data.inFlightFences[data.currentFrame];


	vk::Semaphore waitSemaphores[1] = { data.imageAvailableSemaphores[data.currentFrame] };
	vk::Semaphore signalSemaphores[] = { data.renderFinishedSemaphores[data.currentFrame] };
	vk::PipelineStageFlags waitStages[1] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };


	vk::SubmitInfo submitInfo = {
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = waitSemaphores,
		.pWaitDstStageMask = waitStages,
		.commandBufferCount = 1,
		.pCommandBuffers = &data.commandBuffers[imageIndex],
		.signalSemaphoreCount = 1,
		.pSignalSemaphores = signalSemaphores,
	};

	if (result == vk::Result::eSuboptimalKHR)
		submitInfo.signalSemaphoreCount = 0;


	result = data.device.resetFences(1, &data.inFlightFences[data.currentFrame]);
	if (result != vk::Result::eSuccess)
		throw std::runtime_error("Failed to reset Fence");


	//fenceが終わったタイミングでコマンドブッファを作り直す
	result = data.commandBuffers[imageIndex].reset(vk::CommandBufferResetFlagBits::eReleaseResources);
	//commands.call();

	LockGuard lock(GMutex);//for:queue multi thread error
	result = data.graphicsQueue.submit(1, &submitInfo, data.inFlightFences[data.currentFrame]);

	if (result != vk::Result::eSuccess)
		throw std::runtime_error("Failed to submit Queue");



	vk::SwapchainKHR swapChainsVK[1] = { data.swapchain };
	vk::PresentInfoKHR presentInfo = {
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = signalSemaphores,
		.swapchainCount = 1,
		.pSwapchains = swapChainsVK,
		.pImageIndices = &imageIndex,
	};


	if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || data.framebufferResized) {
		//data.framebufferResized = false;
		//recreateswapchain
		return;
	}
	else if (result != vk::Result::eSuccess) {
		throw std::runtime_error("Failed to submit Queue");
	}
	else {
		result = data.presentQueue.presentKHR(presentInfo);
	}

	data.currentFrame = (data.currentFrame + 1) % data.inFlightFences.size();
}

void FVK::Internal::Draw::destroy() const noexcept {

}
