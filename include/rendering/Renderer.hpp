#ifndef SKELANIME_RENDERER_HPP
#define SKELANIME_RENDERER_HPP

#include <GL3/gl3.h>
#include <GL3/gl3w.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/vec4.hpp>

#include <assimp/scene.h>

#include "Scene.hpp"

enum Attribute_IDs {
	modelVertexPosition,
	modelVertexColor,
	modelVertexNormal,
	modelVertexBoneId
};

enum Buffer_IDs {
	Model_Vertex_Position_Buffer,
	Model_Vertex_Normal_Buffer,
	Model_Vertex_Color_Buffer,
	Model_Vertex_Bone_Id_Buffer,
	Bone_Vertex_Position_Buffer,
	Bone_Vertex_Normal_Buffer,
	Bone_Vertex_Color_Buffer,
	Buffer_Count
};

enum VAO_IDs {
	Model_VAO,
	Bone_VAO,
	VAO_Count
};

#define VERTEX_SHADER "../src/shader_vertex.glsl"
#define FRAGMENT_SHADER "../src/shader_fragment.glsl"

class Renderer {

private:
	static GLuint modelUniformId;
	static GLuint viewUniformId;
	static GLuint projUniformId;
	static GLuint objectUniformId;
	static GLuint cumulativeBoneTransformsUniformId;

	static GLuint buffers[Buffer_Count];
	static GLuint vaos[VAO_Count];

protected:
	static void SetupShaders();
	static void SetupBuffers(Scene*);
public:
	static void Setup(Scene*);
	static void RenderScene(Scene*);
};

#endif // SKELANIME_RENDERER_HPP
