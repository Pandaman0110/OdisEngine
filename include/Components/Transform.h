#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "OdisMath.h"


namespace OdisEngine
{
	struct Transform2D
	{
		vec3 position;
		vec2 scale;
		float rotation;

		Transform2D(vec3 position = {0, 0, 0}, float rotation = 0, vec2 scale = {1, 1}) : position(position), rotation(rotation), scale(scale) {};
	};

	struct Transform3D
	{
		vec3 position;
		vec3 rotation;
		vec3 scale;

		Transform3D(vec3 position = {0, 0, 0}, vec3 rotation = {0, 0, 0}, vec3 scale = {1, 1, 1}) : position(position), rotation(rotation), scale(scale) {};
	};
}


#endif