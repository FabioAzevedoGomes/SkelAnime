#ifndef SKELANIME_SKELETON_HPP
#define SKELANIME_SKELETON_HPP

#include <glm/gtx/quaternion.hpp>
#include <vector>
#include "Bone.hpp"

class Skeleton {
private:
	std::vector<Bone> bones;

public:
	Skeleton();
	virtual ~Skeleton();

	int GetNumberOfBones();

	std::vector<Bone>& GetBones();
	void PrintBone(int index);
	void AddBone(int parent, glm::mat4 transformation, std::string name);

	float* GetVertexPositionInformation();
	float* GetVertexNormalInformation();
	float* GetVertexColorInformation();
};

void customExport(Skeleton& skeleton, const std::string& path);
Skeleton customImport(const std::string& path);

#endif // SKELANIME_SKELETON_HPP
