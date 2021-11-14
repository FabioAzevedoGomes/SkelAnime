#include "PropertyGUI.hpp"


#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>
#include <sstream>
#include <iomanip>

imgui_addons::ImGuiFileBrowser fileDialog;

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

	this->shouldDisplay = std::map<DisplayableWindows, bool>();
	shouldDisplay[DisplayableWindows::ARMATURE] = false;
	shouldDisplay[DisplayableWindows::HELP] = true;
	shouldDisplay[DisplayableWindows::FILES] = false;
}

PropertyGUI::~PropertyGUI() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void PropertyGUI::DrawBonePropertyWindow(std::vector<Bone>& bones, int index) {
	bool open = true;

	ImGui::Begin(("Properties of " + bones[index].name).c_str(), &open, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Current pose");
	ImGui::BeginChildFrame(2, ImVec2(200, 75), 0);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			ImGui::SameLine();
			std::stringstream ss;
			ss << std::setw(5) << std::fixed << std::setprecision(2) << bones[index].transformation[j][i];
			ImGui::Text(ss.str().c_str());
		}
		ImGui::Spacing();
	}
	ImGui::EndChildFrame();

	glm::vec3 scale, translation, skew;
	glm::vec4 perspective;
	glm::quat rotation;
	glm::decompose(bones[index].transformation, scale, rotation, translation, skew, perspective);
	glm::vec3 eulerRotation = glm::eulerAngles(rotation);

	if (ImGui::CollapsingHeader("Translation", 0)) {
		ImGui::Text(glm::to_string(translation).c_str());
		ImGui::InputFloat("Translation X", &translation.x, 0.01f, 0.1f, "%f", 0);
		ImGui::InputFloat("Translation Y", &translation.y, 0.01f, 0.1f, "%f", 0);
		ImGui::InputFloat("Translation Z", &translation.z, 0.01f, 0.1f, "%f", 0);
		ImGui::Spacing();
	}

	if (ImGui::CollapsingHeader("Rotation", 0)) {
		ImGui::Text(glm::to_string(eulerRotation).c_str());
		ImGui::InputFloat("Rotation X", &eulerRotation.x, 0.01f, 0.1f, "%f", 0);
		ImGui::InputFloat("Rotation Y", &eulerRotation.y, 0.01f, 0.1f, "%f", 0);
		ImGui::InputFloat("Rotation Z", &eulerRotation.z, 0.01f, 0.1f, "%f", 0);
		ImGui::Spacing();
	}

	if (ImGui::CollapsingHeader("Scale", 0)) {
		ImGui::Text(glm::to_string(scale).c_str());
		ImGui::InputFloat("Scale X", &scale.x, 0.01f, 0.1f, "%f", 0);
		ImGui::InputFloat("Scale Y", &scale.y, 0.01f, 0.1f, "%f", 0);
		ImGui::InputFloat("Scale Z", &scale.z, 0.01f, 0.1f, "%f", 0);
		ImGui::Spacing();
	}

	glm::mat4 transformation(1.0f);
	rotation = glm::quat(eulerRotation);

	bones[index].transformation = glm::scale(glm::toMat4(rotation) * transformation, scale);
	bones[index].transformation[3] = glm::vec4(translation, 1.0f); // glm::translate was doing something weird idk

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

void PropertyGUI::DrawArmatureWindow() {
	std::vector<Bone>& bones = this->scene->GetModel()->GetSkeleton()->GetBones();

	bool p_open = true;
	ImGui::Begin("Model Armature", &p_open, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::BeginChildFrame(1, ImVec2(300, 60), 0);
	ImGui::TextWrapped("This window contains the armature's bone hierarchy. Expand the tree below by clicking the arrows or manipulate each bone's properties by clicking it's name.");
	ImGui::EndChildFrame();
	RenderBoneTree(bones, 0);
	ImGui::End();

	for (int i = 0; i < bones.size(); i++)
		if (this->displayProperties[i])
			DrawBonePropertyWindow(bones, i);

	shouldDisplay[ARMATURE] = p_open;
}

void PropertyGUI::DrawHelpWindow() {

	bool p_open = true;
	ImGui::Begin("Help", &p_open, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Controls");
	ImGui::BulletText("Move camera with \'W\', \'A\', \'S\', \'D\'");
	ImGui::BulletText("Lock/unlock mouse with \'F1\'");
	ImGui::BulletText("Quit application with \'ESC\' or by closing the window");
	ImGui::End();

	shouldDisplay[HELP] = p_open;
}

void PropertyGUI::DrawFileWindow() {

	bool p_open = true;
	ImGui::Begin("Save/Load", &p_open, ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::Button("Load Pose")) {
		ImGui::OpenPopup("Open File");
	}

	if (fileDialog.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".fbx"))
		scene->LoadPoseFromFile( fileDialog.selected_path );

	if (ImGui::Button("Save Pose")) {
		ImGui::OpenPopup("Save File");
	}

	if (fileDialog.showFileDialog("Save File", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(700, 310), ".fbx"))
		scene->SavePoseToFile(fileDialog.selected_path);

	ImGui::End();

	shouldDisplay[FILES] = p_open;
}

void PropertyGUI::DrawMenuBar() {
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("View...", true)) {
		bool armature_selected = false;
		ImGui::MenuItem("Armature window", "", &armature_selected, true);
		if (armature_selected) {
			shouldDisplay[ARMATURE] = true;
		}
		ImGui::EndMenu();
	}

	bool help_selected = false;
	ImGui::MenuItem("Help", "h", &help_selected, true);
	if (help_selected) {
		shouldDisplay[HELP] = true;
	}

	bool files_selected = false;
	ImGui::MenuItem("Save/Load", "h", &files_selected, true);
	if (files_selected) {
		shouldDisplay[FILES] = true;
	}

	ImGui::EndMainMenuBar();
}

void PropertyGUI::Draw() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	DrawMenuBar();

	if (shouldDisplay[ARMATURE])
		DrawArmatureWindow();

	if (shouldDisplay[HELP])
		DrawHelpWindow();

	if (shouldDisplay[FILES])
		DrawFileWindow();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
