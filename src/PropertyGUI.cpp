#include "PropertyGUI.hpp"

PropertyGUI::PropertyGUI(GLFWwindow* drawnToWindow, Scene* scene) {

	this->scene = scene;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui::StyleColorsClassic();
	ImGui_ImplGlfw_InitForOpenGL(drawnToWindow, true);
	ImGui_ImplOpenGL3_Init("#version 400");
}

PropertyGUI::~PropertyGUI() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void PropertyGUI::Draw() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Properties", NULL, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Spacing();
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
