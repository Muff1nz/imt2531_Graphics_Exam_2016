#pragma once
#include "GameObject.h"
#include "GameEvent.h"

#include "lightHandler.h"
#include "MirrorHandler.h"

class Car : public GameObject {
public:
	Car(Mesh* mesh, 
		glm::vec3 position = { 0, 0, 0 }, glm::vec3 scale = { 1, 1, 1 }, glm::vec3 rotation = { 0, 0, 0 },
		std::string texture = "")
		: GameObject(mesh, position, scale, rotation, texture) {
		speed = 0;
		turningSpeed = 1.0f;
		maxSpeed = 40.0f;
		acceleration = 50.0f;
		brakeForce = 100.0f;
		drag = 10.0f;
	};
	~Car() {};

	void init();

	void update(ActionEnum action, float dt);
	void updatePhysics(float dt);

	void updateLights();
	void toggleLights();
private:
	float speed;
	glm::vec3 forward;

	float turningSpeed;
	float maxSpeed;
	float acceleration;
	float brakeForce;
	float drag;

	glm::vec3 lightCarDir; //Original direction for car
	std::vector<glm::vec3> lightCarPos; //mounting points for lights
	std::vector<Light*> lights;	//lights mounted to car
};

