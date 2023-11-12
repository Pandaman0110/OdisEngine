
#include <concepts>
#include <type_traits>

namespace OdisEngine
{
	template <typename T>
	concept RectangleType = requires (T rect)
	{
		{ rect.x };
		{ rect.y };
		{ rect.w };
		{ rect.h };
		{ std::is_convertible_v<decltype(rect.x), float> };
		{ std::is_convertible_v<decltype(rect.y), float> };
		{ std::is_convertible_v<decltype(rect.w), float> };
		{ std::is_convertible_v<decltype(rect.h), float> };
	};

	template <typename T1, typename ... T2>
	concept TRectangleType = requires (T1 rect)
	{
		requires RectangleType<T1>;
		{ std::is_convertible_v<decltype(rect.x), T2...> };
		{ std::is_convertible_v<decltype(rect.y), T2...> };
		{ std::is_convertible_v<decltype(rect.w), T2...> };
		{ std::is_convertible_v<decltype(rect.h), T2...> };
	};
	
}