#include "GameObject.h"



GameObject::GameObject(Mesh* mesh, glm::vec3 position,
	glm::vec3 scale, glm::vec3 rotation, std::string texture) {
	
	this->mesh = mesh;
	transform.position = position;
	transform.scale = scale;
	this->texture = texture;
	transform.rotation = rotation;

	parent = nullptr;
}


GameObject::~GameObject() {
}

void GameObject::updateModelMatrix() {
	modelMatrix = glm::translate(glm::mat4(1), transform.position)
		* glm::rotate(glm::mat4(1), transform.rotation.z, glm::vec3(0, 0, 1))
		* glm::rotate(glm::mat4(1), transform.rotation.x, glm::vec3(1, 0, 0))
		* glm::rotate(glm::mat4(1), transform.rotation.y, glm::vec3(0, 1, 0))
		* glm::scale(glm::mat4(1), transform.scale);
}

glm::mat4* GameObject::getModelMatrix() { 
	updateModelMatrix();
	return &modelMatrix;
}

void GameObject::draw(ShaderHandler::ShaderProgram* shaderProgram) {
	updateModelMatrix();

	//Bind data spesific to this object (modelmatrix and texture)
	if(!parent)
		glUniformMatrix4fv((*shaderProgram)["ModelMatrix"], 1, GL_FALSE, glm::value_ptr(modelMatrix));
	else
		glUniformMatrix4fv((*shaderProgram)["ModelMatrix"], 1, GL_FALSE, glm::value_ptr(*parent->getModelMatrix() * modelMatrix));
	
	if(texture != "") //Textures are optional
		glUniform1i((*shaderProgram)["textureBuffer"], TextureHandler::getInstance().getTexture(texture));

	//draw actual mesh
	mesh->draw();
}