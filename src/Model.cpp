#include "Model.hpp"

#include <iostream>
#include <glm/gtx/string_cast.hpp>

Model::Model(Skeleton skeleton, std::vector<Vertex> vertices) {
	this->restSkeleton = skeleton;
	this->skeleton = skeleton;
	this->vertices = vertices;
}

Model::~Model() {

}

void Model::PrintSkeleton() {
	for (int i = 0; i < this->skeleton.GetNumberOfBones(); i++) {
		this->skeleton.PrintBone(i);;
	}
}

void Model::PrintVertices() {
	for (int i = 0; i < this->vertices.size(); i++) {
		std::cout << "Vertex: " << std::endl;
		std::cout << "Position: " << glm::to_string(this->vertices[i].position) << std::endl;
		std::cout << "Normal: " << glm::to_string(this->vertices[i].normal) << std::endl;
		std::cout << "Color: " << glm::to_string(this->vertices[i].color) << std::endl;
		std::cout << "Bone weights: " << std::endl;
		for (int j = 0; j < MAX_INFLUENCING_BONES; j++) {
			std::cout << "Bone " << this->vertices[i].boneIds[j] << ": " << this->vertices[i].boneWeights[j] << std::endl;
		}
	}
}

Skeleton* Model::GetSkeleton() {
	return &this->restSkeleton;
}

long Model::GetVertexCount() {
	return this->vertices.size();
}

float* Model::GetVertexPositionInformation() {
	float* positionInformation = new float[GetVertexCount() * 3];

	int index = 0;
	for (int i = 0; i < GetVertexCount(); i++) {
		positionInformation[index + 0] = this->vertices[i].position.x;
		positionInformation[index + 1] = this->vertices[i].position.y;
		positionInformation[index + 2] = this->vertices[i].position.z;
		index += 3;
	}

	return positionInformation;
}

float* Model::GetVertexNormalInformation() {
	float* normalInformation = new float[GetVertexCount() * 3];

	int index = 0;
	for (int i = 0; i < GetVertexCount(); i++) {
		normalInformation[index + 0] = this->vertices[i].normal.x;
		normalInformation[index + 1] = this->vertices[i].normal.y;
		normalInformation[index + 2] = this->vertices[i].normal.z;
		index += 3;
	}
	return normalInformation;
}

float* Model::GetVertexColorInformation() {
	float* colorInformation = new float[GetVertexCount() * 3];

	int index = 0;
	for (int i = 0; i < GetVertexCount(); i++) {
		colorInformation[index + 0] = this->vertices[i].color.x;
		colorInformation[index + 1] = this->vertices[i].color.y;
		colorInformation[index + 2] = this->vertices[i].color.z;
		index += 3;
	}

	return colorInformation;
}
