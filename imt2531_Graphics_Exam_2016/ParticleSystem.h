#pragma once
#if defined(__linux__)
#include <GL/glew.h>

//Windows
#elif defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
#include <gl\glew.h>
#endif

#define GLM_FORCE_RADIANS //Using degrees with glm is deprecated.
#include <glm/glm.hpp>

#include "ShaderHandler.h"
#include "TextureHandler.h"

#define particleCount 5000 //Wouldn't let me make arrays with a const int

//This particle system is basicly tailored for this project
//	So the emitter is the size of the top of the skybox
//	didn't want to spend time making a general solution at this point
//	since im only going to use it for this specific scenario anyways
class ParticleSystem {
public:
	ParticleSystem();
	~ParticleSystem();

	void update(float dt);
	void draw(ShaderHandler::ShaderProgram* shaderProgram);
private:
	GLuint VAO;
	GLuint texture;
	float time;
};

