#ifndef SKELANIME_CAMERA_HPP
#define SKELANIME_CAMERA_HPP

#include <glm/vec4.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "defaults.hpp"

class Camera {
private:
	glm::vec4 view, up;
	glm::vec4 position;
public:
	Camera();
	virtual ~Camera();

	void LookAt(glm::vec4 point);
	void MoveTo(glm::vec4 point);
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();
};

#endif // SKELANIME_CAMERA_HPP
