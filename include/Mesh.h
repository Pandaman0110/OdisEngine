#ifndef MESH_H
#define MESH_H

#include <vector>
#include <span>
#include <type_traits>
#include <concepts>

#include "OdisMath.h"

namespace OdisEngine
{

	class Mesh
	{
	private:
		unsigned int vao, vbo, ebo;

	public:
		Mesh(const std::span<float>& vertices, const std::span<unsigned int>& indices)
		{
			this->vertices.resize(vertices.size());
			this->indices.resize(indices.size());

			std::copy(vertices.begin(), vertices.end(), this->vertices.begin());
			std::copy(indices.begin(), indices.end(), this->indices.begin());

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
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(unsigned int), (void*)0);
			// vertex texture coords
			//glEnableVertexAttribArray(2);
			//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coords));

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		};

		std::vector<float> vertices{};
		std::vector<unsigned int> indices{};
	};
}



#endif