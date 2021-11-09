#include "Window.hpp"

Window::Window(const char* title, int width, int height) {
	glfwInit();
	this->window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);
	gl3wInit();

	this->width = width;
	this->height = height;
	this->activeScene = nullptr;
	this->previousCursorPosition = glm::vec2(0.0f, 0.0f);
	this->captureMouse = false;
	memset(pressed, false, GLFW_KEY_MENU);

	auto resizeCallbackFunction = [](GLFWwindow* window, int width, int height) {
		Window* myWindow =
			reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		myWindow->resizeCallback(width, height);
	};

	auto cursorPosCallbackFunction = [](GLFWwindow* window, double posx, double posy) {
		Window* myWindow =
			reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		myWindow->cursorPosCallback(posx, posy);
	};

	auto keyCallbackFunction = [](GLFWwindow* window, int key, int scancode, int action, int mod) {
		Window* myWindow =
			reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		myWindow->keyCallback(key, scancode, action, mod);
	};

	glfwSetWindowUserPointer(window, reinterpret_cast<void*>(this));

	glfwSetWindowSizeCallback(window, resizeCallbackFunction);
	glfwSetCursorPosCallback(window, cursorPosCallbackFunction);
	glfwSetKeyCallback(window, keyCallbackFunction);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

Window::~Window() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

bool Window::ShouldClose() { return glfwWindowShouldClose(window); }

GLFWwindow* Window::GetWindow() {
	return this->window;
}

void Window::Refresh() {
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Window::RenderScene(Scene* scene) {
	this->activeScene = scene;
	Renderer::RenderScene(scene);
}
