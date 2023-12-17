#ifndef MESH_H
#define MESH_H

#include <glad/gl.h>

#include <iostream>
#include <vector>
#include <span>
#include <numeric>
#include <type_traits>
#include <algorithm>
#include <initializer_list>
#include <concepts>

#include "utility/OdisMath.h"

namespace OdisEngine
{
	enum class DrawPrimitive
	{
		triangles = GL_TRIANGLES,
		triangle_fans = GL_TRIANGLE_FAN,
		triangle_strip = GL_TRIANGLE_STRIP,
		quads = GL_QUADS,
		points = GL_POINTS,
		lines = GL_LINES,
		line_strip = GL_LINE_STRIP,
		line_loop = GL_LINE_LOOP,
		patches = GL_PATCHES,
	};

	enum class DrawMode
	{
		dynamic_draw = GL_DYNAMIC_DRAW,
		static_draw = GL_STATIC_DRAW,
		stream_draw = GL_STREAM_DRAW,
	};

	class Mesh
	{
	private:
		unsigned int vao, vbo, ebo;
		
		int stride;

		//{num, offset}
		void set_vertex_attributes(std::initializer_list<int> vertex_sizes)
		{
			stride = std::accumulate(vertex_sizes.begin(), vertex_sizes.end(), 0);

			int sum = 0;
			int current_attribute_num = 0;

			for (auto num_vertices : vertex_sizes)
			{
				glVertexAttribPointer(current_attribute_num, num_vertices, GL_FLOAT, GL_FALSE, stride * sizeof(float), reinterpret_cast<void*>(sum * sizeof(float)));
				glEnableVertexAttribArray(current_attribute_num);

				//std::cout << current_attribute_num << " " << num_vertices << " " << stride * sizeof(float) << " " << sum * sizeof(float) << std::endl;

				++current_attribute_num;
				sum += num_vertices;
			}
		};
	public:
		Mesh() {};

		void erase() 
		{
			glDeleteVertexArrays(1, &this->vao);
		};

		template <std::integral ... Args>
		void set_vertices(std::vector<float> vertices, DrawMode draw_mode, Args ... args)
		{
			this->vertices.clear();
			this->vertices.shrink_to_fit();
			this->vertices.swap(vertices);

			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);

			glBindVertexArray(vao);

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), this->vertices.data(), static_cast<int>(draw_mode));

			set_vertex_attributes({ args... });

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		template <std::integral ... Args>
		void set_vertices(std::vector<float> vertices, Args ... args)
		{
			set_vertices(vertices, DrawMode::static_draw, std::forward<Args>(args)...);
		}
		
		void set_indices(std::span<unsigned int> indices)
		{
			this->indices.clear();
			this->indices.resize(indices.size(), 0u);
			std::copy(indices.begin(), indices.end(), this->indices.begin());

			glGenBuffers(1, &ebo);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);

		}
		
		void draw(size_t count)
		{
			draw(DrawPrimitive::triangles, count);
		}

		void draw(DrawPrimitive draw_primitive, size_t count)
		{
			glBindVertexArray(vao);

			const int offset = 0;
			glDrawArrays(static_cast<int>(draw_primitive), offset, static_cast<GLsizei>(count));
			//std::cout << static_cast<GLsizei>(vertices.size() / stride) << '\n';

			glBindVertexArray(0);
		}

		void draw_instanced(DrawPrimitive draw_primitive, size_t count, size_t instance_count)
		{
			glBindVertexArray(vao);

			const int offset = 0;
			glDrawArraysInstanced(static_cast<int>(draw_primitive), offset, static_cast<GLsizei>(count), static_cast<GLsizei>(instance_count));

			glBindVertexArray(0);
		}

		void bind()
		{
			glBindVertexArray(vao);
		}

		size_t size() const { return vertices.size() / 2; };

		std::vector<float> vertices{};
		std::vector<unsigned int> indices{};
	};
}



#endif