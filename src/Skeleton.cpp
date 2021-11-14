#include "Skeleton.hpp"

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <fstream>

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
	glm::mat4 transform = transformation;

	if (parent >= 0 && parent <= this->bones.size() - 1) {
		int iterationParent = parent;
		do {
			transform = this->bones[iterationParent].transformation * transform;
			iterationParent = this->bones[iterationParent].parent;
		} while (iterationParent != -1);
	}
	bone.cumulativeTransformation = transform;

	this->bones.push_back(bone);
	if (parent >= 0 && parent <= this->bones.size() - 1) {
		this->bones[parent].children.push_back(this->bones.size() - 1);
	}
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

void customExport(Skeleton& skeleton, const std::string& path) {
	std::ofstream output(path, std::ios::out | std::ios::binary);
	if (!output) {
		throw new std::runtime_error("Cannot open or create export file");
	}
	int boneCount = skeleton.GetNumberOfBones();

	output.write((char*)&boneCount, sizeof(int));
	for (int index = 0; index < skeleton.GetNumberOfBones(); index++) {
		auto& bone = skeleton.GetBones()[index];
		int num_children = bone.children.size();
		int name_size = bone.name.length() + 1;

		output.write((char*)&index, sizeof(int));
		output.write((char*)&bone.parent, sizeof(int));
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				output.write((char*)&bone.transformation[i][j], sizeof(float));
			}
		}
		output.write((char*)&name_size, sizeof(int));
		output.write((char*)bone.name.c_str(), sizeof(char) * name_size);
	}

	output.close();
}

Skeleton customImport(const std::string& path) {
	Skeleton s;

	std::ifstream input(path, std::ios::in | std::ios::binary);

	int boneCount = 0;
	input.read((char*)&boneCount, sizeof(int));

	for (int index = 0; index < boneCount; index++) {
		int boneIndex, name_size, parent;

		glm::mat4 transform(1.0f);
		input.read((char*)&boneIndex, sizeof(int));
		input.read((char*)&parent, sizeof(int));
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				input.read((char*)&transform[i][j], sizeof(float));
			}
		}
		input.read((char*)&name_size, sizeof(int));
		
		std::string name(name_size,'\0');
		input.read((char*)name.data(), sizeof(char) * name_size);

		s.AddBone(parent, transform, name);
	}

	input.close();

	return s;
}
