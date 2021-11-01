#ifndef SKELANIME_PROPERTY_GUI
#define SKELANIME_PROPERTY_GUI

#include <GL3/gl3.h>
#include <GL3/gl3w.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ImGuiFileBrowser.h"

#include "Scene.hpp"

class PropertyGUI {
private:
	imgui_addons::ImGuiFileBrowser fileDialog;
	Scene* scene;

public:
	PropertyGUI(GLFWwindow* drawnToWindow, Scene* scene);
	virtual ~PropertyGUI();

	void Draw();
};

#endif // SKELANIME_PROPERTY_GUI
