#include "Window.hpp"

Window::Window(const char* title, int width, int height) {
	glfwInit();
	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	glfwMakeContextCurrent(window);
	gl3wInit();

	this->width = width;
	this->height = height;

	auto resizeCallbackFunction = [](GLFWwindow* window, int width, int height) {
		Window* myWindow =
			reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		myWindow->resizeCallback(width, height);
	};

	glfwSetWindowUserPointer(window, reinterpret_cast<void*>(this));
	glfwSetWindowSizeCallback(window, resizeCallbackFunction);
}

Window::~Window() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::resizeCallback(int newWidth, int newHeight) {
	width = newWidth;
	height = newHeight;
}

bool Window::shouldClose() { return glfwWindowShouldClose(window); }

void Window::refresh() {
	glfwSwapBuffers(window);
	glfwPollEvents();
}
