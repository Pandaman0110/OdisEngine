#ifndef COLOR_H
#define COLOR_H

#include <concepts>
#include <type_traits>

namespace OdisEngine {
	template <typename T>
	concept ColorTypeRGB = requires (T color)
	{
		requires std::floating_point<decltype(color.r)>;
		requires std::floating_point<decltype(color.g)>;
		requires std::floating_point<decltype(color.b)>;
	};

	struct ColorRGB
	{
		using value_type = float;
		value_type r = 1.0f;
		value_type g = 1.0f;
		value_type b = 1.0f;
	};

	typedef ColorRGB Color;
}

#endif