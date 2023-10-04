#include "InputEvent.h"

using namespace OdisEngine;

InputEvent::InputEvent(Key key, KeyScancode scancode, KeyEvent key_event, KeyName key_name) : key(key), scancode(scancode), key_event(key_event), key_name(key_name)
{
	frames_held = 0;
}

std::string_view OdisEngine::InputEvent::get_event_string() const
{
	if (get_is_pressed()) return "Pressed";
	if (get_is_down()) return "Down";
	if (get_is_released()) return "Released";
}

std::ostream& OdisEngine::operator<<(std::ostream& stream, const InputEvent& input_event)
{
	stream << "Key code: " << static_cast<int>(input_event.key) << "\n";
	stream << "Key scancode: " << input_event.scancode << "\n";
	stream << "Key event: " << input_event.get_event_string() << "\n";
	stream << "Key name: " << input_event.key_name << std::endl;
	return stream;	
}

bool OdisEngine::operator==(const InputEvent& lhs, const InputEvent& rhs)
{
	return lhs.get_key() == rhs.get_key();
}
