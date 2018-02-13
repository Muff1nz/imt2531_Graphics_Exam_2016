#include "TextureHandler.h"
#include "globals.h"

void TextureHandler::init()
{
	loadTexture("track", "./resources/trackFixed.png");
	loadCubemap("skyBoxDay", "./resources/greywash_");
    loadCubemap("skyBoxNight", "./resources/purplenebula_");
	loadTexture("snow", "./resources/snow.png");
	loadTexture("snow2", "./resources/snow2.png");
}

void TextureHandler::loadTexture(const std::string& name, const std::string& path)
{
	// SDL Image is used to load the image and save it as a surface.
	SDL_Surface* textureSurface = IMG_Load(path.c_str());
	if (!textureSurface)
	{
		printf("Failed to load texture \"%s\": %s\n", path.c_str(), IMG_GetError());
		return;
	}

	// Generate and bind the texture buffer
	GLuint textureBuffer = 0;
	glActiveTexture(GL_TEXTURE0 + textureIDs.size()); 
	glGenTextures(1, &textureBuffer);
	glBindTexture(GL_TEXTURE_2D, textureBuffer);

	// We tell OpenGL what to do when it needs to render the texture at a higher or lower resolution.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Create the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureSurface->w, textureSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureSurface->pixels);

	//SDL_FreeSurface(textureSurface); I hate this line of code so so much. crashes my program with certain textures, i'd rather leak memory
	//	wasted more time because of it then i'd like.

	textureIDs.insert(std::pair<std::string, int>(name, textureIDs.size()));

	textureCount++;
}

//This function assumes files from http://www.custommapmakers.org/skyboxes.php
void TextureHandler::loadCubemap(const std::string& name, const std::string& path) {
	GLuint textureID;
	SDL_Surface* textureSurface;

	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0 + textureIDs.size());

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	textureSurface = IMG_Load((path + "ft.tga").c_str());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, textureSurface->w, textureSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, textureSurface->pixels);
	textureSurface = IMG_Load((path + "bk.tga").c_str());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, textureSurface->w, textureSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, textureSurface->pixels);
	textureSurface = IMG_Load((path + "rt.tga").c_str());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, textureSurface->w, textureSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, textureSurface->pixels);
	textureSurface = IMG_Load((path + "lf.tga").c_str());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, textureSurface->w, textureSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, textureSurface->pixels);
	textureSurface = IMG_Load((path + "up.tga").c_str());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, textureSurface->w, textureSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, textureSurface->pixels);
	textureSurface = IMG_Load((path + "dn.tga").c_str());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, textureSurface->w, textureSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, textureSurface->pixels);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	textureIDs.insert(std::pair<std::string, int>(name, textureIDs.size()));

	textureCount++;
}

int TextureHandler::getTexture(const std::string& name)
{
	if (textureIDs.count(name))
		return textureIDs[name];

	printf("ERROR: No texture with the name \"%s\" could be found.\n", name.c_str());
	return 0;
}

