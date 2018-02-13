#pragma once

#if defined(__linux__)						// If we are using linux.
#include <SDL2/SDL.h>

#elif defined(_WIN16) || defined(_WIN32) || defined(_WIN64)	// If we are using a windows machine
#include <SDL.h>
#endif

#define GLM_FORCE_RADIANS //Using degrees with glm is deprecated.
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>

#include "globals.h"
#include "GameEvent.h"

class Camera {
public:
	Camera() {};
	Camera(glm::vec3 position);
	~Camera();

	void lookAt(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
	void updateAngle(glm::vec2 mouseDelta); //Makes it so camera can be controlled with mouse
	void updatePos(ActionEnum action, float dt); //Handles movement

	void setPosition(glm::vec3 pos);
	void setProjection(float near, float far, float fov = glm::quarter_pi<float>() * 1.5f);

	glm::vec3* getPos() { return &m_position; }
	glm::vec3* getForward() { return &forward; }
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	void setCarModelMatrix(glm::mat4* model) { carModel = model; } 
private:
	glm::vec3 m_position;
	glm::mat4* carModel; //This is needed to mount the camera to the car seat
	glm::vec3 carSeat;	//Carseat position in car modelspace
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;

	float angle1, angle2;

	glm::vec3 forward, right, up;

	const float speed = 50.0f;
};