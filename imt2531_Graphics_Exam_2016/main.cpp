/* Template Author:Simon McCallum
Notes:
Created for the IMT2531 Graphics Exam 2015

To get the naming right I have set.
Project-> Properties -> Configuration properties -> General -> Target name  = exam2016.exe
Project-> Properties -> Configuration properties -> Debugging -> Commande Arguments = level01.dat

I suggest you read and understand how the input handler works.  It uses and event queue system.  
The keyboard events create game events which are pushed onto the GameEvent queue.  These are the
events which the program should actually repond to rather than the actual keyboard presses.

You should have environment variables set up for home of SDL GLEW and GLM so the calls below work
$(SDL_HOME)\include;$(GLEW_HOME)\include;$(GLM_HOME);$(IncludePath)


*/

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
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//C++ 11 stuff to use
#include <stdio.h>
#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include <sstream>

#include "globals.h"
#include "GameLevel.h"
#include "InputHandler.h"
#include "GameEvent.h"
#include "Camera.h"

#include "ShaderHandler.h"
#include "lightHandler.h"
#include "TextureHandler.h"
#include "MirrorHandler.h"

//Screen dimension constants
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
std::string windowName = "Graphics Exam 2016 - Martin Bjerknes"; //You should update this to your name



SDL_Window* gWindow = NULL;

SDL_GLContext gContext;

bool init();
bool initGL();


SDL_Event input;


/*Does the initialisaiton of SDL*/
bool init()
{
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	} else {
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		gWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		} else {
			//Create context
			gContext = SDL_GL_CreateContext(gWindow);
			if (gContext == NULL) {
				printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			} else {
				glewExperimental = GL_TRUE;
				GLenum glewError = glewInit();
				if (glewError != GLEW_OK) {
					printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
				}
				if (SDL_GL_SetSwapInterval(1) < 0){
					printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
				}
				if (!initGL())	{
					printf("Unable to initialize OpenGL!\n");
					success = false;
				}
			}
		}
	}
	IMG_Init(IMG_INIT_JPG);
	TextureHandler::getInstance().init();
	LightHandler::getInstance().init();
	InputHandler::getInstance().init();
	return success;
}


/*
This is where all the initialisation for OpenGL would go.  You may want to move this to its own file.
*/
bool initGL()
{
	//Success flag
	bool success = true;
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	// As part of exam you should work out what to include here
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	return success;
}

