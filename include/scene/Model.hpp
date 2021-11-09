#ifndef SKELANIME_MODEL_HPP
#define SKELANIME_MODEL_HPP

#include "Skeleton.hpp"
#include "Vertex.hpp"

class Model {
private:
	Skeleton restSkeleton;
	Skeleton skeleton;
	std::vector<Vertex> vertices;

public:
	Model(Skeleton skeleton, std::vector<Vertex> vertices);
	virtual ~Model();

	void PrintSkeleton();
	void PrintVertices();

	Skeleton* GetSkeleton();
	long GetVertexCount();
	float* GetVertexPositionInformation();
	float* GetVertexNormalInformation();
	float* GetVertexColorInformation();
};

#endif // SKELANIME_MODEL_HPP
