#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <vector>
#include <functional>

#include <GLFW/glfw3.h>
#include <bx/bx.h>

#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif

#include <GLFW/glfw3native.h>

static void glfwErrorCallback(int error, const char* description)
{
	std::cout << "GLFW ERROR: " << error << ": " << description << std::endl;
}

namespace OdisEngine 
{
	enum class NativeWindow
	{
		WINDOWS,
		COCOA,
		X11,
	};

	class Window
	{

	public:
		Window(int width, int height, std::string name, bool fullscreen_mode);

		int should_close();
		void terminate();

		inline int get_monitor_width() const { return monitor_width; };
		inline int get_monitor_height() const { return monitor_height; };

		inline int get_window_width() const { return window_width; };
		inline int get_window_height() const { return window_height; };

		GLFWwindow* get_window_handle() const { return window; };

#if BX_PLATFORM_WINDOWS
		HWND getWin32Window();
#elif BX_PLATFORM_OSX
		NSWindow glfwGetCocoaWindow();
#elif BX_PLATFORM_LINUX
		Display glfwGetX11Window();
#endif 
	private:
		static void error_callback(int error, const char* description);

		int monitor_width;
		int monitor_height;

		int window_width;
		int window_height;

		GLFWwindow* window;
	};

}

#endif