#ifndef INPUTEVENT_H
#define INPUTEVENT_H

#include <iostream>
#include <string>
#include <string_view>

#include "Keys.h"

//const float SECONDS_UNTIL_HELD = 0.02f;

namespace OdisEngine
{
	class InputEvent
	{
	public:
		InputEvent() {};

		inline bool is_pressed() const { return (pressed == true and echo == false); };
		inline bool is_down() const { return echo == true; };
		inline bool is_released() const { return pressed == false; };

		inline bool expired() const { return is_released() and time_held > 0; };
		inline void tick(float dt) { time_held += dt; echo = true; };
	protected:
		bool pressed;

	private:
		bool echo = false;
		float time_held = 0.0f;
	};

	class ButtonInputEvent : public InputEvent
	{
	private:
		Key key_code;
		KeyScancode scancode;
		std::string key_name;

	public:
		ButtonInputEvent(Key key_code, KeyScancode key_scancode, bool pressed, std::string key_name) : key_code(key_code), scancode(key_scancode), key_name(key_name)
		{
			this->pressed = pressed;
		}

		inline bool is(Key key) const { return key_code == key; };

		inline Key get_key() const { return key_code; };
		inline std::string get_key_name() const { return key_name; };
	};

	inline bool operator== (const ButtonInputEvent& lhs, const ButtonInputEvent& rhs)
	{
		return lhs.is(rhs.get_key());
	};
}

#endif