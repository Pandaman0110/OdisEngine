#include "Input.h"

#include <functional>
#include <algorithm>

using namespace OdisEngine;

Input::Input(Window& window)
{
	window.set_keyboard_input_callback(std::bind(&Input::keyboard_input_callback, this, std::placeholders::_1));
	window.set_mouse_button_input_callback(std::bind(&Input::mouse_button_input_callback, this, std::placeholders::_1));

}


//TODO implement the actions thing
/*
bool Input::is_action_pressed(std::string action) const
{
	for (auto& input_event : input_queue)
	{
		//if (input_event.get_key_name() == action and input_event.get_is_pressed())
		//	return true;
	}
	return false;
}

bool Input::is_action_released(std::string action) const
{
	for (auto& input_event : input_queue)
	{
		//if (input_event.get_key_name() == action and input_event.get_is_released())
		//return true;
	}
	return false;
}

bool Input::is_action_down(std::string action) const
{
	for (auto& input_event : input_queue)
	{
		//if (input_event.get_key_name() == action and input_event.get_is_down())
			//return true;
	}
	return false;
}

*/

bool Input::is_key_pressed(Key key) const
{
	for (auto& input_event : keyboard_input_queue)
	{
		//std::cout << input_event.is_pressed() << "\n";

		if (input_event.is(key) and input_event.is_pressed())
			return true;
	}

	return false;
}

bool Input::is_key_released(Key key) const
{
	for (auto& input_event : keyboard_input_queue)
	{
		if (input_event.is(key) and input_event.is_released())
			return true;
	}

	return false;
}

bool Input::is_key_down(Key key) const
{
	for (auto& input_event : keyboard_input_queue)
	{
		if (input_event.is(key) and input_event.is_down())
			return true;
	}

	return false;
}

bool Input::is_mouse_button_pressed(MouseButton button) const
{
	for (auto& input_event : mouse_button_input_queue)
	{
		if (input_event.is(button) and input_event.is_pressed())
			return true;
	}

	return false;
}

bool Input::is_mouse_button_released(MouseButton button) const
{
	for (auto& input_event : mouse_button_input_queue)
	{
		if (input_event.is(button) and input_event.is_released())
			return true;
	}

	return false;
}

bool Input::is_mouse_button_down(MouseButton button) const
{
	for (auto& input_event : mouse_button_input_queue)
	{
		if (input_event.is(button) and input_event.is_down())
			return true;
	}

	return false;
}


void Input::poll_inputs()
{
	for (auto& input_event : keyboard_input_queue)
		input_event++;

	for (auto& input_event : mouse_button_input_queue)
		input_event++;

	keyboard_input_queue.erase(std::remove_if(keyboard_input_queue.begin(), keyboard_input_queue.end(),
		[](KeyboardInputEvent input_event)
		{
			return input_event.expired();
		}), keyboard_input_queue.end());

	mouse_button_input_queue.erase(std::remove_if(mouse_button_input_queue.begin(), mouse_button_input_queue.end(),
		[](MouseButtonInputEvent input_event)
		{
			return input_event.expired();
		}), mouse_button_input_queue.end());
}

void Input::keyboard_input_callback(KeyboardInputEvent&& keyboard_input_event)
{
	if (keyboard_input_event.is_pressed())
		keyboard_input_queue.push_back(std::move(keyboard_input_event));
	else
	{
		for (size_t i = 0; i < keyboard_input_queue.size(); i++)
		{
			auto &input_event = keyboard_input_queue.at(i);
			if (input_event == keyboard_input_event)
				keyboard_input_queue.at(i) = std::move(keyboard_input_event);
		}
	};
}

void Input::mouse_button_input_callback(MouseButtonInputEvent&& mouse_button_input_event)
{
	if (mouse_button_input_event.is_pressed())
		mouse_button_input_queue.push_back(std::move(mouse_button_input_event));
	else
	{
		for (size_t i = 0; i < mouse_button_input_queue.size(); i++)
		{
			auto& input_event = mouse_button_input_queue.at(i);
			if (input_event == mouse_button_input_event)
				mouse_button_input_queue.at(i) = std::move(mouse_button_input_event);
		}
	}
}