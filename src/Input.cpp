#include "Input.h"

#include <functional>

using namespace OdisEngine;

InputQueue Input::input_queue;

Input::Input(Window& window)
{
	glfwSetKeyCallback(window.get_window_handle(), keyboard_input_event);
}

bool Input::is_action_pressed(std::string action) const
{
	for (auto& input_event : input_queue)
	{
		if (input_event.get_key_name() == action and input_event.get_is_pressed())
			return true;
	}
	return false;
}

bool Input::is_action_released(std::string action) const
{
	for (auto& input_event : input_queue)
	{
		if (input_event.get_key_name() == action and input_event.get_is_released())
			return true;
	}
	return false;
}

bool Input::is_action_down(std::string action) const
{
	for (auto& input_event : input_queue)
	{
		if (input_event.get_key_name() == action and input_event.get_is_down())
			return true;
	}
	return false;
}

void Input::poll_inputs()
{
	for (auto i = input_queue.begin(); i != input_queue.end(); i++)
	{
		auto &input_event = *i;

		if (!input_event.get_is_released())
			input_event++; 
	}

	input_queue.remove_if([](const InputEvent& input_event) { return input_event.get_is_released(); });

	glfwPollEvents();
}

void Input::keyboard_input_event(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	KeyName key_name;

	if (glfwGetKeyName(key, scancode) != NULL)
		key_name = glfwGetKeyName(key, scancode);
	else
		key_name = "No Name";

	InputEvent input_event = InputEvent(static_cast<Key>(key), static_cast<KeyScancode>(scancode), static_cast<KeyEvent>(action), key_name);

	if (action == static_cast<int>(KeyEvent::KEY_RELEASED))
		input_queue.remove_if([&input_event](const InputEvent& input_event2) { return input_event2 == input_event; });

	if (action != 2)
		Input::input_queue.push_back(input_event);
}