#ifndef SKELANIME_WINDOW_HPP
#define SKELANIME_WINDOW_HPP

#include <GL3/gl3.h>
#include <GL3/gl3w.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "Scene.hpp"
#include "Renderer.hpp"
#include "defaults.hpp"

class Window {
private:
	int width, height;
	bool pressed[GLFW_KEY_MENU];
	glm::vec2 previousCursorPosition;
	GLFWwindow* window;

	Scene* activeScene;

protected:
	inline void resizeCallback(int newWidth, int newHeight) {
		this->width = newWidth;
		this->height = newHeight;
		glViewport(0, 0, newWidth, newHeight);
	}

	inline void cursorPosCallback(double x, double y) {
		double dx = x - this->previousCursorPosition.x;
		double dy = y - this->previousCursorPosition.y;

		this->previousCursorPosition = glm::vec2(x, y);
		this->activeScene->GetCamera()->Rotate(dx, dy);
	}

	inline void keyCallback(int key, int scancode, int action, int mod) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);

		if (action == GLFW_PRESS || action == GLFW_REPEAT)
			pressed[key] = true;
		else if (action == GLFW_RELEASE)
			pressed[key] = false;

		if (pressed[GLFW_KEY_W]) this->activeScene->GetCamera()->MoveZ(1);
		if (pressed[GLFW_KEY_A]) this->activeScene->GetCamera()->MoveX(-1);
		if (pressed[GLFW_KEY_S]) this->activeScene->GetCamera()->MoveZ(-1);
		if (pressed[GLFW_KEY_D]) this->activeScene->GetCamera()->MoveX(1);
	}

public:
	Window(const char* title = DEFAULT_TITLE, int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT);
	~Window();

	bool ShouldClose();
	void Refresh();
	void RenderScene(Scene* scene);
};

#endif // SKELANIME_WINDOW_HPP
