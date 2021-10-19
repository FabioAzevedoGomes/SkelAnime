#ifndef SKELANIME_MESH_HPP
#define SKELANIME_MESH_HPP

#include <vector>

#include "Vertex.hpp"

typedef struct skelanime_mesh {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	struct skelanime_mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
		this->vertices = vertices;
		this->indices = indices;
	}
} Mesh;

#endif // SKELANIME_MESH_HPP
