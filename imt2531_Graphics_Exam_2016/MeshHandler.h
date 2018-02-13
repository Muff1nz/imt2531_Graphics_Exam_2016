#pragma once

#include <string>
#include <map>

#include "Mesh.h"
#include "MirrorHandler.h"

//Stores a reference to everymesh, so that meshes can be reused easier
class MeshHandler {
public:
	//This ensures that there can only be one MeshHandler object at any given time.
	static MeshHandler& getInstance() {
		static MeshHandler instance;
		return instance;
	}


	//This ensures that there can only be one MeshHandler object at any given time.
	MeshHandler(MeshHandler const& copy) = delete;
	void operator=(MeshHandler const& copy) = delete;

	Mesh* loadMesh(std::string modelPath, std::string name, bool texture = false, glm::vec2 shapeRange = {0, 0});
	Mesh* getMesh(std::string name) { return meshes[name]; };
private:
	std::map<std::string, Mesh*> meshes;
	MeshHandler() {};
};

