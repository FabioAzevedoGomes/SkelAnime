#ifndef SKELANIME_CAMERA_HPP
#define SKELANIME_CAMERA_HPP

#include <glm/vec4.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>

#include "defaults.hpp"

class Camera {
private:
	glm::vec4 view, up;
	glm::vec4 position;
	float sensitivity, speed;
	double theta, phi;
public:
	Camera();
	virtual ~Camera();

	void MoveX(float mod);
	void MoveZ(float mod);
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();

	void Rotate(double theta, double phi);
};

#endif // SKELANIME_CAMERA_HPP
