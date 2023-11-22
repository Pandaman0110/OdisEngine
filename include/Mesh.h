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
	class Mesh
	{
	private:
		unsigned int vao, vbo, ebo;
		
		int stride;
		int current_attribute_num = 0;

		//{num, offset}
		void set_vertex_attributes(std::initializer_list<int> num_vertices)
		{
			stride = std::accumulate(num_vertices.begin(), num_vertices.end(), 0);

			int sum = 0;
			for (auto num : num_vertices)
			{
				glEnableVertexAttribArray(current_attribute_num);
				glVertexAttribPointer(current_attribute_num, num, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(stride * sizeof(float)), (void*)(sum * sizeof(float)));

				++current_attribute_num;

				sum += num;
			}

		};
	public:
		Mesh() {};
		~Mesh() 
		{
			glDeleteVertexArrays(1, &this->vao);
		};

		template <std::integral ... Args>
		void set_vertices(std::span<float> vertices, Args ... args)
		{
			this->vertices.clear();
			this->vertices.resize(vertices.size(), 0.0f);
			std::copy(vertices.begin(), vertices.end(), this->vertices.begin());

			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);

			glBindVertexArray(vao);

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

			set_vertex_attributes({ args... });

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		
		void set_indices(std::span<unsigned int> indices)
		{
			this->indices.clear();
			this->indices.resize(indices.size(), 0u);
			std::copy(indices.begin(), indices.end(), this->indices.begin());

			glGenBuffers(1, &ebo);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		}
		
		void draw()
		{
			glBindVertexArray(vao);
			
			glDrawArrays(GL_TRIANGLES, 0, 6);

			glBindVertexArray;
		}

		std::vector<float> vertices{};
		std::vector<unsigned int> indices{};
	};
}



#endif