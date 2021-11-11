#ifndef SKELANIME_VERTEX_HPP
#define SKELANIME_VERTEX_HPP

#include <glm/vec4.hpp>

#define MAX_INFLUENCING_BONES 1

typedef struct vertex_t {
	glm::vec4 position;
	glm::vec4 normal;
	glm::vec4 color;
	int boneIds[MAX_INFLUENCING_BONES];
	float boneWeights[MAX_INFLUENCING_BONES];
	float relativePositions[4 * MAX_INFLUENCING_BONES];
} Vertex;

#endif // SKELANIME_VERTEX_HPP
