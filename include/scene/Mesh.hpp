#ifndef SKELANIME_MESH_HPP
#define SKELANIME_MESH_HPP

#include "Vertex.hpp"

typedef struct mesh_t {
	int numberOfVertices;
	Vertex* vertices;
} Mesh;

#endif // SKELANIME_MESH_HPP
