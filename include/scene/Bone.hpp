#ifndef SKELANIME_BONE_HPP
#define SKELANIME_BONE_HPP

#include <glm/mat4x4.hpp>
#include <string>

typedef struct bone_t {
	glm::mat4 transformation;
	int parent;
	std::vector<int> children;
	std::string name;
} Bone;

#endif // SKELANIME_BONE_HPP
