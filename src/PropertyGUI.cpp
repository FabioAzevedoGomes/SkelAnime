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

void RenderBoneTree(std::vector<Bone>& bones, int boneIndex) {
	if (boneIndex >= 0 && boneIndex < bones.size()) {
		Bone bone = bones[boneIndex];
		if (bone.children.size() > 0) {
			if (ImGui::TreeNode(bone.name.c_str())) {
				for (auto child = bone.children.begin(); child != bone.children.end(); child++) {
					RenderBoneTree(bones, *child);
				}
				ImGui::TreePop();
			}
		}
		else {
			ImGui::Text(bone.name.c_str());
		}
	}
}

void PropertyGUI::Draw() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Properties", NULL, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::TreeNode("Armature")) {

		std::vector<Bone>& bones = this->scene->GetModel()->GetSkeleton()->GetBones();
		RenderBoneTree(bones, 0);
		ImGui::TreePop();
	}

	ImGui::Spacing();
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
