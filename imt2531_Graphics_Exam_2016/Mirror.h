#pragma once

//Linux
#if defined(__linux__)
#include <GL/glew.h>
//Windows
#elif defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
#include <gl\glew.h>
#endif


#define GLM_FORCE_RADIANS //Using degrees with glm is deprecated.
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GameObject.h"
#include "globals.h"

//Renders scene from its point of view, to a texture thats applied to itself
class Mirror : public GameObject {
public:
	Mirror(Mesh* mesh, 
		glm::vec3 position = { 0, 0, 0 }, glm::vec3 scale = { 1, 1, 1 }, glm::vec3 rotation = { 0, 0, 0 },
		std::string texture = "")
		: GameObject(mesh, position, scale, rotation, texture) {
		direction = { 0, 1, 0 };
	};
	~Mirror() {};

	void init();
	void draw(ShaderHandler::ShaderProgram* shaderProgram);

	glm::mat4 getView();
	glm::mat4 getProj();
	GLuint getFB() { return mirrorFB; }
	GLuint getRes() { return mirrorRes; }
private:
	glm::vec3 direction;//the direction the mirror is facing
	GLfloat mirrorRes;	//texture resolution
	GLuint mirrorFB;	//Framebuffer that renders to texture
	GLuint mirrorFBTex;	//Texture that framebuffer renders to
};

