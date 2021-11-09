#include <iostream>

#include "Scene.hpp"

void Scene::AddMeshVertices(aiMesh* mesh, const aiScene* model, std::vector<Vertex>& vertices) {
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		vertex.position = glm::vec4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.0f);
		if (mesh->HasNormals()) {
			vertex.normal = glm::vec4(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z, 0.0f);
		}
		vertex.color = glm::vec4(26.0f / 255.0f, 228.0f / 255.0f, 235.0f / 255.0f, 255.0f);
		for (int j = 0; j < MAX_INFLUENCING_BONES; j++) {
			vertex.boneIds[j] = -1;
			vertex.boneWeights[j] = 0;
		}
		vertices.push_back(vertex);
	}
}

void Scene::ProcessModelNodesRecursively(aiNode* node, const aiScene* model, std::vector<Vertex>& vertices) {
	if (node != nullptr) {

		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = model->mMeshes[node->mMeshes[i]];
			AddMeshVertices(mesh, model, vertices);
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			ProcessModelNodesRecursively(node->mChildren[i], model, vertices);
		}
	}
}

glm::mat4 getGlmMatrixFromAiMatrix(aiMatrix4x4 matrix) {

	glm::mat4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result[i][j] = matrix[j][i];
		}
	}
	return result;
}

void Scene::ProcessSkeletonNodesRecursively(Skeleton* skeleton, aiNode* node, std::map<std::string, int>& nameToIndexMap) {

	int parentPosition = -1;
	if (node->mParent != nullptr) {
		parentPosition = nameToIndexMap[node->mParent->mName.C_Str()];
	}
	skeleton->AddBone(parentPosition, getGlmMatrixFromAiMatrix(node->mTransformation), node->mName.C_Str());
	nameToIndexMap.insert({ (std::string)node->mName.C_Str(), skeleton->GetNumberOfBones() - 1 });

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		ProcessSkeletonNodesRecursively(skeleton, node->mChildren[i], nameToIndexMap);
	}
}

Scene::Scene(std::pair<const aiScene*, const aiScene*> skeletonModelPair) {
	this->camera = new Camera();
	Skeleton skeleton;
	std::vector<Vertex> vertices;

	ProcessSkeletonNodesRecursively(&skeleton, skeletonModelPair.first->mRootNode, std::map<std::string, int>());
	ProcessModelNodesRecursively(skeletonModelPair.second->mRootNode, skeletonModelPair.second, vertices);

	this->model = new Model(skeleton, vertices);
	//this->model->PrintSkeleton();
	//this->model->PrintVertices();
}

Scene::~Scene() {
}

Model* Scene::GetModel() {
	return this->model;
}

Camera* Scene::GetCamera() {
	return this->camera;
}

long Scene::GetVertexCount() {
	return this->model->GetVertexCount();
}

float* Scene::GetVertexPositionInformation() {
	return this->model->GetVertexPositionInformation();
}

float* Scene::GetVertexNormalInformation() {
	return this->model->GetVertexNormalInformation();
}

float* Scene::GetVertexColorInformation() {
	return this->model->GetVertexColorInformation();
}
