
#include <concepts>
#include <type_traits>

namespace OdisEngine
{
	template <typename T>
	concept RectangleType = requires (T rect)
	{
		{ std::is_convertible_v<decltype(rect.x), float> };
		{ std::is_convertible_v<decltype(rect.y), float> };
		{ std::is_convertible_v<decltype(rect.w), float> };
		{ std::is_convertible_v<decltype(rect.h), float> };
	};

	struct Rectangle
	{
		float x;
		float y;
		float w;
		float h;
	};
}