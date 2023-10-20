#ifndef INPUTEVENT_H
#define INPUTEVENT_H

#include <iostream>
#include <string>
#include <string_view>

#include "Keys.h"

//TODO change this to use seconds
const int KEYDOWN_FRAMECOUNT = 5;

namespace OdisEngine
{
	class InputEvent
	{
	public:
		InputEvent();

		InputEvent& operator++() { frames_held++; echo = true; return *this; };
		InputEvent operator++(int) { InputEvent tmp(*this); operator++(); return *this; };
		
		inline bool is_pressed() const {  return pressed == true and echo == false; };
		inline bool is_down() const { return frames_held > KEYDOWN_FRAMECOUNT and echo != false; };
		inline bool is_released() const { return pressed == false; };

		inline bool expired() const { return is_released() and frames_held > 0; };

	protected:
		bool pressed;

	private:
		bool echo = false;
		uint16_t frames_held = 0;
	};

	class KeyboardInputEvent : public InputEvent
	{
	private:
		Key key_code;
		KeyScancode scancode;
		std::string key_name;

	public:
		KeyboardInputEvent(Key key_code, KeyScancode key_scancode, bool pressed, std::string&& key_name) : key_code(key_code), scancode(key_scancode), key_name(std::move(key_name))
		{
			this->pressed = pressed;
		}

		inline bool is(Key key) const { return key_code == key; };

		inline Key get_key() const { return key_code; };
		inline std::string get_key_name() const { return key_name; };
	};
	
	inline bool operator== (const KeyboardInputEvent& lhs, const KeyboardInputEvent& rhs)
	{
		return lhs.is(rhs.get_key());
	};
	
	class MouseButtonInputEvent : public InputEvent
	{
	private:
		MouseButton button_num;

	public:
		MouseButtonInputEvent(MouseButton button_num, bool pressed) : button_num(button_num)
		{
			this->pressed = pressed;
		}

		inline bool is(MouseButton button_num) const { return this->button_num == button_num; };

		inline MouseButton get_key() const { return button_num; };
	};
	
	inline bool operator== (const MouseButtonInputEvent& lhs, const MouseButtonInputEvent& rhs)
	{
		return lhs.is(rhs.get_key());
	};
	
}



#endif