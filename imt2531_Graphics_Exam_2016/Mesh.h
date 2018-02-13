#pragma once

//Linux
#if defined(__linux__)
	#include <SDL2/SDL.h>
	#include <GL/glew.h>
	#include <SDL2/SDL_opengl.h>
//Windows
#elif defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
	#include <SDKDDKVer.h>

	#include <SDL.h>
	#include <gl\glew.h>
	#include <SDL_opengl.h>
	#include <gl\glu.h>
#endif

#define GLM_FORCE_RADIANS //Using degrees with glm is deprecated.
#include <glm/glm.hpp>

#include <vector>
#include <string>

struct ModelData {
	int vertexOrder = 3;
	int materialDepth = 3;
	int normalOrder = 3;

	int numberOfVertexes = 0;
	std::vector<GLfloat> vertexPosition;
	std::vector<GLfloat> vertexNormal;
	std::vector<GLfloat> vertexUV;
	//material data
	std::vector<GLfloat> ambient;
	std::vector<GLfloat> diffuse;
	std::vector<GLfloat> specular;
	std::vector<GLfloat> specularExponent;

	int numberOfIndices = 0;
	std::vector<GLuint> indices;
};

class Mesh
{
public:
	Mesh();
	Mesh(std::string modelPath, bool texture, glm::vec2 shapeRange = { 0, 0 });
	~Mesh();
	
	void draw();
private:
	GLuint VAO, IBO, drawSize;

	void createBufferObjects(const ModelData& model, bool texture);
	ModelData loadModelData(std::string modelPath, bool texture, glm::vec2 shapeRange = { 0, 0 });
	void centerAndRaise(std::vector<float> &verts); //Used when importing a subset of shapes from a .obj file
};

