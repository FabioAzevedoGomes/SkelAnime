#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Window.hpp"
#include "Renderer.hpp"

Assimp::Importer importer;

void LogErrorAndQuit(const char* message, int error_code) {
	std::cerr << message << std::endl;
	exit(error_code);
}

const aiScene* LoadModelFromFile(const std::string filename) {

	const aiScene* scene = importer.ReadFile(filename, aiProcess_GenNormals);

	if (!scene) {
		LogErrorAndQuit("Could not load model", 1);
	}

	return scene;
}

int main(int argc, char** argv) {
	if (argc < 1) {
		LogErrorAndQuit("Please drag file into application", 1);
	}

	Window window;
	Scene* scene = new Scene(LoadModelFromFile(argv[1]));
	Renderer::Setup(scene);

	while (!window.shouldClose()) {
		Renderer::RenderScene(scene);
		window.refresh();
	}

	return 0;
}
