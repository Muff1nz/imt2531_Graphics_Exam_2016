#pragma once


#define GLM_FORCE_RADIANS //Using degrees with glm is deprecated.
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "TextureHandler.h"
#include "Mesh.h"
#include "ShaderHandler.h"

struct Transform {
public:
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
};

class GameObject {
public:
	GameObject(Mesh* mesh, 
		glm::vec3 position = { 0, 0, 0 }, glm::vec3 scale = { 1, 1, 1 }, glm::vec3 rotation = {0, 0, 0},
		std::string texture = "");
	~GameObject();

	virtual void draw(ShaderHandler::ShaderProgram* shaderProgram);
	glm::mat4* getModelMatrix();
	void updateModelMatrix();
	Transform* getTransform() { return &transform; }
	void setTexture(std::string name) { texture = name; }
	std::string getTexture() { return texture; }
	void setParent(GameObject* p) { parent = p; }
protected:	
	Transform transform;
	glm::mat4 modelMatrix;
	std::string texture;
	Mesh* mesh;
	GameObject* parent;
};

