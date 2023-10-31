#ifndef COLOR_H
#define COLOR_H

#include <concepts>
#include <type_traits>

namespace OdisEngine {
	template <typename T>
	concept ColorTypeRGB = requires (T color)
	{
		{ std::is_convertible_v<decltype(color.r), float> };
		{ std::is_convertible_v<decltype(color.g), float> };
		{ std::is_convertible_v<decltype(color.b), float> };
	};

	struct ColorRGB
	{
		float r = 1.0f;
		float g = 1.0f;
		float b = 1.0f;
	};

	typedef ColorRGB Color;
}

#endif