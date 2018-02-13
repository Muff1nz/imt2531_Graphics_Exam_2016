#include "lightHandler.h"
#include "TextureHandler.h"

//returns nullptr if unsuccessful
Light* LightHandler::createLight() {
	if (lightCount < maxLights) {
		lights[lightCount] = new Light();
		lights[lightCount]->initShadow();
		return lights[lightCount++];
	} else
		return nullptr; //to avoid crashing (atleast in this function)
}

void LightHandler::init() {
	lightCount = 0;
	//Sets up the sun
	lights[lightCount] = new Light();
	lights[lightCount]->shadowRes = 4096;
	lights[lightCount]->initShadow(1);
	delete lights[lightCount]->pos;
	lights[lightCount]->pos = new glm::vec3(-100 * cosf(glm::radians(20.0f)), 100 * sinf(glm::radians(20.0f)), 0);
	delete lights[lightCount]->direction;
	lights[lightCount]->direction = new glm::vec3(cosf(glm::radians(20.0f)), -sinf(glm::radians(20.0f)), 0);
	lights[lightCount]->type = 2;

	lightCount++;
}

void LightHandler::sendUniformData(ShaderHandler::ShaderProgram* shaderProgram) {
	int activeLightCount = 0;

	glm::vec3 pos[maxLights];
	glm::vec3 direction[maxLights];
	float arc[maxLights];
	glm::vec3 specular[maxLights];
	glm::vec3 diffuse[maxLights];
	glm::vec3 ambient[maxLights];
	GLuint depthFBTex[maxLights];
	glm::mat4 lightVP[maxLights];
	GLuint lightType[maxLights];
	//a lot of these values don't change in between frames, but this way any value could be dynamic if desired.
	for (int i = 0; i < lightCount; i++) { //Build the arrays that we send to GLSL
		if (lights[i]->active) {
			pos[activeLightCount] = *lights[i]->pos;
			direction[activeLightCount] = *lights[i]->direction;
			specular[activeLightCount] = lights[i]->specular;
			diffuse[activeLightCount] = lights[i]->diffuse;
			ambient[activeLightCount] = lights[i]->ambient;
			depthFBTex[activeLightCount] = lights[i]->depthFBTex;
			arc[activeLightCount] = lights[i]->arc;
			lightVP[activeLightCount] = lights[i]->getProj() * lights[i]->getView();
			lightType[activeLightCount] = lights[i]->type;
			activeLightCount++;
		}		
	}
	glUniform1i((*shaderProgram)["lightCount"], activeLightCount);

	glUniform3fv((*shaderProgram)["lightDirection"], activeLightCount, glm::value_ptr(direction[0]));
	glUniform3fv((*shaderProgram)["lightPos"], activeLightCount, glm::value_ptr(pos[0]));
	glUniform1fv((*shaderProgram)["lightArc"], activeLightCount, &arc[0]);
	glUniform3fv((*shaderProgram)["lightSpecular"], activeLightCount, glm::value_ptr(specular[0]));
	glUniform3fv((*shaderProgram)["lightDiffuse"], activeLightCount, glm::value_ptr(diffuse[0]));
	glUniform3fv((*shaderProgram)["lightAmbient"], activeLightCount, glm::value_ptr(ambient[0]));

	glUniformMatrix4fv((*shaderProgram)["lightVP"], activeLightCount, GL_FALSE, glm::value_ptr(lightVP[0]));
	glUniform1iv((*shaderProgram)["shadowTexture"], activeLightCount, (GLint*)&depthFBTex[0]);
	glUniform1iv((*shaderProgram)["lightType"], activeLightCount, (GLint*)&lightType[0]);
}

void LightHandler::toggleSun() {
	//The sun is always the first light
	lights[0]->active = !lights[0]->active;
}