#include <iostream>
#include "Renderer.hpp"

#define BUFFER_OFFSET(a) ((void *)(a))

GLuint Renderer::modelUniformId;
GLuint Renderer::viewUniformId;
GLuint Renderer::projUniformId;
GLuint Renderer::objectUniformId;
GLuint Renderer::buffers[Buffer_Count];
GLuint Renderer::vaos[VAO_Count];

static const GLchar* ReadShader(const char* filename) {

	FILE* shader = fopen(filename, "rb");

	if (!shader) {
		std::cerr << "Unable to open shader file " << filename << std::endl;
		exit(1);
	}

	fseek(shader, 0, SEEK_END);
	int len = ftell(shader);
	fseek(shader, 0, SEEK_SET);

	GLchar* source = new GLchar[len + 1];
	fread(source, 1, len, shader);
	source[len] = 0;

	fclose(shader);

	return const_cast<const GLchar*>(source);
}

void Renderer::SetupShaders() {
	GLuint program = glCreateProgram();
	GLint compiled;

	const GLchar* vertexShaderSource = ReadShader(VERTEX_SHADER);
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	delete[] vertexShaderSource;
	glCompileShader(vertexShader);
	glAttachShader(program, vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLsizei len;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &len);
		GLchar* log = new GLchar[len + 1];
		glGetShaderInfoLog(vertexShader, len, &len, log);
		std::cerr << "Vertex shader compilation failed: " << log << std::endl;
		delete[] log;
		exit(1);
	}

	const GLchar* fragmentShaderSource = ReadShader(FRAGMENT_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	delete[] fragmentShaderSource;
	glCompileShader(fragmentShader);
	glAttachShader(program, fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLsizei len;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &len);
		GLchar* log = new GLchar[len + 1];
		glGetShaderInfoLog(fragmentShader, len, &len, log);
		std::cerr << "Fragment shader compilation failed: " << log << std::endl;
		delete[] log;
		exit(1);
	}

	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLsizei len;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = new GLchar[len + 1];
		glGetProgramInfoLog(program, len, &len, log);
		std::cerr << "Shader linking failed: " << log << std::endl;
		delete[] log;

		exit(1);
	}

	glUseProgram(program);

	Renderer::modelUniformId = glGetUniformLocation(program, "model");
	Renderer::viewUniformId = glGetUniformLocation(program, "view");
	Renderer::projUniformId = glGetUniformLocation(program, "proj");
	Renderer::objectUniformId = glGetUniformLocation(program, "object");
}

bool once = false;

void Renderer::SetupBuffers(Scene* scene) {
	glGenVertexArrays(VAO_Count, vaos);
	glBindVertexArray(vaos[Model_VAO]);
	glCreateBuffers(Buffer_Count, buffers);

	void* modelPosition = scene->GetVertexPositionInformation();
	void* modelNormal = scene->GetVertexNormalInformation();
	void* modelColor = scene->GetVertexColorInformation();
	Skeleton* modelSkeleton = scene->GetModel()->GetSkeleton();
	float* bonePosition = modelSkeleton->GetVertexPositionInformation();
	void* boneNormal = modelSkeleton->GetVertexNormalInformation();
	void* boneColor = modelSkeleton->GetVertexColorInformation();

	// Model Position
	glBindBuffer(GL_ARRAY_BUFFER, buffers[Model_Vertex_Position_Buffer]);
	glBufferStorage(GL_ARRAY_BUFFER, scene->GetVertexCount() * 3l * sizeof(float), modelPosition, GL_DYNAMIC_STORAGE_BIT);
	glVertexAttribPointer(modelVertexPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(modelVertexPosition);

	// Model Colors
	glBindBuffer(GL_ARRAY_BUFFER, buffers[Model_Vertex_Color_Buffer]);
	glBufferStorage(GL_ARRAY_BUFFER, scene->GetVertexCount() * 3l * sizeof(float), modelColor, GL_DYNAMIC_STORAGE_BIT);
	glVertexAttribPointer(modelVertexColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(modelVertexColor);

	// Model Normals
	glBindBuffer(GL_ARRAY_BUFFER, buffers[Model_Vertex_Normal_Buffer]);
	glBufferStorage(GL_ARRAY_BUFFER, scene->GetVertexCount() * 3l * sizeof(float), modelNormal, GL_DYNAMIC_STORAGE_BIT);
	glVertexAttribPointer(modelVertexNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(modelVertexNormal);

	glBindVertexArray(vaos[Bone_VAO]);

	// Bone Position
	glBindBuffer(GL_ARRAY_BUFFER, buffers[Bone_Vertex_Position_Buffer]);
	glBufferStorage(GL_ARRAY_BUFFER, modelSkeleton->GetNumberOfBones() * 6l * sizeof(float), bonePosition, GL_DYNAMIC_STORAGE_BIT);
	glVertexAttribPointer(modelVertexPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(modelVertexPosition);

	// Bone Colors
	glBindBuffer(GL_ARRAY_BUFFER, buffers[Bone_Vertex_Color_Buffer]);
	glBufferStorage(GL_ARRAY_BUFFER, modelSkeleton->GetNumberOfBones() * 6l * sizeof(float), boneColor, GL_DYNAMIC_STORAGE_BIT);
	glVertexAttribPointer(modelVertexColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(modelVertexColor);

	// Bone Normals
	glBindBuffer(GL_ARRAY_BUFFER, buffers[Bone_Vertex_Position_Buffer]);
	glBufferStorage(GL_ARRAY_BUFFER, modelSkeleton->GetNumberOfBones() * 6l * sizeof(float), boneNormal, GL_DYNAMIC_STORAGE_BIT);
	glVertexAttribPointer(modelVertexNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(modelVertexNormal);

	if (once) {
		std::cout << "Pos: " << std::endl;
		for (int i = 0; i < modelSkeleton->GetNumberOfBones() * 6; i += 6) {
			std::cout << "Bone " << modelSkeleton->GetBones()[i / 6].name << std::endl;
			std::cout << bonePosition[i] << ", " << bonePosition[i + 1] << ", " << bonePosition[i + 2] << std::endl;
			std::cout << bonePosition[i + 3] << ", " << bonePosition[i + 4] << ", " << bonePosition[i + 5] << std::endl;
			std::cout << std::endl;
		}
		once = false;
		std::cout << "Total bones: " << modelSkeleton->GetNumberOfBones();
	}

	free(modelPosition);
	free(modelNormal);
	free(modelColor);
	free(bonePosition);
	free(boneNormal);
	free(boneColor);
}

void Renderer::Setup(Scene* scene) {
	SetupShaders();
	SetupBuffers(scene);
};

void Renderer::RenderScene(Scene* scene) {

	SetupBuffers(scene);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec4(0.0f)));

	glUniformMatrix4fv(modelUniformId, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
	glUniformMatrix4fv(projUniformId, 1, GL_FALSE, glm::value_ptr(scene->GetCamera()->GetProjectionMatrix()));
	glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(scene->GetCamera()->GetViewMatrix()));

	glUniform1i(objectUniformId, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(vaos[Model_VAO]);
	glDrawArrays(GL_TRIANGLES, 0, scene->GetVertexCount());
	glBindVertexArray(0);

	glUniform1i(objectUniformId, 1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(vaos[Bone_VAO]);
	glLineWidth((GLfloat)2.0f);
	glDrawArrays(GL_LINES, 0, 2 * scene->GetModel()->GetRestSkeleton()->GetNumberOfBones());
	glBindVertexArray(0);
};
