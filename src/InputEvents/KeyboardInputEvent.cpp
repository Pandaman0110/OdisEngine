#include "InputEvent.h"

using namespace OdisEngine;

KeyboardInputEvent::KeyboardInputEvent(Key key, KeyScancode key_scancode, bool pressed, KeyName key_name) : key(key), scancode(scancode), key_name(key_name)
{
	this->pressed = pressed;
}

/*
std::string_view KeyboardInputEvent::get_event_string() const
{
	if (get_is_pressed()) return "Pressed";
	if (get_is_down()) return "Down";
	if (get_is_released()) return "Released";
}
*/

bool OdisEngine::operator== (const KeyboardInputEvent& lhs, const KeyboardInputEvent& rhs)
{
	return lhs.is(rhs.get_key());
}
