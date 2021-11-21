
#include <glm/ext/quaternion_common.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "Animation.hpp"

void matdecompose(glm::mat4& m, glm::vec3& scale, glm::quat& rotation, glm::vec3& translation) {
	translation = glm::vec3(m[3]);
	scale = glm::vec3(glm::l2Norm(glm::vec3(m[0])), glm::l2Norm(glm::vec3(m[1])), glm::l2Norm(glm::vec3(m[2])));
	glm::mat3 mrot;
	mrot[0] = glm::vec3(m[0]) / scale[0];
	mrot[1] = glm::vec3(m[1]) / scale[1];
	mrot[2] = glm::vec3(m[2]) / scale[2];
	rotation = glm::quat(mrot);
}

glm::mat4 matcompose(glm::vec3& scale, glm::quat& rotation, glm::vec3& translation) {
	glm::mat4 result = glm::mat4_cast(rotation);
	result[0] *= scale.x;
	result[1] *= scale.y;
	result[2] *= scale.z;
	result[3] = glm::vec4(translation.x, translation.y, translation.z, 1.0f);
	return result;
}

glm::mat4 GetInterpolatedTransform(glm::mat4 m1, glm::mat4 m2, float alpha) {
	glm::vec3 scale1, scale2;
	glm::vec3 translation1, translation2;
	glm::quat q1, q2;

	glm::vec3 skew;
	glm::vec4 perspective;

	matdecompose(m1, scale1, q1, translation1);
	matdecompose(m2, scale2, q2, translation2);

	glm::conjugate(q1);
	glm::conjugate(q2);

	auto s = scale1 + alpha * (scale2 - scale1);
	auto q = glm::mix(q1, q2, alpha);
	auto t = translation1 + alpha * (translation2 - translation1);

	return matcompose(s, q, t);
}

void Animation::SetInterpolatedPose(Skeleton* s1, Skeleton* s2, float alpha, Skeleton* target)
{
	auto& bones1 = s1->GetBones();
	auto& bones2 = s2->GetBones();
	auto& targetBones = target->GetBones();

	for (int i = 0; i < bones1.size(); i++) {
		auto& m1 = bones1[i].transformation;
		auto& m2 = bones2[i].transformation;
		targetBones[i].transformation = GetInterpolatedTransform(m1,m2,alpha);
	}
}

void Animation::AddKeyframe(std::string name, float time, Skeleton s) {
	keyframes[name].pose = s;
	keyframes[name].time = time;
}

void Animation::RemovePose(std::string name) {
	keyframes.erase(name);
}

void Animation::EditPose(std::string name, Skeleton newSkeleton) {
	keyframes[name].pose = newSkeleton;
}

void Animation::EditTime(std::string name, float time) {
	keyframes[name].time = time;
}

void Animation::SetPose(float tCurrent, float tMin, float tMax, Skeleton* target) {

	Skeleton *p1 = nullptr, *p2 = nullptr;

	float t1 = tMin - 1;
	float t2 = tMax + 1;

	for( auto& p : keyframes) {
		float t = p.second.time;
		if( t1 < t && t < tCurrent) {
			t1 = t;
			p1 = &(p.second.pose);
		}
		else
		if (tCurrent < t && t < t2) {
			t2 = t;
			p2 = &(p.second.pose);
		}
	}

	if (p1 == nullptr && p2 == nullptr) {
		return;
	}

	if (p1 == nullptr) {
		auto& bones = p2->GetBones();
		for (int i = 0; i < bones.size(); i++)
			target->GetBones()[i].transformation = bones[i].transformation;
		return;
	}
		
	if (p2 == nullptr) {
		auto& bones = p1->GetBones();
		for (int i = 0; i < bones.size(); i++)
			target->GetBones()[i].transformation = bones[i].transformation;
		return;
	}

	float alpha = (tCurrent - t1) / (t2 - t1);
	SetInterpolatedPose(p1, p2, alpha, target);
}