#include "Window.h"

#include <cassert>

#include "Log.h"

using namespace OdisEngine;

void Window::error_callback(int error, const char* description)
{
	logger->get("OdisEngine")->log(LogLevel::fatal, "GLFW ERROR", error, description);
	//std::cout << "GLFW ERROR: " << error << ": " << description << std::endl;
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
		
		keyboard_callback(ButtonInputEvent{ key_num, key_scancode, pressed, key_name });
	}
}

//mouse buttons dont have a scan code
void Window::mouse_button_input_callback(GLFWwindow* window, int key, int action, int mods)
{
	if (action != 2)
	{
		Key key_num = static_cast<Key>(key);
		KeyScancode key_scancode = static_cast<KeyScancode>(0);
		std::string key_name{ (glfwGetKeyName(key, 0) != NULL) ? glfwGetKeyName(key, 0) : "key_unknown" };
		bool pressed = action;

		mouse_button_callback(ButtonInputEvent{ key_num, key_scancode, pressed, key_name });
	}
}

void Window::mouse_pos_input_callback(GLFWwindow* window, double x, double y)
{
	mouse_pos_callback(x, y);
}

Window::Window(int width, int height, std::string name, bool fullscreen_mode, bool vsync, RenderAPI render_api)
{
	auto c = logger->create("OdisEngine");

	//initiliaze GLFW
	if (!glfwInit())
	{
		c->log(LogLevel::fatal, "GLFW init failed");
		std::abort();
	}

	//window hinting, happens before window creation
	this->window_setup(render_api);

	//actually creates the window
	this->create_window(width, height, name, fullscreen_mode);

	//sets up window related stuff for the choosen render api
	this->render_api_setup(RenderAPI::opengl);

	//vsync mode
	this->set_vsync(vsync);

	glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	monitor_width = mode->width;
	monitor_height = mode->height;

	glfwGetWindowSize(window, &window_width, &window_height);

	glfwSetErrorCallback(error_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, keyboard_input_callback);
	glfwSetMouseButtonCallback(window, mouse_button_input_callback);
	glfwSetCursorPosCallback(window, mouse_pos_input_callback);
}

void Window::window_setup(RenderAPI render_api)
{
	switch (render_api)
	{
	case RenderAPI::opengl:
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		//Core profile so we dont have deprecated features
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		break;
	case RenderAPI::vulkan:
		break;
	default:
	#ifdef _DEBUG
		logger->get("OdisEngine")->log(LogLevel::fatal, "GLFW init failed");
		std::abort();
	#endif
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
	auto c = logger->get("OdisEngine");
	int version;
	std::string ver;

	switch (render_api)
	{
	case RenderAPI::opengl:

		glfwMakeContextCurrent(window);

		version = gladLoadGL(glfwGetProcAddress);
		ver = std::to_string(GLAD_VERSION_MAJOR(version)) + "." + std::to_string(GLAD_VERSION_MINOR(version));

		if (version == 0)
		{
			c->log(LogLevel::fatal, "Failed to initialize OpenGL context");

			std::cout << "Failed to initialize OpenGL context ABORT ABORT" << std::endl;
			std::abort();
		}

		c->log(LogLevel::info, "Loaded OpenGL version", ver);
		//std::cout << "Loaded OpenGL version: " << GLAD_VERSION_MAJOR(version) << "." << GLAD_VERSION_MINOR(version) << std::endl;

		break;
	case RenderAPI::vulkan:


		break;
	default:
		c->log(LogLevel::fatal, "No Render Api choosen");

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

void Window::set_vsync(bool value)
{
	//1 sets vsync on, 0 sets it off
	glfwSwapInterval(static_cast<int>(value));
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
