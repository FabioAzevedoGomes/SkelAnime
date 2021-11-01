#include "Skeleton.hpp"

#include <glm/gtx/string_cast.hpp>
#include <iostream>

Skeleton::Skeleton() {
}

Skeleton::~Skeleton() {
}

int Skeleton::GetNumberOfBones() {
	return this->bones.size();
}

std::vector<Bone>& Skeleton::GetBones() {
	return this->bones;
}

void Skeleton::PrintBone(int index) {
	if (index < 0 || index > this->bones.size() - 1) {
		std::cerr << "Bone index " << index << " out of range [0, " << this->bones.size() - 1 << "]." << std::endl;
		exit(1);
	}
	Bone bone = this->bones[index];
	std::cout << "Bone at index " << index << ", " << bone.name << std::endl;
	std::cout << "Transformation: " << glm::to_string(bone.transformation) << std::endl;
	std::cout << "Parent: " << bone.parent << std::endl;
	std::cout << "Children: (" << bone.children.size() << ")" << std::endl;
	for (auto i = bone.children.begin(); i != bone.children.end(); i++) {
		std::cout << *i << " ";
	}
	std::cout << std::endl;
}

void Skeleton::AddBone(int parent, glm::mat4 transformation, std::string name) {
	Bone bone;
	bone.transformation = transformation;
	bone.parent = parent;
	bone.name = name;
	this->bones.push_back(bone);
	if (parent >= 0 && parent <= this->bones.size() - 1) {
		this->bones[parent].children.push_back(this->bones.size() - 1);
	}
}

void Skeleton::RotateBone(int bone, glm::quat rotation) {

}
