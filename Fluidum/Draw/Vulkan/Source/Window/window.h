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
			FluidumUtils_Class_Delete_ConDestructor(Parameter)
		};

		template<>
		struct Parameter<WindowMode::Normal> {
			const char* title = "";
			int32_t width = 0;
			int32_t height = 0;
			int32_t posX = 0;
			int32_t posY = 0;
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
		FluidumUtils_Class_Default_CopyMove(Window)

	public:
		[[nodiscard]] const Data::WindowInfo& get() const noexcept;

	public:
		//no-throw
		void destroy() noexcept;

	public:
		[[nodiscard]] std::pair<I32, I32> windowSize() const noexcept;

	private:

		//Create window by specifying the position and size.
		void create(const Data::WindowData& data, const NormalParameter& parameter);

		//Fullscreen									
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
		void setResizedCallback();

		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

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