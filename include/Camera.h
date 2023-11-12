#ifndef CAMERA_H
#define CAMERA_H

#include <concepts>
#include <type_traits>
#include <utility>

#include "utility/OdisMath.h"	

namespace OdisEngine
{
	class Camera
	{
	public:
		Camera() {};
		virtual ~Camera() = 0;
		
	};

	class Camera2D
	{
	private:
		glm::mat4 projection;
		glm::mat4 view;

		glm::ivec2 position{ 0, 0 };

		glm::ivec2 target{ 0, 0 };
		float rotation = 0.0f;
		float zoom = 1.0f;
	public:
		Camera2D() {};

		template <IntVectorType T = glm::ivec2>
		Camera2D(T target, float rotation, float zoom) : target(target), rotation(rotation), zoom(zoom)
		{
			//glm::mat4 projection = glm::ortho{ target.x, }
		}

		//projection.

		glm::vec2 get_position() const { return position; };
		float get_rotation() const { return rotation; };

		inline void set_target(glm::vec2 target) { this->target = target; };
		inline void set_rotation(float rotation) { this->rotation = rotation; };

		inline void move(glm::vec2 velocity) { this->position += velocity; };
		inline void rotate(float rotation) { this->rotation += rotation; };
	};


	/* TODO 3d
	class Camera3D : public Camera
	{
	public:
		Camera3D();

		vec3 getPosition() const { return position; };
		quat getOrientation() const { return rotation; };

		void setPosition(vec3 position) { this->position = position; };
		void setOrientation(quat rotation) { this->rotation = rotation; };

	private:
		vec3 position;
		quat orientation;


	};
	*/
}


#endif