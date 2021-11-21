#ifndef SKELANIME_ANIMATION_HPP
#define SKELANIME_ANIMATION_HPP

#include <map>
#include <list>
#include <vector>

#include "Skeleton.hpp"

struct Keyframe {
	float time;
	Skeleton pose;
};

class Animation
{
	std::map<std::string,Keyframe> keyframes;

	void SetInterpolatedPose(Skeleton* s1, Skeleton* s2, float alpha, Skeleton * target);
public:

	auto& GetKeyframes()
	{
		return keyframes;
	}

	void AddKeyframe(std::string name, float time, Skeleton s);
	void RemovePose(std::string name);

	void EditPose(std::string name, Skeleton newSkeleton);
	void EditTime(std::string name, float time);

	void SetPose(float tCurrent, float tMax, float tMin, Skeleton* target);

};

void matdecompose(glm::mat4& m, glm::vec3& scale, glm::quat& rotation, glm::vec3& translation);
glm::mat4 matcompose(glm::vec3& scale, glm::quat& rotation, glm::vec3& translation);

#endif