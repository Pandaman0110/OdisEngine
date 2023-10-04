#include "Window.h"

#include <bx/bx.h>
#include <GLFW/glfw3.h>

#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif

#include <GLFW/glfw3native.h>

#include "InputEvent.h"

using namespace OdisEngine;

Window::Window(int width, int height, std::string name, bool fullscreen_mode)
{
	glfwSetErrorCallback(error_callback);

	//initiliaze GLFW
	if (!glfwInit())
	{
		std::cout << "GLFW init failed" << std::endl;
		std::abort();
	}

	//tell glfw to create the window with no API spec
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	//create the window
	if (fullscreen_mode)
		window = glfwCreateWindow(width, height, name.c_str(), glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

	if (!window)
	{
		std::cout << "GLFW window creation failed" << std::endl;
		std::abort();
	}

	glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	monitor_width = mode->width;
	monitor_height = mode->height;

	glfwGetWindowSize(window, &window_width, &window_height);

}

int Window::should_close()
{
	return glfwWindowShouldClose(window);
}

void Window::terminate()
{
	glfwTerminate();
}

void Window::error_callback(int error, const char* description)
{
	std::cout << "GLFW ERROR: " << error << ": " << description << std::endl;
}

#if BX_PLATFORM_WINDOWS
HWND Window::getWin32Window()
{
	return glfwGetWin32Window(window);
}

#elif BX_PLATFORM_OSX
NSWindow Window::glfwGetCocoaWindow()
{
	return glfwGetCocoaWindow(window);
}

#elif BX_PLATFORM_LINUX
Display Window::glfwGetX11Window()
{
	return glfwGetX11Window(window);
}
#endif 
