#ifndef INPUT_H
#define INPUT_H

#include <vector>
#include <string>
#include <map>
#include <set>

#include <GLFW/glfw3.h>

#include "Window.h"
#include "Keys.h"
#include "InputEvent.h"

namespace OdisEngine
{

	class InputMap
	{
	public:
		InputMap();

		void add_event(std::string action, int event);
		void erase_event(std::string action, int event);
		void remove_events(std::string action);

		void add_action(std::string action);
		void erase_action(std::string action);

		void load_actions_from_config ();

		std::vector<std::string> get_actions() const;

	private:
		//std::multimap<std::string, >

	};

	typedef std::list<InputEvent> InputQueue;

	class Input
	{
	public:
		Input(Window& window);

		bool is_action_pressed(std::string action) const;
		bool is_action_released(std::string action) const;
		bool is_action_down(std::string action) const;

		void poll_inputs();
	private:
		static bool polling;

		InputMap input_map;
		static InputQueue input_queue;

		static void keyboard_input_event(GLFWwindow* window, int key, int scancode, int action, int mods);
	};

}

#endif