#pragma once

#include <vector>
#include "Mirror.h"
#include "MeshHandler.h"

//I made this  class because i want a reference to every mirror 
//	for rendering without digging through a bunch of objects
//	same goes for lightHandler
class MirrorHandler {
public:
	//This ensures that there can only be one MirrorHandler object at any given time.
	static MirrorHandler& getInstance() {
		static MirrorHandler instance;
		return instance;
	}


	//This ensures that there can only be one MirrorHandler object at any given time.
	MirrorHandler(MirrorHandler const& copy) = delete;
	void operator=(MirrorHandler const& copy) = delete;

	Mirror* createMirror(glm::vec3 position = { 0, 0, 0 }, glm::vec3 scale = { 1, 1, 1 }, glm::vec3 rotation = { 0, 0, 0 },
		std::string texture = "");

	Mirror* operator[](int i) { return mirrors[i]; }
	int getSize() { return mirrors.size(); }
private:
	std::vector<Mirror*> mirrors;
	MirrorHandler() {};
};

