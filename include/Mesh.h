#ifndef MESH_H
#define MESH_H

#include <glad/gl.h>

#include <iostream>
#include <vector>
#include <span>
#include <type_traits>
#include <algorithm>
#include <initializer_list>

#include "OdisMath.h"

namespace OdisEngine
{
	class Mesh
	{
	private:
		unsigned int vao, vbo, ebo;

		int current_attribute_num = 0;

		//{num, offset}
		void set_vertex_attributes(std::initializer_list<int> num_vertices)
		{
			int sum = 0;
			for (auto num : num_vertices)
			{
				glEnableVertexAttribArray(current_attribute_num);
				glVertexAttribPointer(current_attribute_num, num, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(vertices.size() * sizeof(float)), (void*)(sum * sizeof(float)));

				++current_attribute_num;
				sum += num;
			}
		};
	public:
		Mesh() {};

		void set_vertices(std::span<float> vertices, int num_vertices ...)
		{
			this->vertices.clear();
			this->vertices.resize(vertices.size(), 0.0f);
			std::copy(vertices.begin(), vertices.end(), this->vertices.begin());

			set_vertex_attributes({ num_vertices });
		}

		void set_indices(std::span<unsigned int> indices)
		{
			this->indices.clear();
			this->indices.resize(indices.size(), 0u);
			std::copy(indices.begin(), indices.end(), this->indices.begin());
		}

		void bind()
		{
			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);
			glGenBuffers(1, &ebo);

			glBindVertexArray(vao);

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), this->vertices.data(), GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), this->indices.data(), GL_STATIC_DRAW);

			// vertex positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		void draw()
		{
			glBindVertexArray(this->vao);
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
			glBindVertexArray;
		}

		std::vector<float> vertices{};
		std::vector<unsigned int> indices{};
	};
}



#endif