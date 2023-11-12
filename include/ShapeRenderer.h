#ifndef SHAPE_RENDERER_H
#define SHAPE_RENDERER_H\

#include <array>
#include <iostream>

#include <glad/gl.h>

#include "Texture2D.h"
#include "GLSLShader.h"
#include "utility/OdisMath.h"
#include "Color.h"
#include "Mesh.h"

namespace OdisEngine
{
	class ShapeRenderer
	{

	private:
		GLSLShader shader; 
		Mesh mesh{};

	public:
		ShapeRenderer() {};

		ShapeRenderer(GLSLShader& shader) : shader(shader)
		{
			std::vector<float> rect_vertices
			{
				0.0f, 1.0f,
				1.0f, 0.0f,
				0.0f, 0.0f,

				0.0f, 1.0f,
				1.0f, 1.0f,
				1.0f, 0.0f
			};

			mesh.set_vertices(rect_vertices, 2);
			//mesh.set_indices(rect_indices);
		}

		template <IntVectorType T1, ColorTypeRGB T2>
		void draw_rect(T1 pos, T1 size, T2 color, float rotation, float alpha)
		{
			shader.use();

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(pos.x, pos.y, 0.0f));

			model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
			model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

			model = glm::scale(model, glm::vec3(size.x, size.y, 1.0f));

			shader.set_matrix4("model", model);
			shader.set_vector4f("shape_color", glm::vec4(color.r, color.g, color.b, alpha));

			mesh.draw();
			/*
			glBindVertexArray(this->vao);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			*/
		}

		void draw();
	};
}

#endif