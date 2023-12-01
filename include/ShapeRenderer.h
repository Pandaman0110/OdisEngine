#ifndef SHAPE_RENDERER_H
#define SHAPE_RENDERER_H

#include <array>
#include <iostream>
#include <numbers>

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

		std::vector<float> build_polygon(size_t num_sides)
		{
			const size_t num_triangles = num_sides;
			const size_t num_vertices = num_triangles + 1;
			//one extra on the perimeter of the circle
			//const size_t num_vertices = num_sides + 1;
			const auto pi = std::numbers::pi_v<float>;
			const float angle = 360.0f / num_sides;

			std::vector<float> polygon_vertices{};
			polygon_vertices.reserve(num_vertices);

			//polygon_vertices.push_back(0.0f);
			//polygon_vertices.push_back(0.0f);

			//generate the vertices for the outer edge of the circle
			for (std::size_t i = 0; i < num_vertices; i++)
			{
				float current_angle_degrees = angle * i;
				float current_angle_radians = glm::radians(current_angle_degrees);

				float x = std::cos(current_angle_radians);
				float y = std::sin(current_angle_radians);

				polygon_vertices.push_back(x);
				polygon_vertices.push_back(y);
			}

			return polygon_vertices;
		};
	public:
		ShapeRenderer() {};

		ShapeRenderer(GLSLShader& shader) : shader(shader)
		{
			std::vector<float> rect_vertices
			{
				0.0f, 1.0f,
				0.0f, 0.0f,
				1.0f, 1.0f,
				1.0f, 0.0f,
			};

			mesh.set_vertices(std::move(rect_vertices), 2);
		}

		template <RectangleVectorType T1, ColorTypeRGB T2>
		void draw(T1 rect, T2 color, float rotation, float alpha)
		{
			draw_rect(rect.pos, rect.size, color, rotation, alpha);
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

			mesh.draw(DrawMode::triangle_strip, mesh.size());
		}

		template <IntVectorType T1, ColorTypeRGB T2>
		void draw_polygon(T1 pos, float radius, size_t num_sides, T2 color, float rotation)
		{
			shader.use();

			std::vector<float> vertices = build_polygon(num_sides);

			//std::cout << "Num vertex's" << vertices.size() / 2 << std::endl;
			//std::ranges::for_each(vertices, [](auto& e) {std::cout << e << std::endl; });
			//std::abort();

			Mesh poly_mesh{};
			poly_mesh.set_vertices(std::move(vertices), 2);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(pos.x, pos.y, 0.0f));

			model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));

			model = glm::scale(model, glm::vec3(radius, radius, 1.0f));

			shader.set_matrix4("model", model);
			shader.set_vector4f("shape_color", glm::vec4(color.r, color.g, color.b, 1.0f));

			poly_mesh.draw(DrawMode::triangle_fans, poly_mesh.size());

			//std::cout << polygon_vertices.size() << std::endl;;
		}
	};
}

#endif