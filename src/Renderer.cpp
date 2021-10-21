#include <iostream>
#include "Renderer.hpp"

#define BUFFER_OFFSET(a) ((void *)(a))

GLuint Renderer::modelUniformId;
GLuint Renderer::viewUniformId;
GLuint Renderer::projUniformId;
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
}

void Renderer::SetupBuffers(Scene* scene) {
	glGenVertexArrays(VAO_Count, vaos);
	glBindVertexArray(vaos[Model_VAO]);
	glCreateBuffers(Buffer_Count, buffers);

	void* position = scene->GetVertexPositionInformation();
	void* normal = scene->GetVertexNormalInformation();
	void* color = scene->GetVertexColorInformation();

	// Position
	glBindBuffer(GL_ARRAY_BUFFER, buffers[Vertex_Position_Buffer]);
	glBufferStorage(GL_ARRAY_BUFFER, scene->GetVertexCount() * 3l * sizeof(float), position, GL_DYNAMIC_STORAGE_BIT);
	glVertexAttribPointer(vertexPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vertexPosition);

	// Colors
	glBindBuffer(GL_ARRAY_BUFFER, buffers[Vertex_Color_Buffer]);
	glBufferStorage(GL_ARRAY_BUFFER, scene->GetVertexCount() * 3l * sizeof(float), color, GL_DYNAMIC_STORAGE_BIT);
	glVertexAttribPointer(vertexColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vertexColor);

	// Normals
	glBindBuffer(GL_ARRAY_BUFFER, buffers[Vertex_Normal_Buffer]);
	glBufferStorage(GL_ARRAY_BUFFER, scene->GetVertexCount() * 3l * sizeof(float), normal, GL_DYNAMIC_STORAGE_BIT);
	glVertexAttribPointer(vertexNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vertexNormal);

	free(position);
	free(normal);
	free(color);
}

void Renderer::Setup(Scene* scene) {
	SetupShaders();
	SetupBuffers(scene);
};

void Renderer::RenderScene(Scene* scene) {

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec4(0.0f)));

	glUniformMatrix4fv(modelUniformId, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
	glUniformMatrix4fv(projUniformId, 1, GL_FALSE, glm::value_ptr(scene->GetCamera()->GetProjectionMatrix()));
	glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(scene->GetCamera()->GetViewMatrix()));

	glBindVertexArray(vaos[Model_VAO]);
	glDrawArrays(GL_TRIANGLES, 0, scene->GetVertexCount());
	glBindVertexArray(0);

};
