#include "Window.h"

#include <cassert>

using namespace OdisEngine;

std::function<void(int, int)> Window::window_size_callback;
std::function<void(KeyboardInputEvent)> Window::keyboard_callback;
std::function<void(MouseButtonInputEvent)> Window::mouse_button_callback;

void Window::error_callback(int error, const char* description)
{
	std::cout << "GLFW ERROR: " << error << ": " << description << std::endl;
}

//the new width and height of the framebuffer
void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	window_size_callback(width, height);
}

void Window::keyboard_input_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != 2)
	{
		Key key_num = static_cast<Key>(key);
		KeyScancode key_scancode = static_cast<KeyScancode>(scancode);
		std::string key_name { (glfwGetKeyName(key, scancode) != NULL) ? glfwGetKeyName(key, scancode) : "key_unknown" };
		bool pressed = action;
		
		keyboard_callback(KeyboardInputEvent(key_num, key_scancode, pressed, std::move(key_name)));
	}
}

void Window::mouse_button_input_callback(GLFWwindow* window, int button_num, int action, int mods)
{
	if (action != 2)
	{
		MouseButton button = static_cast<MouseButton>(button_num);
		bool pressed = action;

		mouse_button_callback(MouseButtonInputEvent(button, pressed));
	}
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
	this->window_setup(render_api);

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
	glfwSetKeyCallback(window, keyboard_input_callback);
	glfwSetMouseButtonCallback(window, mouse_button_input_callback);
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
void Window::poll()
{
	glfwPollEvents();
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
