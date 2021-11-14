#include <iostream>
#include <fstream>
#include <iomanip>

#include <glm/gtx/closest_point.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <assimp/Exporter.hpp>

#include "Scene.hpp"

#define BIG_NUMBER 9999999.0f

Assimp::Exporter skeletonExporter;

void prettyPrintMatrix(glm::mat4 matrix) {

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << std::setw(7) << std::fixed << std::setprecision(7) << matrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

glm::vec3 getClosestPoint(glm::vec3 point, glm::vec3 segmentStart, glm::vec3 segmentEnd) {
	return glm::closestPointOnLine(point, segmentStart, segmentEnd);
}

int getClosestBone(Skeleton& skeleton, Vertex& vertex) {
	int closest_bone = -1;
	float closest_dist = BIG_NUMBER;

	glm::vec3 translation1, translation2, skew, scale;
	glm::vec4 perspective;
	glm::quat rotation;
	for (int i = 0; i < skeleton.GetNumberOfBones(); i++) {
		float dist = BIG_NUMBER;
		glm::decompose(skeleton.GetBones()[i].cumulativeTransformation, scale, rotation, translation1, skew, perspective);

		glm::vec3 closestPoint = translation1;

		if (skeleton.GetBones()[i].parent != -1) {
			glm::decompose(skeleton.GetBones()[skeleton.GetBones()[i].parent].cumulativeTransformation, scale, rotation, translation2, skew, perspective);
			closestPoint = getClosestPoint(vertex.position, translation1, translation2);
			dist = glm::distance(closestPoint, glm::vec3(vertex.position));
		}
		else {
			dist = glm::distance(vertex.position, glm::vec4(translation1, 1.0f));
		}

		if (dist < closest_dist
			&& strcmp(skeleton.GetBones()[i].name.c_str(), "Armature")
			&& strcmp(skeleton.GetBones()[i].name.c_str(), "RootNode")
			) {
			closest_bone = i;
			closest_dist = dist;
		}
	}

	return closest_bone == -1 ? 0 : closest_bone;
}

glm::vec4 computeRelativeVertexPosition(Bone& bone, glm::vec4 vertexPos) {
	return glm::inverse(bone.cumulativeTransformation) * vertexPos;
}

void Scene::AddMeshVertices(aiMesh* mesh, const aiScene* model, std::vector<Vertex>& vertices, Skeleton& skeleton) {
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		vertex.position = glm::vec4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.0f);
		if (mesh->HasNormals()) {
			vertex.normal = glm::vec4(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z, 0.0f);
		}
		vertex.color = glm::vec4(26.0f / 255.0f, 228.0f / 255.0f, 235.0f / 255.0f, 255.0f);

		for (int j = 0; j < MAX_INFLUENCING_BONES; j++) {
			vertex.boneIds[j] = getClosestBone(skeleton, vertex);
			vertex.boneWeights[j] = 1;
			glm::vec4 relativePos = computeRelativeVertexPosition(skeleton.GetBones()[vertex.boneIds[j]], vertex.position);
			vertex.relativePositions[4 * j + 0] = relativePos.x;
			vertex.relativePositions[4 * j + 1] = relativePos.y;
			vertex.relativePositions[4 * j + 2] = relativePos.z;
			vertex.relativePositions[4 * j + 3] = relativePos.w;
		}

		vertices.push_back(vertex);
	}
}

void Scene::ProcessModelNodesRecursively(aiNode* node, const aiScene* model, std::vector<Vertex>& vertices, Skeleton& skeleton) {
	if (node != nullptr) {

		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = model->mMeshes[node->mMeshes[i]];
			AddMeshVertices(mesh, model, vertices, skeleton);
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			ProcessModelNodesRecursively(node->mChildren[i], model, vertices, skeleton);
		}
	}
}

glm::mat4 getGlmMatrixFromAiMatrix(aiMatrix4x4 matrix, std::string name) {

	glm::mat4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result[i][j] = matrix[j][i];
		}
	}

	return result;
}

aiMatrix4x4 getAiMatrixFromGlmMatrix(glm::mat4 matrix) {

	aiMatrix4x4 result;
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
	skeleton->AddBone(parentPosition, getGlmMatrixFromAiMatrix(node->mTransformation, node->mName.C_Str()), node->mName.C_Str());
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
	ProcessModelNodesRecursively(skeletonModelPair.second->mRootNode, skeletonModelPair.second, vertices, skeleton);

	this->model = new Model(skeleton, vertices);
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
	return this->model->GetVertexRelativePositionInformation();
}

float* Scene::GetVertexNormalInformation() {
	return this->model->GetVertexNormalInformation();
}

float* Scene::GetVertexColorInformation() {
	return this->model->GetVertexColorInformation();
}

void Scene::LoadPoseFromFile(const std::string& path)
{
	Skeleton s = customImport(path);
	model->LoadSkeleton(&s);
}

aiNode* RecreateSkeletonNodeRecursively(Skeleton* skeleton, int currentIndex, aiNode * parent)
{
	auto& bone = skeleton->GetBones()[currentIndex];
	
	aiNode* current = new aiNode();
	current->mName = bone.name;
	current->mParent = parent;
	current->mTransformation = getAiMatrixFromGlmMatrix(bone.transformation);

	std::vector<aiNode*> children( bone.children.size() );

	for (int i=0; i<bone.children.size(); i++)
	{
		children[i] = RecreateSkeletonNodeRecursively(skeleton, bone.children[i], current);
	}

	current->addChildren(children.size(), children.data());

	return current;
}

void Scene::SavePoseToFile(const std::string& path)
{
	/*aiNode* skeletonNode = RecreateSkeletonNodeRecursively(model->GetSkeleton(), 0, nullptr);

	aiScene scene;
	scene.mRootNode = skeletonNode;

	using namespace std::literals;

	skeletonExporter.Export(&scene, "fbx"s, path);
	*/

	customExport(*(model->GetSkeleton()), path);
}