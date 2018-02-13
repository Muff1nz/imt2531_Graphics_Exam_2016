#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "GameLevel.h"
#include <iostream>

GameLevel::GameLevel(){
}
/*
** Reads the game level from a filename as a string.  It will try to read files from the same location
*/
GameLevel::GameLevel(std::string filename) {
	std::ifstream inputFile(filename);
	std::stringstream lineStream;
	std::string inputLine = "";
	Mesh* temp;
	int type;
	float x, y;
	// Rest of file handling here.
	while (!inputFile.eof()) {
		getline(inputFile, inputLine);
		if (inputLine.compare(0, 2, "c ") == 0) {
			temp = MeshHandler::getInstance().loadMesh(inputLine.substr(2), "car", false);
			car = new Car(temp, { 0, 2.5f, 0 });

		}

		if (inputLine.compare(0, 2, "g ") == 0) {
			temp = MeshHandler::getInstance().loadMesh(inputLine.substr(2), "plane", true);
			ground = new GameObject(temp, { 0, 0, 0 }, { 100, 1, 100 }, { 0, 0, 0 }, "track");
		}

		if (inputLine.compare(0, 2, "s ") == 0) {
			temp = MeshHandler::getInstance().loadMesh(inputLine.substr(2), "skybox", true);
			skyBox = new GameObject(temp, { 0, 0, 0 }, { 100, 100, 100 }, { 0, 0, 0 }, "skyBoxDay");
		}

		if (inputLine.compare(0, 2, "t ") == 0)
			loadTrees(inputLine.substr(2));

		if (inputLine.compare(0, 2, "tp") == 0) {
			sscanf(inputLine.c_str(), "tp %d, %f, %f", &type, &x, &y);
			setTreePos(type, x, y);
		}
	}
	//Car cannot init before plane
	car->init();
	snow = new ParticleSystem();
}

GameLevel::~GameLevel()
{
}


Mesh * GameLevel::loadMesh(std::string filename, bool texture){	
	Mesh* mesh = new Mesh(filename, texture);
	return mesh;
}

/*
* builds the treeTypes vector
*/
void GameLevel::loadTrees(std::string filename){
	glm::vec2 range(17, 19);

	for (int i = 0; i < 9; i++) {
		if (i == 5) range = glm::vec2(6, 7);
		else if (i == 6) range = glm::vec2(4, 6);
		else range -= 2;
		treeTypes.push_back(new Mesh(filename, false, range));
		
	}
}

unsigned int GameLevel::setTreePos(int treeType, float x, float z)
{
	trees.push_back(GameObject(treeTypes[treeType], { x, 0, z }));
	return trees.size();

}

void GameLevel::toggleSkyBox() {
	skyBox->setTexture((skyBox->getTexture() == "skyBoxDay") ? "skyBoxNight" : "skyBoxDay");
}

