#pragma once

#include <GL3/gl3.h>
#include <GL3/gl3w.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "defaults.hpp"

class Window {
private:
	int width, height;
	GLFWwindow* window;

protected:
	void resizeCallback(int newWidth, int newHeight);

public:
	Window(const char* title = DEFAULT_TITLE, int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT);
	~Window();

	bool shouldClose();
	void refresh();
};
