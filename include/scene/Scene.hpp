#ifndef SKELANIME_SCENE_HPP
#define SKELANIME_SCENE_HPP

#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <map>
#include <string>

#include "Model.hpp"
#include "Camera.hpp"
#include "Animation.hpp"

class Scene {
private:
	Animation* animation;
	Model* model;
	Camera* camera;

protected:
	void AddMeshVertices(aiMesh* mesh, const aiScene* model, std::vector<Vertex>& vertices, Skeleton& skeleton);
	void ProcessSkeletonNodesRecursively(Skeleton* skeleton, aiNode* node, std::map<std::string, int>& nameToPositionMap);
	void ProcessModelNodesRecursively(aiNode* node, const aiScene* model, std::vector<Vertex>& vertices, Skeleton& skeleton);

public:
	Scene(std::pair<const aiScene*, const aiScene*> skeletonModelPair);
	virtual ~Scene();

	Animation* GetAnimation();
	Model* GetModel();
	Camera* GetCamera();
	long GetVertexCount();

	float* GetVertexPositionInformation();
	int* GetVertexBoneIdsInformation();
	float* GetVertexNormalInformation();
	float* GetVertexColorInformation();

	void LoadPoseFromFile(const std::string& path);
	void SavePoseToFile(const std::string& path);

	void LoadAnimationFromFile(const std::string& path);
	void SaveAnimationToFile(const std::string& path);
};

#endif // SKLEANIME_SCENE_HPP
