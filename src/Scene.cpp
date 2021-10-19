#include <iostream>

#include "Scene.hpp"

Mesh Scene::CreateMesh(aiMesh* mesh, const aiScene* model) {
	std::vector<Vertex> meshVertices;
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		if (mesh->HasNormals()) {
			vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		}
		else {
			vertex.normal = glm::vec3(1.0, 0.0, 0.0); // TODO Calculate normal
		}
		vertex.color = glm::vec3(255.0f, 255.0f, 255.0);
		meshVertices.push_back(vertex);
		this->totalVertices++;
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	return Mesh(meshVertices, indices);
}

void Scene::ProcessNodesRecursively(aiNode* node) {

	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = model->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(CreateMesh(mesh, model));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		ProcessNodesRecursively(node->mChildren[i]);
	}
}

Scene::Scene(const aiScene* model) {
	this->model = model;
	this->camera = new Camera();
	this->totalVertices = 0;
	ProcessNodesRecursively(model->mRootNode);
}

Scene::~Scene() {
}

const aiScene* Scene::GetModel() {
	return this->model;
}

Camera* Scene::GetCamera() {
	return this->camera;
}

long Scene::GetVertexCount() {
	return this->totalVertices;
}

float* Scene::GetVertexPositionInformation() {
	float* positionInformation = new float[GetVertexCount() * 3];

	int index = 0;
	for (auto i = this->meshes.begin(); i != this->meshes.end(); i++) {
		for (auto j = i->vertices.begin(); j != i->vertices.end(); j++) {
			positionInformation[index + 0] = j->position.x;
			positionInformation[index + 1] = j->position.y;
			positionInformation[index + 2] = j->position.z;
			//std::cout << "vertex " << index / 3.0f << ": " << positionInformation[index + 0] << ", " << positionInformation[index + 1] << ", " << positionInformation[index + 2] << std::endl;
			index += 3;
		}
	}

	return positionInformation;
}

float* Scene::GetVertexNormalInformation() {
	float* normalInformation = new float[GetVertexCount() * 3];

	int index = 0;
	for (auto i = this->meshes.begin(); i != this->meshes.end(); i++) {
		for (auto j = i->vertices.begin(); j != i->vertices.end(); j++) {
			normalInformation[index + 0] = j->normal.x;
			normalInformation[index + 1] = j->normal.y;
			normalInformation[index + 2] = j->normal.z;
			index += 3;
		}
	}

	return normalInformation;

}

float* Scene::GetVertexColorInformation() {
	float* colorInformation = new float[GetVertexCount() * 3];

	int index = 0;
	for (auto i = this->meshes.begin(); i != this->meshes.end(); i++) {
		for (auto j = i->vertices.begin(); j != i->vertices.end(); j++) {
			colorInformation[index + 0] = j->color.x;
			colorInformation[index + 1] = j->color.y;
			colorInformation[index + 2] = j->color.z;
			index += 3;
		}
	}

	return colorInformation;
}
