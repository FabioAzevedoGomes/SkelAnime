#include <iostream>

#include "Camera.hpp"

Camera::Camera() {
	this->view = CAMERA_DEFAULT_VIEW;
	this->up = CAMERA_DEFAULT_UP;
	this->position = CAMERA_DEFAULT_POSITION;
	this->sensitivity = CAMERA_DEFAULT_SENSITIVITY;
	this->theta = CAMERA_DEFAULT_THTETA;
	this->phi = CAMERA_DEFAULT_PHI;
	this->speed = CAMERA_DEFAULT_SPEED;
}

Camera::~Camera() {}

void Camera::MoveZ(float mod) {
	this->position += mod * this->speed * this->view;
}

void Camera::MoveX(float mod) {
	this->position += mod
		* this->speed
		* glm::vec4(glm::normalize(glm::cross(glm::vec3(this->view), glm::vec3(this->up))), 0.0f);
}

glm::mat4 Camera::GetProjectionMatrix() {
	return glm::perspective(FIELD_OF_VIEW, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);
}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(glm::vec3(this->position), glm::vec3(this->position + this->view), glm::vec3(this->up));
}

void Camera::Rotate(double deltaTheta, double deltaPhi) {
	this->theta -= this->sensitivity * deltaTheta;
	this->phi -= this->sensitivity * deltaPhi;

	this->phi = std::max(std::min(CAMERA_MAX_PHI, this->phi), -CAMERA_MAX_PHI);

	this->view = glm::normalize(glm::rotate(glm::mat4(1.0f), (float)this->theta, glm::vec3(0.0, 1.0, 0.0))
		* glm::rotate(glm::mat4(1.0f), (float)this->phi, glm::vec3(1.0, 0.0, 0.0))
		* CAMERA_DEFAULT_VIEW);

	this->up = glm::normalize(glm::rotate(glm::mat4(1.0f), (float)this->theta, glm::vec3(0.0, 1.0, 0.0))
		* glm::rotate(glm::mat4(1.0f), (float)this->phi, glm::vec3(1.0, 0.0, 0.0))
		* CAMERA_DEFAULT_UP);
}

