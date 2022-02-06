#pragma once

#include "../Common/include.h"

namespace FVK::Internal {

	class Window final {
	public:
		enum class WindowMode : UT {
			Normal,
			FullScreen,
			Maximized
		};

		template<WindowMode>
		struct Parameter {
		private:
			FluidumUtils_Class_Delete_ConDestructor(Parameter);
		};

		template<>
		struct Parameter<WindowMode::Normal> {
			const char* title = "";
			I32 width = 0;
			I32 height = 0;
			I32 posX = 0;
			I32 posY = 0;
		};

		template<>
		struct Parameter<WindowMode::FullScreen> {
			const char* title = "";
		};

		template<>
		struct Parameter<WindowMode::Maximized> {
			const char* title = "";
		};

		using NormalParameter = Parameter<WindowMode::Normal>;
		using FullScreenParameter = Parameter<WindowMode::FullScreen>;
		using MaximizedParameter = Parameter<WindowMode::Maximized>;

	public:
		/*
		Exception:
			FailedToCreate
		*/
		//strong
		template<typename T>
		explicit Window(ManagerPassKey, const Data::WindowData& data, const T& parameter) {
			this->create(data, parameter);
		}

		~Window() noexcept = default;
		FluidumUtils_Class_Default_CopyMove(Window);

	public:
		[[nodiscard]] const Data::WindowInfo& get() const noexcept;

	public:
		//no-throw
		void destroy() noexcept;

	public:
		[[nodiscard]] std::pair<I32, I32> windowSize() const noexcept;

		void resizeWindow(const IF32 x, const IF32 y, const IF32 width, const IF32 height) const noexcept;


		/*
		Returns the height of the window minus the height of the taskbar.

		Exception: strong
			Unexpected : Native error

		Return:
			{ width, height }
		*/
		static [[nodiscard]] std::pair<IF32, IF32> fullscreenSize();

		/*
		Exception: strong
			Unexpected : Native error

		Return:
			{ posX, posY }
		*/
		static [[nodiscard]] std::pair<IF32, IF32> fullscreenPos();

		/*
		Exception: strong
			Unexpected : Native error
		*/
		void fullscreen() const;

	private:

		//Create window by specifying the position and size.
		void create(const Data::WindowData& data, const NormalParameter& parameter);

		//Fullscreen(taskbar)								
		void create(const Data::WindowData& data, const FullScreenParameter& parameter);

		//GLFW_MAXIMIZED									
		void create(const Data::WindowData& data, const MaximizedParameter& parameter);

	private:
		/*
		Exception:
			FailedToCreate
		*/
		//strong
		void checkGlfwCreateWindow() const;

		/*
		Exception:
			FailedToCreate
		*/
		//basic
		void setCallbacks() noexcept;

		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
		static void setPosCallback(GLFWwindow* window, int posX, int posY);
		static void focusedCallback(GLFWwindow* window, int focus);

		static void sizeLimitsCallback(
			GLFWwindow* window,
			std::optional<int> minwidth,
			std::optional<int> minheight,
			std::optional<int> maxwidth,
			std::optional<int> maxheight
		);

		/*
		Exception:
			FailedToCreate
		*/
		//strong
		const GLFWvidmode* getVideoMode() const;

	private:
		Data::WindowInfo info{};

	};

}

//with callback
void glfwSetWindowSizeLimits_(GLFWwindow* window, int minwidth, int minheight, int maxwidth, int maxheight);