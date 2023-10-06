#include "Window.h"

#include <cassert>

using namespace OdisEngine;

std::function<void(int, int)> Window::window_size_callback;

void Window::error_callback(int error, const char* description)
{
	std::cout << "GLFW ERROR: " << error << ": " << description << std::endl;
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	window_size_callback(width, height);
}

Window::Window(int width, int height, std::string name, bool fullscreen_mode, RenderAPI render_api)
{
	//initiliaze GLFW
	if (!glfwInit())
	{
		std::cout << "GLFW init failed" << std::endl;
		std::abort();
	}

	//window hinting, happens before window creation
	this->window_setup(RenderAPI::OpenGL);

	//actually creates the window
	this->create_window(width, height, name, fullscreen_mode);

	//sets up window related stuff for the choosen render api
	this->render_api_setup(RenderAPI::OpenGL);




	glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	monitor_width = mode->width;
	monitor_height = mode->height;

	glfwGetWindowSize(window, &window_width, &window_height);

	glfwSetErrorCallback(error_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void Window::window_setup(RenderAPI render_api)
{
	switch (render_api)
	{
	case RenderAPI::OpenGL:
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		//Core profile so we dont have deprecated features
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		break;
	case RenderAPI::Vulkan:
		break;
	default:
		std::cout << "No render api choosen" << std::endl;
		std::abort();
		break;
	}
}

void Window::create_window(int width, int height, std::string name, bool fullscreen_mode)
{
	//create the window
	if (fullscreen_mode)
		window = glfwCreateWindow(width, height, name.c_str(), glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

	assert(window != NULL && "Failed to create GLFW window ABORT ABORT\n");
}

void Window::render_api_setup(RenderAPI render_api)
{
	int version;

	switch (render_api)
	{
	case RenderAPI::OpenGL:

		glfwMakeContextCurrent(window);

		version = gladLoadGL(glfwGetProcAddress);
		if (version == 0)
		{
			std::cout << "Failed to initialize OpenGL context ABORT ABORT" << std::endl;
			std::abort();
		}

		std::cout << "Loaded OpenGL version: " << GLAD_VERSION_MAJOR(version) << "." << GLAD_VERSION_MINOR(version) << std::endl;

		break;
	case RenderAPI::Vulkan:


		break;
	default:
		std::cout << "No render api choosen" << std::endl;
		std::abort();
		break;
	}
}

int Window::should_close()
{
	return glfwWindowShouldClose(window);
}

void Window::terminate()
{
	glfwTerminate();
}

void Window::swap_buffers()
{
	glfwSwapBuffers(window);
}

void Window::set_window_size_callback(std::function<void(int, int)> window_size_callback)
{
	Window::window_size_callback = window_size_callback;
}


#if _WIN32
HWND Window::get_win32_window()
{
	return glfwGetWin32Window(window);
}

#elif __APPLE__
NSWindow Window::get_cocoa_window()
{
	return glfwGetCocoaWindow(window);
}

#elif __linux__
Display Window::get_x11window()
{
	return glfwGetX11Window(window);
}
#endif 