void renderScene(GameLevel* level, glm::mat4 view, glm::mat4 proj, ShaderHandler* shaderHandler) {
	ShaderHandler::ShaderProgram* shaderProgram = nullptr;
	//-------------------Render stuff without textures-------------------
	shaderProgram = shaderHandler->getShader("shadowPhong");
	glUseProgram(shaderProgram->programId);

	LightHandler::getInstance().sendUniformData(shaderProgram);

	glUniformMatrix4fv((*shaderProgram)["ProjectionMatrix"], 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv((*shaderProgram)["ViewMatrix"], 1, GL_FALSE, glm::value_ptr(view));

	level->getCar()->draw(shaderProgram);
	for (GameObject i : *level->getTrees())	i.draw(shaderProgram);

	glUseProgram(NULL);
	//-------------------Render stuff with textures-------------------
	shaderProgram = shaderHandler->getShader("shadowPhongTexture");
	glUseProgram(shaderProgram->programId);

	LightHandler::getInstance().sendUniformData(shaderProgram);

	glUniformMatrix4fv((*shaderProgram)["ProjectionMatrix"], 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv((*shaderProgram)["ViewMatrix"], 1, GL_FALSE, glm::value_ptr(view));

	level->getGround()->draw(shaderProgram);
	for (int i = 0; i < MirrorHandler::getInstance().getSize(); i++)
		MirrorHandler::getInstance()[i]->draw(shaderProgram);

	glUseProgram(NULL);
	//-------------------Render skybox-------------------
	shaderProgram = shaderHandler->getShader("skyboxShader");
	glUseProgram(shaderProgram->programId);

	glUniformMatrix4fv((*shaderProgram)["ProjectionMatrix"], 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv((*shaderProgram)["ViewMatrix"], 1, GL_FALSE, glm::value_ptr(view));

	glFrontFace(GL_CW); //Render the insides of the box
	level->getSkyBox()->draw(shaderProgram);
	glFrontFace(GL_CCW);

	glUseProgram(NULL);
	//-------------------Render Particles------------------
	shaderProgram = shaderHandler->getShader("particle");
	glUseProgram(shaderProgram->programId);

	LightHandler::getInstance().sendUniformData(shaderProgram);

	glEnable(GL_POINT_SPRITE); //So that i can texture my points
	glEnable(GL_PROGRAM_POINT_SIZE);
	glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);

	glUniformMatrix4fv((*shaderProgram)["ProjectionMatrix"], 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv((*shaderProgram)["ViewMatrix"], 1, GL_FALSE, glm::value_ptr(view));
	level->getSnow()->draw(shaderProgram);

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glDisable(GL_PROGRAM_POINT_SIZE);
	glDisable(GL_POINT_SPRITE);

	glUseProgram(NULL);
}

void renderShadows(GameLevel* level, ShaderHandler* shaderHandler) {
	ShaderHandler::ShaderProgram* shaderProgram = nullptr;
	shaderProgram = shaderHandler->getShader("shadow");
	glUseProgram(shaderProgram->programId);
	for (int i = 0; i < LightHandler::getInstance().getLightCount(); i++) {
		Light* light = LightHandler::getInstance().getLight(i);
		if (light->active) {
			glBindFramebuffer(GL_FRAMEBUFFER, light->depthFB);
			glViewport(0, 0, light->shadowRes, light->shadowRes);
			glClearColor(0.0, 0.0, 0.0, 1.0);
			glClear(GL_DEPTH_BUFFER_BIT);

			glUniformMatrix4fv((*shaderProgram)["lightVP"], 1, GL_FALSE, glm::value_ptr(light->getProj() * light->getView()));

			level->getCar()->draw(shaderProgram);
			level->getGround()->draw(shaderProgram);
			for (GameObject i : *level->getTrees())	i.draw(shaderProgram);
		}
	}
	glUseProgram(NULL);
}

void renderMirrors(GameLevel* level, ShaderHandler* shaderHandler) {
	for (int i = 0; i < MirrorHandler::getInstance().getSize(); i++) {
		Mirror* mirror = MirrorHandler::getInstance()[i];
		glBindFramebuffer(GL_FRAMEBUFFER, mirror->getFB());
		glViewport(0, 0, mirror->getRes(), mirror->getRes());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderScene(level, mirror->getView(), mirror->getProj(), shaderHandler);
	}
}

void draw(GameLevel* level, Camera* camera, ShaderHandler* shaderHandler) {
	//===========================================Render shadows to texture===========================================
	renderShadows(level, shaderHandler);
	//===========================================Render mirror reflections to texture===========================================
	renderMirrors(level, shaderHandler);
	//===========================================Render scene===========================================
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	renderScene(level, camera->getViewMatrix(), camera->getProjectionMatrix(), shaderHandler);

	//Update screen
	SDL_GL_SwapWindow(gWindow);
}

void update(float deltaTime, std::queue<GameEvent> &eventQueue, Camera &camera, GameLevel* currentLevel) {
	Car* car = currentLevel->getCar();

	while (!eventQueue.empty()) {
		GameEvent nextEvent = eventQueue.front();
		eventQueue.pop();

		if (nextEvent.action == ActionEnum::MOUSEMOTION)
			camera.updateAngle(InputHandler::getInstance().getMouseDelta());
		if (nextEvent.action == ActionEnum::NIGHT) {
			currentLevel->toggleSkyBox();
			LightHandler::getInstance().toggleSun();
			car->toggleLights();
		}
		if (nextEvent.action == ActionEnum::PRINTPOS) //I used this to place stuff in the world (trees and car equipment)
			printf("camPos: (%f, %f, %f)\n", camera.getPos()->x, camera.getPos()->y, camera.getPos()->z);
		if (nextEvent.action == ActionEnum::TOGGLECAMERA)
			freeCam = !freeCam;

		if (nextEvent.action == ActionEnum::TOGGLEMOUSE) {
			SDL_ShowCursor(!SDL_ShowCursor(SDL_QUERY));
			SDL_SetRelativeMouseMode((SDL_bool)!(bool)SDL_GetRelativeMouseMode());	//The ! operator isnt supported by the SDL_bool it seems
		}

		if (freeCam)
			camera.updatePos(nextEvent.action, deltaTime);
		else
			car->update(nextEvent.action, deltaTime);
	}
	car->updatePhysics(deltaTime);
	car->updateLights();
	currentLevel->getSnow()->update(deltaTime);
}

/*
I assume you want a description of the entire program here

The program features all the required elements, plus shadows, snowfall (particles) and freecam mode

Keybindsings are: WASD(SPACE + C) movement, N nightmode, L toggle cursor, F toggle freecam, ESC to quit, also mouse to change camera direction

You can send a commandline parameter to load a level file, my project only contains one level file,
which is treated as the default.

The game loop starts by reading input to generate events, then updating all objects and proccesing events
Every 1/60 of a second it renders everything as a last step

*/
int main(int argc, char * argv[])
{
	gRunning = true;
	freeCam = true;
	SDL_Event eventHandler;
	GameLevel* currentLevel;
	std::queue<GameEvent> eventQueue;
	
	float nextFrame = 1 / gFpsGoal; //Time between frames in seconds
	float nextFrameTimer = 0.0f; //Time from last frame in seconds
	float deltaTime = 0.0f; //Time since last pass through of the game loop.
	auto clockStart = std::chrono::high_resolution_clock::now(); //Clock used for timing purposes
	auto clockStop = clockStart;

	if (!init()){
		printf("Failed to initialize!\n");
	} else {

		if (argc > 1) {
			currentLevel = new GameLevel(std::string(argv[1]));
		} else { //Default level
			currentLevel = new GameLevel("resources/level01.dat");
		}

		ShaderHandler shaderHandler;
		shaderHandler.initializeShaders();

		Camera camera({ 0, 0, 0 });
		camera.setCarModelMatrix(currentLevel->getCar()->getModelMatrix());

		while (gRunning){
			clockStart = std::chrono::high_resolution_clock::now();
			
			InputHandler::getInstance().readInput(eventQueue);
			update(deltaTime, eventQueue, camera, currentLevel);
	
			if (nextFrameTimer >= nextFrame) {
				draw(currentLevel, &camera, &shaderHandler);
				nextFrameTimer = 0.0f;
			}		

			clockStop = std::chrono::high_resolution_clock::now();
			deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(clockStop - clockStart).count();
			nextFrameTimer += deltaTime;
		}
	}
	return 0;
}