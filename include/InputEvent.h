#ifndef INPUTEVENT_H
#define INPUTEVENT_H

#include <iostream>
#include <string>
#include <string_view>

#include "Keys.h"

const int KEYDOWN_FRAMECOUNT = 5;

namespace OdisEngine
{
	class InputEvent
	{
	public:
		InputEvent(Key key, KeyScancode key_scancode, KeyEvent key_event, KeyName key_name);

		InputEvent& operator++() { frames_held++; echo = true; return *this; };
		InputEvent operator++(int) { InputEvent tmp(*this); operator++(); return tmp; };

		inline Key get_key() const { return key; };
		inline KeyName get_key_name() const { return key_name; };
		
		inline bool get_is_pressed() const { return !echo and !get_is_released(); };
		inline bool get_is_down() const { return frames_held >= KEYDOWN_FRAMECOUNT; };
		inline bool get_is_released() const { return key_event == KeyEvent::KEY_RELEASED; };

		std::string_view get_event_string() const;

		friend std::ostream& operator<< (std::ostream& stream, const InputEvent& input_event);

	private:
		bool echo = false;
		uint16_t frames_held;

		Key key;
		KeyScancode scancode;
		KeyEvent key_event;
		KeyName key_name;
	};


	bool operator== (const InputEvent& lhs, const InputEvent& rhs);
}



#endif