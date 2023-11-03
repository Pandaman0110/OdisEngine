#ifndef ODISMATH_H
#define ODISMATH_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

template <typename T>
concept VectorType= requires (T component)
{
	{ component.x };
	{ component.y };
};

#endif