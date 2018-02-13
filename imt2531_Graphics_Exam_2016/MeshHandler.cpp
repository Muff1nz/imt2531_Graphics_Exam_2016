#include "MeshHandler.h"

Mesh* MeshHandler::loadMesh(std::string modelPath, std::string name, bool texture, glm::vec2 shapeRange) {
	if (meshes.find(name) != meshes.end()) {	//no point in loading the same mesh twice
		printf("There's already a mesh named %s, use getMesh().\n", name);
		return getMesh(name);
	}

	meshes[name] = new Mesh(modelPath, texture, shapeRange);
	return meshes[name];
}