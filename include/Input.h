#ifndef INPUT_H
#define INPUT_H

#include <vector>
#include <string>
#include <map>
#include <functional>
#include <algorithm>

#include "Window.h"
#include "Keys.h"
#include "InputEvent.h"

namespace OdisEngine
{
	/*
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
	};
	*/

	//template <typename T>
	//concept InputMapType = requires (T map)
	//{

	//};

	class Input
	{

	private:
		std::vector<ButtonInputEvent> button_input_queue;

		double mouse_x, mouse_y;

		void button_input_callback(ButtonInputEvent button_input_event)
		{
			button_input_event.is_pressed() ? button_input_queue.push_back(button_input_event) :
				std::replace_if(button_input_queue.begin(), button_input_queue.end(), [&](auto& e) { return (e == button_input_event); }, button_input_event);
		}

		void mouse_pos_callback(double x, double y)
		{
			mouse_x = x;
			mouse_y = y;
		}
	public:

		Input(Window* window)
		{
			button_input_queue = {};

			{
				using namespace std::placeholders;
				window->set_keyboard_input_callback(std::bind(&Input::button_input_callback, this, _1));
				window->set_mouse_button_input_callback(std::bind(&Input::button_input_callback, this, _1));
				window->set_mouse_pos_input_callback(std::bind(&Input::mouse_pos_callback, this, _1, _2));
			}
		}

		//TODO input mapping
		//bool is_action_pressed(std::string action) const;
		//bool is_action_released(std::string action) const;
		//bool is_action_down(std::string action) const;

		bool is_key_pressed(Key key) const
		{
			return std::any_of(button_input_queue.begin(), button_input_queue.end(), [&](const auto& e) {  return (e.is(key) and e.is_pressed()); });
		}

		bool is_key_released(Key key) const
		{
			return std::any_of(button_input_queue.begin(), button_input_queue.end(), [&](const auto& e) {  return (e.is(key) and e.is_released()); });
		}

		bool is_key_down(Key key) const
		{
			return std::any_of(button_input_queue.begin(), button_input_queue.end(), [&](const auto& e) {  return (e.is(key) and e.is_down()); });
		}

		void poll_inputs(float dt)
		{
			std::for_each(button_input_queue.begin(), button_input_queue.end(), [&](auto& e) {  e.tick(dt); });

			button_input_queue.erase(std::remove_if(button_input_queue.begin(), button_input_queue.end(),
				[](const auto& input_event) { return input_event.expired(); }),
				button_input_queue.end());
		}

		double get_mouse_x() const { return mouse_x; };
		double get_mouse_y() const { return mouse_y; };

		template <FloatVectorType T>
		T get_mouse_pos() const { return T{ get_mouse_x(), get_mouse_y() }; };



	};

}

#endif