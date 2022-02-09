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
			const char* iconFilePath = nullptr;
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
		[[nodiscard]] std::pair<IF32, IF32> getPos() const;
		[[nodiscard]] std::pair<IF32, IF32> getSize() const;

		void setPos(const IF32 x, const IF32 y) const;
		void setSize(const IF32 width, const IF32 height) const;

		void resize(const IF32 x, const IF32 y, const IF32 width, const IF32 height) const;

	public:
		void setWindowSizeLimit();

	public:
		void minimize() const;


	public:


		/*
		Returns the height of the window minus the height of the taskbar.

		Exception: strong
			Unexpected : Native error

		Return:
			{ posX, posY, width, height }
		*/
		static [[nodiscard]] std::tuple<IF32, IF32, IF32, IF32> fullscreenPosSize();

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

		void setTransparent(const I32 r, const I32 g, const I32 b, const I32 alpha) const;
		void unsetTransparent() const;

	private:

		//Create window by specifying the position and size.
		void create(const Data::WindowData& data, const NormalParameter& parameter);

		//Fullscreen(taskbar)								
		void create(const Data::WindowData& data, const FullScreenParameter& parameter);

		void create(const Data::WindowData& data, const MaximizedParameter& parameter);

	private:
		void checkWindow();

		/*
		Exception:
			FailedToCreate
		*/
		//basic
		void setCallbacks() noexcept;

		//static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
		//static void setPosCallback(GLFWwindow* window, int posX, int posY);
		//static void focusedCallback(GLFWwindow* window, int focus);

	private://proc
#ifdef FluidumUtils_Type_OS_Windows
		static LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
#endif

	private:
		Data::WindowInfo info{};

	};

}
