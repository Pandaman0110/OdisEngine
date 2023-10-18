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
	public:
		KeyboardInputEvent(Key key_code, KeyScancode key_scancode, bool pressed, KeyName key_name);

		inline bool is(Key key) const { return key_code == key; };

		inline Key get_key() const { return key_code; };
		inline KeyName get_key_name() const { return key_name; };

	private:
		Key key_code;
		KeyScancode scancode;
		KeyName key_name;
	};

	bool operator== (const KeyboardInputEvent& lhs, const KeyboardInputEvent& rhs);
}



#endif