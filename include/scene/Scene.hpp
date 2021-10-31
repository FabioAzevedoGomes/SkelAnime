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

class Scene {
private:
	Model* model;
	Camera* camera;

protected:
	void AddMeshVertices(aiMesh* mesh, const aiScene* model, std::vector<Vertex>& vertices);
	void ProcessSkeletonNodesRecursively(Skeleton* skeleton, aiNode* node, std::map<std::string, int>& nameToPositionMap);
	void ProcessModelNodesRecursively(aiNode* node, const aiScene* model, std::vector<Vertex>& vertices);

public:
	Scene(std::pair<const aiScene*, const aiScene*> skeletonModelPair);
	virtual ~Scene();

	Model* GetModel();
	Camera* GetCamera();
	long GetVertexCount();

	float* GetVertexPositionInformation();
	float* GetVertexNormalInformation();
	float* GetVertexColorInformation();

};

#endif // SKLEANIME_SCENE_HPP
