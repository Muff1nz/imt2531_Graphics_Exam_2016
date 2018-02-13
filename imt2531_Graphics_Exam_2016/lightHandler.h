#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "lights.h"
#include "ShaderHandler.h"

//this class exists to keep track of all the lights, to make sending data to
//	the shaders easier. maxLights is the same value in the shaders.
const int maxLights = 10;

class LightHandler {
public:
	//This ensures that there can only be one LightHandler object at any given time.
	static LightHandler& getInstance() {
		static LightHandler instance;
		return instance;
	}


	//This ensures that there can only be one LightHandler object at any given time.
	LightHandler(LightHandler const& copy) = delete;
	void operator=(LightHandler const& copy) = delete;

	void init();

	int getLightCount() { return lightCount; }
	Light* createLight(); 
	Light* getLight(int i) { return lights[i]; }
	void toggleSun();
	void sendUniformData(ShaderHandler::ShaderProgram* shaderProgram);
private:
	int textureCount; //So that the depthmaps dont interfere with the other textures
	int lightCount;
	Light* lights[maxLights]; //I use an array to be more inline with GLSL conventions

	LightHandler() { };
};