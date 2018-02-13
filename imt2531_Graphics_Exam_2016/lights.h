#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

struct Light {
public:
	glm::vec3* pos;
	glm::vec3* direction;	//Direction for the spotlight
	glm::vec3 specular;
	glm::vec3 diffuse;
	glm::vec3 ambient;

	GLuint type;	//1 = spot, 2 = directional
	GLuint depthFB; //Framebuffer used for making depth texture
	GLuint depthFBTex; //Texture that stores depthmap
	GLuint shadowRes;	//Resolution of depthmap

	float arc;	//angle for spotlight, set to 360/180 (depending on how you look at it) for spotlight

	bool active;

	Light() {
		pos = new glm::vec3(0, 0, 0);
		direction = new glm::vec3(0, 0, -1);
		specular = glm::vec3(0.9f, 0.9f, 0.9f);
		diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		ambient = glm::vec3(0.2f, 0.2f, 0.2f);
		active = true;
		shadowRes = 1024;
		type = 1; //spotlight
		arc = glm::quarter_pi<float>();
	}

	Light(glm::vec3* pos, glm::vec3* direction, glm::vec3 specular, glm::vec3 diffuse, glm::vec3 ambient) {
		this->pos = pos;
		this->direction = direction;
		this->specular = specular;
		this->diffuse = diffuse;
		this->ambient = ambient;
		
		active = true;
		shadowRes = 1024;
		arc = glm::quarter_pi<float>();
	}

	glm::mat4 getView();
	glm::mat4 getProj();

	void initShadow(int param = 0);
};