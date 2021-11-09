#include "Skeleton.hpp"

#include <glm/gtx/matrix_decompose.hpp>
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


float* Skeleton::GetVertexPositionInformation() {
	float* positionInformation = new float[GetNumberOfBones() * 6];

	memset(positionInformation, 0.0f, GetNumberOfBones() * 6);

	int index = 0;
	glm::vec3 translation, skew, scale;
	glm::vec4 perspective;
	glm::quat rotation;
	for (int i = 0; i < GetNumberOfBones(); i++) {
		if (this->bones[i].parent != -1) {

			glm::mat4 transform(1.0f);
			int parent = this->bones[i].parent;
			do {
				transform = this->bones[parent].transformation * transform;
				parent = this->bones[parent].parent;
			} while (parent != -1);

			glm::decompose(transform, scale, rotation, translation, skew, perspective);

			*(positionInformation + index + 0) = (float)translation.x;
			*(positionInformation + index + 1) = (float)translation.y;
			*(positionInformation + index + 2) = (float)translation.z;

			transform = glm::mat4(1.0f);

			parent = i;
			do {
				transform = this->bones[parent].transformation * transform;
				parent = this->bones[parent].parent;
			} while (parent != -1);

			glm::decompose(transform, scale, rotation, translation, skew, perspective);


			*(positionInformation + index + 3) = (float)translation.x;
			*(positionInformation + index + 4) = (float)translation.y;
			*(positionInformation + index + 5) = (float)translation.z;
		}
		else {
			glm::decompose(this->bones[i].transformation, scale, rotation, translation, skew, perspective);
			*(positionInformation + index + 0) = translation.x;
			*(positionInformation + index + 1) = translation.y;
			*(positionInformation + index + 2) = translation.z;
			*(positionInformation + index + 3) = translation.x;
			*(positionInformation + index + 4) = translation.y;
			*(positionInformation + index + 5) = translation.z;
		}
		index += 6;
	}

	return positionInformation;
}

float* Skeleton::GetVertexNormalInformation() {
	float* normalInformation = new float[GetNumberOfBones() * 6];

	memset(normalInformation, 1.0f, GetNumberOfBones() * 6);

	return normalInformation;
}

float* Skeleton::GetVertexColorInformation() {
	float* colorInformation = new float[GetNumberOfBones() * 6];

	memset(colorInformation, 1.0f, GetNumberOfBones() * 6);

	return colorInformation;
}
