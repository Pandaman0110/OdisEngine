#ifndef SHAPE_RENDERER_H
#define SHAPE_RENDERER_H

#include <array>
#include <iostream>
#include <numbers>

#include <glad/gl.h>
#include "Log.h"

#include "Texture2D.h"
#include "GLSLShader.h"
#include "utility/OdisMath.h"
#include "Color.h"
#include "Mesh.h"

constexpr size_t max_instances = 1024;

namespace OdisEngine
{
	struct RectangleDrawCommand
	{
		glm::mat4 model;
		glm::vec4 color;
	};

	struct PolygonDrawCommand
	{
		glm::vec2 pos{ 0, 0 };
		float radius{ 1.0f };
		size_t num_sides{ 4 };
		glm::vec3 color{ 1.0f, 1.0f, 1.0f };
		float rotation{ 0.0f };
	};

	class ShapeRenderer
	{

	private:
		GLSLShader shader;
		Mesh mesh{};

		unsigned int shape_model_buffer;
		unsigned int shape_color_buffer;
		std::vector<glm::mat4> rectangle_models;
		std::vector<glm::vec4> rectangle_colors;
		//std::vector<PolygonDrawCommand, max_instances> polygon_draw_queue;

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

		template <VectorType T1, ColorTypeRGB T2>
		void flush_rectangles(T1 pos, T1 size, T2 color, float rotation, float alpha)
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

		template <VectorType T1, ColorTypeRGB T2>
		void flush_polygons(T1 pos, float radius, size_t num_sides, T2 color, float rotation)
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
	public:
		ShapeRenderer() {};

		ShapeRenderer(GLSLShader& shader) : shader(shader)
		{
			// vertex buffer object

			std::vector<float> rect_vertices
			{
				0.0f, 1.0f,
				0.0f, 0.0f,
				1.0f, 1.0f,
				1.0f, 0.0f,
			};

			mesh.set_vertices(std::move(rect_vertices), 2);

			mesh.bind();

			glGenBuffers(1, &shape_model_buffer);
			glGenBuffers(1, &shape_color_buffer);

			glBindBuffer(GL_ARRAY_BUFFER, shape_model_buffer);
			glBufferData(GL_ARRAY_BUFFER, max_instances * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);

			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));

			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));

			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

			glBindBuffer(GL_ARRAY_BUFFER, shape_color_buffer);
			glBufferData(GL_ARRAY_BUFFER, max_instances * sizeof(glm::vec4) * 4, nullptr, GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);

			glVertexAttribDivisor(1, 1);
			glVertexAttribDivisor(2, 1);
			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);

			glBindVertexArray(0);
		}

		template <VectorType T1, ColorTypeRGB T2>
		void draw_rect(T1 pos, T1 size, T2 color, float rotation, float alpha)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(pos.x, pos.y, 0.0f));

			model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
			model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

			model = glm::scale(model, glm::vec3(size.x, size.y, 1.0f));

			glm::vec4 rect_color = { color.r, color.g, color.b, alpha };

			rectangle_models.push_back(model);
			rectangle_colors.push_back(rect_color);

			if (rectangle_models.size() >= max_instances)
				flush();
		}
		

		template <VectorType T1, ColorTypeRGB T2>
		void draw_polygon(T1 pos, float radius, size_t num_sides, T2 color, float rotation)
		{
			shader.use();

			std::vector<float> vertices = build_polygon(num_sides);

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

		void flush()
		{
			shader.use();

			int offset = 0;

			glBindBuffer(GL_ARRAY_BUFFER, shape_model_buffer);
			glBufferSubData(GL_ARRAY_BUFFER, offset, rectangle_models.size() * sizeof(glm::mat4), rectangle_models.data());

			glBindBuffer(GL_ARRAY_BUFFER, shape_color_buffer);
			glBufferSubData(GL_ARRAY_BUFFER, offset, rectangle_models.size() * sizeof(glm::mat4), rectangle_colors.data());

			mesh.draw_instanced(DrawMode::triangle_strip, 4, rectangle_models.size());

			rectangle_models.clear();
			rectangle_colors.clear();
		}


	};

}

#endif