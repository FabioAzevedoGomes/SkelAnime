#include "Camera.hpp"

Camera::Camera() {
	this->view = CAMERA_DEFAULT_VIEW;
	this->up = CAMERA_DEFAULT_UP;
	this->position = CAMERA_DEFAULT_POSITION;
}

Camera::~Camera() {}

void Camera::LookAt(glm::vec4 point) {
	this->view = glm::normalize(point - this->position);
	glm::vec3 right = glm::cross(glm::vec3(this->up), glm::vec3(this->view));
	this->up = glm::vec4(glm::normalize(glm::cross(right, glm::vec3(this->view))), 0.0f);
}

void Camera::MoveTo(glm::vec4 point) {
	this->position = point;
}

glm::mat4 Camera::GetProjectionMatrix() {
	return glm::perspective(FIELD_OF_VIEW, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);
}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(glm::vec3(this->position), glm::vec3(this->position + this->view), glm::vec3(this->up));
}
