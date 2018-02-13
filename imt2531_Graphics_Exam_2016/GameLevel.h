#pragma once

//Linux
#if defined(__linux__)
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
//Windows
#elif defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
	#include <SDKDDKVer.h>

	#include <SDL.h>
	#include <SDL_image.h>
#endif

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "Mesh.h"
#include "GameObject.h"
#include "Car.h"
#include "Mirror.h"
#include "MeshHandler.h"
#include "ParticleSystem.h"

class GameLevel
{
public:
	GameLevel();
	GameLevel(std::string filename);
	~GameLevel();

	Mesh* loadMesh(std::string filename, bool texture); // car obj read from filename
	void loadTrees(std::string filename); // load the trees from a collected file

	unsigned int setTreePos(int treeType, float x, float z); // add a tree placement to the vector of trees in a location

	void toggleSkyBox();
	
	Car* getCar() { return car; };
	Mirror* getMirror() { return mirror; }
	GameObject* getGround() { return ground; }
	GameObject* getSkyBox() { return skyBox; }
	ParticleSystem* getSnow() { return snow; }

	std::vector<GameObject>* getTrees() { return &trees; }
private:
	Mirror* mirror;
	Car* car;
	GameObject* ground;
	GameObject* skyBox;
	ParticleSystem* snow;
	std::vector<GameObject> trees;
	std::vector<Mesh*> treeTypes;
};

