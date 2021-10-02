#pragma once

#include "../Common/FVKinclude.h"
//#include "FVKdrawcommand.h"
#include "../Buffer/FVKuniformbuffer.h"

namespace FVK::Internal {

	class Draw final {
	public:

		struct Parameter {

		};

	public:
		explicit Draw(ManagerPassKey, const Data::DrawData data, const Parameter& parameter);
		~Draw() = default;
		FluidumUtils_Class_Default_CopyMove(Draw)

	private:
		struct {
			vk::Device device = nullptr;
			vk::SwapchainKHR swapchain = nullptr;
			std::vector<vk::CommandBuffer> commandBuffers = {};
			vk::Queue graphicsQueue = nullptr;
			vk::Queue presentQueue = nullptr;

			std::vector<vk::Fence> inFlightFences = {};
			std::vector<vk::Fence> imagesInFlight = {};
			std::vector<vk::Semaphore> imageAvailableSemaphores = {};
			std::vector<vk::Semaphore> renderFinishedSemaphores = {};

			uint32_t currentFrame = 0;
			bool framebufferResized = false;

		}data;

	private:
		void create(const Data::DrawData data, const Parameter& parameter);
	public:
		void drawFrame( /*Commands& commands*/);
		const Data::DrawInfo& get() const noexcept;
		void destroy() const noexcept;

	private:
		Data::DrawInfo info = {};
	};
}