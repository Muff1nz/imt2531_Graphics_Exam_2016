#pragma once


#include <unordered_map>
#include <memory>

#if defined(__linux__)						// If we are using linux.

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#elif defined(_WIN16) || defined(_WIN32) || defined(_WIN64)	// If we are using a windows machine
#include <SDKDDKVer.h>

//Using SDL, SDL OpenGL, GLEW, standard IO, and strings
#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#endif

#include <string>
#include <map>

/**
 * Handles loading multiple shaderprogram and switching between them.
 */
class ShaderHandler
{
public:
	/**
	 * Handles the shaders various variables.
	 */
	class ShaderProgram
	{
	public:
		ShaderProgram(const std::string& shader);
		~ShaderProgram();
		

		GLuint programId;
		GLuint operator[] (std::string name) { return uniforms[name]; }

	private:
		std::map<std::string, GLuint> uniforms;

		GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
		void findUniforms(const char * shaderFile); //Parses file, and locates uniforms
	};

	/**
	 * Initializes the shaders and returns the one that is set as the current.
	 */
	ShaderProgram* initializeShaders();

	/**
	 * Returns a rawpointer to the ShaderProgram with the passed in name.
	 * @param shader The name of the ShaderProgram we want.
	 * @returns A rawpointer to the ShaderProgram we want.
	 */
	ShaderProgram* getShader(const std::string& shader);

	
private:
	std::unordered_map<std::string, std::unique_ptr<ShaderProgram>> shaders;
};

