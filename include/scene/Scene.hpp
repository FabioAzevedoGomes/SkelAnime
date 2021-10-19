#ifndef SKELANIME_SCENE_HPP
#define SKELANIME_SCENE_HPP

#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <assimp/scene.h>
#include <assimp/mesh.h>

#include "Camera.hpp"
#include "Mesh.hpp"

class Scene {
private:
	const aiScene* model;
	std::vector<Mesh> meshes;
	long totalVertices;
	Camera* camera;

protected:
	Mesh CreateMesh(aiMesh* mesh, const aiScene* model);
	void ProcessNodesRecursively(aiNode* node);

public:
	Scene(const aiScene* model);
	virtual ~Scene();

	const aiScene* GetModel();
	Camera* GetCamera();
	long GetVertexCount();

	float* GetVertexPositionInformation();
	float* GetVertexNormalInformation();
	float* GetVertexColorInformation();
};

#endif // SKLEANIME_SCENE_HPP
