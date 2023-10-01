#ifndef SURFACE_H
#define SURFACE_H

#include <vector>

namespace OdisEngine {

	struct Vertex {
		float x;
		float y;
		float z;
	};

	typedef std::vector<Vertex> VertexBuffer;
	typedef std::vector<uint16_t> VertexBufferIndices;

	struct Mesh {
		VertexBuffer vertices;
		VertexBufferIndices vertex_indices;
	};
}


#endif