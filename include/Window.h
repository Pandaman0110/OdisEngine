#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <vector>
#include <functional>
#include <string>

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

#include "InputEvent.h"
#include "utility/OdisMath.h"

namespace OdisEngine 
{
	enum class RenderAPI
	{
		opengl,
		vulkan
	};

	enum class NativeWindow
	{
		windows,
		cocoa,
		x11,
	};

	class Window
	{

	public:
		Window(int width, int height, std::string name, bool fullscreen_mode, bool vsync, RenderAPI);

		int should_close();

		void terminate();

		void swap_buffers();

		void poll();


		
		inline int get_monitor_width() const { return monitor_width; };

		inline int get_monitor_height() const { return monitor_height; };

		template <IntVectorType T = glm::ivec2>
		T get_monitor_size() const { return T{ get_monitor_width(), get_monitor_height() }; };



		inline int get_window_width() const { return window_width; };

		inline int get_window_height() const { return window_height; };

		template <IntVectorType T = glm::ivec2>
		T get_window_size() const { return T{ get_window_width(), get_window_height()}; };



		//callbacks
		inline void set_window_size_callback(std::function<void(int, int)> callback) { Window::window_size_callback = callback; };
		inline void set_keyboard_input_callback(std::function<void(ButtonInputEvent)> callback) { Window::keyboard_callback = callback; };
		inline void set_mouse_button_input_callback(std::function<void(ButtonInputEvent)> callback) { Window::mouse_button_callback = callback; };
		inline void set_mouse_pos_input_callback(std::function<void(double, double)> callback) { Window::mouse_pos_callback = callback; };


		GLFWwindow* get_window_handle() const { return window; };


		//misc settings

		void set_vsync(bool);

#if	_WIN32
		HWND get_win32_window();
#elif __APPLE__
		NSWindow get_cocoa_window();
#elif __linux__
		Display get_x11_window();
#endif 
	private:
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		static void keyboard_input_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouse_button_input_callback(GLFWwindow* window, int button, int action, int mods);
		static void mouse_pos_input_callback(GLFWwindow* window, double x, double y);
		static void error_callback(int error, const char* description);

		//TODO more than one callback for these
		inline static std::function<void(int, int)> window_size_callback;
		inline static std::function<void(ButtonInputEvent)> keyboard_callback;
		inline static std::function<void(ButtonInputEvent)> mouse_button_callback;
		inline static std::function<void(double, double)> mouse_pos_callback;

		int monitor_width = 0;
		int monitor_height = 0;

		int window_width = 0;
		int window_height = 0;

		GLFWwindow* window = nullptr;

		void create_window(int width, int height, std::string name, bool fullscreen_mode);

		void window_setup(RenderAPI render_api);
		void render_api_setup(RenderAPI render_api);
	};

}

#endif