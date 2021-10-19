#pragma once

#include <glm/vec4.hpp>

#define CAMERA_DEFAULT_POSITION glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
#define CAMERA_DEFAULT_UP glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)
#define CAMERA_DEFAULT_VIEW glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)

#define NEAR_PLANE 0.1f
#define FAR_PLANE 1000.0f
#define ASPECT_RATIO 4.0f / 3.0f
#define FIELD_OF_VIEW 3.14f / 3.0f