#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Window.hpp"
#include "Renderer.hpp"

Assimp::Importer modelImporter;
Assimp::Importer skeletonImporter;

void LogErrorAndQuit(const char* message, int error_code) {
	std::cerr << message << std::endl;
	exit(error_code);
}

std::pair<const aiScene*, const aiScene*> LoadModelsFromFiles(const std::string skeletonFilename, const std::string modelFilename) {

	const aiScene* skeleton = skeletonImporter.ReadFile(skeletonFilename, aiProcess_GenNormals);
	const aiScene* model = modelImporter.ReadFile(modelFilename, aiProcess_GenNormals);

	if (!model) {
		LogErrorAndQuit("Could not load model", 1);
	}

	if (!skeleton) {
		LogErrorAndQuit("Could not load skeleton", 1);
	}

	return { skeleton, model };
}

int main(int argc, char** argv) {
	if (argc < 2) {
		LogErrorAndQuit("Please input skeleton and model files", 1);
	}

	Window window;
	Scene* scene = new Scene(LoadModelsFromFiles(argv[1], argv[2]));
	Renderer::Setup(scene);

	while (!window.ShouldClose()) {
		window.RenderScene(scene);
		window.Refresh();
	}

	return 0;
}
