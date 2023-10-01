#include "Window.h"


#include <bx/bx.h>

#include <GLFW/glfw3.h>

using namespace OdisEngine;

Window::Window(int width, int height, std::string name, bool fullscreen_mode)
{
	glfwSetErrorCallback(glfwErrorCallback);

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
		native_window = glfwCreateWindow(width, height, name.c_str(), glfwGetPrimaryMonitor(), nullptr);
	else
		native_window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

	if (!native_window)
	{
		std::cout << "GLFW window creation failed" << std::endl;
		std::abort();
	}

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	monitor_width = mode->width;
	monitor_height = mode->height;

	glfwGetWindowSize(native_window, &window_width, &window_height);
}

int Window::shouldClose()
{
	return glfwWindowShouldClose(native_window);
}

void Window::terminate()
{
	glfwTerminate();
}