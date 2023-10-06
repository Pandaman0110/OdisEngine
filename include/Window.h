#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <vector>
#include <functional>

#include <glad/gl.h>

#include <GLFW/glfw3.h>

#if __linux__
#define GLFW_EXPOSE_NATIVE_X11
#elif _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#elif __APPLE__
#define GLFW_EXPOSE_NATIVE_COCOA
#endif

#include <GLFW/glfw3native.h>

static void glfwErrorCallback(int error, const char* description)
{
	std::cout << "GLFW ERROR: " << error << ": " << description << std::endl;
}

namespace OdisEngine 
{
	enum class RenderAPI
	{
		OpenGL,
		Vulkan
	};

	enum class NativeWindow
	{
		WINDOWS,
		COCOA,
		X11,
	};

	class Window
	{

	public:
		Window(int width, int height, std::string name, bool fullscreen_mode, RenderAPI);

		int should_close();
		void terminate();
		void swap_buffers();

		inline int get_monitor_width() const { return monitor_width; };
		inline int get_monitor_height() const { return monitor_height; };

		inline int get_window_width() const { return window_width; };
		inline int get_window_height() const { return window_height; };

		GLFWwindow* get_window_handle() const { return window; };

		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		void set_window_size_callback(std::function<void(int, int)> window_size_callback);

#if	_WIN32
		HWND get_win32_window();
#elif __APPLE__
		NSWindow get_cocoa_window();
#elif __linux__
		Display get_x11_window();
#endif 
	private:
		static void error_callback(int error, const char* description);

		static std::function<void(int, int)> window_size_callback;

		int monitor_width;
		int monitor_height;

		int window_width;
		int window_height;

		GLFWwindow* window;

		void create_window(int width, int height, std::string name, bool fullscreen_mode);

		void window_setup(RenderAPI render_api);
		void render_api_setup(RenderAPI render_api);
	};

}

#endif