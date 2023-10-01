#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>

#include <GLFW/glfw3.h>

static void glfwErrorCallback(int error, const char* description)
{
	std::cout << "GLFW ERROR: " << error << ": " << description << std::endl;
}

namespace OdisEngine 
{
	class Window
	{
		friend class Renderer;
	public:
		Window(int width, int height, std::string name, bool fullscreen_mode);

		int getMonitorWidth() const { return window_width; };
		int getMonitorHeight() const { return window_height; };


		int shouldClose();
		void terminate();

	private:
		int monitor_width;
		int monitor_height;

		int window_width;
		int window_height;

		GLFWwindow* native_window;
	};

}

#endif