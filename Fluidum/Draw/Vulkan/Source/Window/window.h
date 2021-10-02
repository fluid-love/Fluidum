#pragma once

#include "../Common/FVKinclude.h"

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
			Parameter() = default;
			~Parameter() = default;
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
		template<typename T>
		explicit Window(ManagerPassKey, const Data::WindowData& data, const T& parameter) {
			this->create(data, parameter);
		}

		~Window() = default;
		FluidumUtils_Class_Default_CopyMove(Window)

	public:
		_NODISCARD const Data::WindowInfo& get() const noexcept;

		void destroy();

	private:

		//位置・サイズを指定して通常作成
		void create(const Data::WindowData& data, const NormalParameter& parameter);

		//フルスクリーン									
		void create(const Data::WindowData& data, const FullScreenParameter& parameter);

		//GLFW_MAXIMIZED									
		void create(const Data::WindowData& data, const MaximizedParameter& parameter);

	private:
		void checkGlfwCreateWindow() const;
		void setGlfw();

		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

	public:
		_NODISCARD std::pair<int32_t, int32_t> getWindowSize() const;

	private:
		Data::WindowInfo info = {};

	};



}