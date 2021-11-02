#include "PropertyGUI.hpp"

#include <iostream>

PropertyGUI::PropertyGUI(GLFWwindow* drawnToWindow, Scene* scene) {

	this->scene = scene;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui::StyleColorsClassic();
	ImGui_ImplGlfw_InitForOpenGL(drawnToWindow, true);
	ImGui_ImplOpenGL3_Init("#version 400");

	this->displayProperties = std::vector<bool>(scene->GetModel()->GetSkeleton()->GetNumberOfBones(), false);
}

PropertyGUI::~PropertyGUI() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void PropertyGUI::DrawBonePropertyWindow(std::vector<Bone>& bones, int index) {
	bool open = true;

	ImGui::Begin(bones[index].name.c_str(), &open, 0);
	ImGui::Text("Something");
	ImGui::End();

	this->displayProperties[index] = open;
}

void PropertyGUI::RenderBoneTree(std::vector<Bone>& bones, int boneIndex) {
	ImGuiTreeNodeFlags  flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
	static int selection_mask = (1 << 2);
	if (boneIndex >= 0 && boneIndex < bones.size()) {
		Bone bone = bones[boneIndex];

		const bool is_selected = (selection_mask & (1 << boneIndex)) != 0;
		if (is_selected)
			flags |= ImGuiTreeNodeFlags_Selected;

		if (bone.children.size() > 0) {
			bool open = ImGui::TreeNodeEx(bone.name.c_str(), flags);
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
				this->displayProperties[boneIndex] = true;

			if (open) {
				for (auto child = bone.children.begin(); child != bone.children.end(); child++) {
					RenderBoneTree(bones, *child);
				}
				ImGui::TreePop();
			}
		}
		else {
			flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
			bool open = ImGui::TreeNodeEx(bone.name.c_str(), flags);
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
				this->displayProperties[boneIndex] = true;
		}
	}
}

void PropertyGUI::Draw() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	std::vector<Bone>& bones = this->scene->GetModel()->GetSkeleton()->GetBones();

	ImGui::Begin("Properties", NULL, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::TreeNode("Armature")) {
		RenderBoneTree(bones, 0);
		ImGui::TreePop();
	}

	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::End();

	for (int i = 0; i < bones.size(); i++)
		if (this->displayProperties[i])
			DrawBonePropertyWindow(bones, i);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
