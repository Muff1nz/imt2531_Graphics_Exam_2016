#pragma once

#if defined(__linux__)						// If we are using linux.
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
	
#elif defined(_WIN16) || defined(_WIN32) || defined(_WIN64)	// If we are using a windows machine
#include <SDL_image.h>
#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#endif

#include <map>
#include <string>

class TextureHandler
{
public:
	//This ensures that there can only be one TextureHandler object at any given time.
	static TextureHandler& getInstance() {
		static TextureHandler instance;
		return instance;
	}


	//This ensures that there can only be one TextureHandler object at any given time.
	TextureHandler(TextureHandler const& copy) = delete;
	void operator=(TextureHandler const& copy) = delete;

	void init();
	
	/**
	* Loads an image and creates an OpenGL texture buffer.
	* @param name Name of the texture
	* @param path The path to the image we are loading.
	* @returns The texture identifier for use with OpenGL.
	*/
	void loadTexture(const std::string& name, const std::string& path);
	void loadCubemap(const std::string& name, const std::string& path);

	int getTexture(const std::string& name);
	int getTextureCount() { return textureIDs.size(); }

private:
	TextureHandler() {};
	~TextureHandler() {};

	std::map<std::string, int> textureIDs;
};