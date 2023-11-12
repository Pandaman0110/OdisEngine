#ifndef ODISMATH_H
#define ODISMATH_H

#include <type_traits>
#include <concepts>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace OdisEngine
{
	constexpr size_t MATRIX_MAX_SIZE = 4;

	template <typename T>
	concept VectorType = requires (T component)
	{
		{ component.x };
		{ component.y };
	};

	template <typename T>
	concept MatrixType = requires (T component)
	{
		{ component.operator[] } ;
	};

	template <typename T>
	concept IntMatrixType = requires (T matrix)
	{
		requires MatrixType<T>;
		requires std::integral<decltype(matrix[1])>;
	};

	template <typename T>
	concept FloatMatrixType = requires (T matrix)
	{
		requires MatrixType<T>;
		requires std::floating_point<decltype(matrix[1])>;
	};


	template <typename T>
	concept IntVectorType = requires (T component)
	{
		requires VectorType<T>;
		requires std::integral<decltype(component.x)>;
		requires std::integral<decltype(component.y)>;
	};

	template <typename T>
	concept FloatVectorType = requires (T component)
	{
		requires VectorType<T>;
		requires std::floating_point<decltype(component.x)>;
		requires std::floating_point<decltype(component.y)>;
	};

}


#endif