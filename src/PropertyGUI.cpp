#include "PropertyGUI.hpp"


#include <glm/gtc/quaternion.hpp>

#include <iostream>
#include <sstream>
#include <iomanip>

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
}

PropertyGUI::~PropertyGUI() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void PropertyGUI::DrawBonePropertyWindow(std::vector<Bone>& bones, int index) {
	bool open = true;

	ImGui::Begin(bones[index].name.c_str(), &open, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Current pose");
	ImGui::BeginChildFrame(1, ImVec2(200, 100), 0);
	ImGui::Spacing();
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
	ImGui::Begin("Armature", &p_open, ImGuiWindowFlags_AlwaysAutoResize);
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
	ImGui::BulletText("Lock/unlock mouse with \'1\'");
	ImGui::BulletText("Quit application with \'ESC\' or by closing the window");
	ImGui::End();

	shouldDisplay[HELP] = p_open;
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

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
