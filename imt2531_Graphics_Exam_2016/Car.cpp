#include "Car.h"


void Car::init() {
	Mirror* temp; //Don't need to store a reference here, since they're passive

	temp = MirrorHandler::getInstance().createMirror({ 1.9, 1.42 , -1.25 }, { 0.2, 1, 0.2 }, { -glm::half_pi<float>(), glm::pi<float>(), 0 });
	temp->setParent(this);
	temp->init();
	temp = MirrorHandler::getInstance().createMirror({ -1.9, 1.42, -1.25 }, { 0.2, 1, 0.2 }, { -glm::half_pi<float>(), glm::pi<float>(), 0 });
	temp->setParent(this);
	temp->init();

	lightCarDir = { 0, -1, 1 };
	
	//I used my camera position, and cubes in the world to find these mounting points
	//	Wish i could use the obj file to find them instead, but the object names in the obj file
	//	wasn't very descriptive
	// -2.5 because car is +2.5

	//bottom front headlights
	lightCarPos.push_back(glm::vec3(-1.15f, 3.1f - 2.5f, 1.4f));
	lightCarPos.push_back(glm::vec3(1.15f , 3.1f - 2.5f, 1.4f));
	//roof lights
	for (int i = 0; i < 4; i++)
		lightCarPos.push_back(glm::vec3(-0.84 + i*0.56f, 5.283087 - 2.5f, -3.0));

	Light* light;
	for (int i = 0; i < 6; i++) { //Create the lights
		light = LightHandler::getInstance().createLight();
		light->ambient = { 0.002f, 0.002f, 0.002f };
		light->specular = { 0.2f, 0.2f, 0.2f };
		light->diffuse = { 0.4f, 0.4f, 0.4f };
		light->active = false;
		lights.push_back(light);
	}
}

void Car::update(ActionEnum action, float dt) {
	//Car turns by changing Y rotation
	if (action == ActionEnum::RIGHT) transform.rotation.y -= dt * turningSpeed;
	else if (action == ActionEnum::LEFT) transform.rotation.y += dt * turningSpeed;

	//Calculate forward direction from y angle
	forward = glm::vec3(cosf(-transform.rotation.y + glm::half_pi<float>()), 0, sinf(-transform.rotation.y + glm::half_pi<float>()));
	glm::normalize(forward); //just to be safe

	if (action == ActionEnum::FORWARD) speed += acceleration * dt;
	else if (action == ActionEnum::BACK) speed -= brakeForce * dt;	
}

//Some simple car physics
void Car::updatePhysics(float dt) {
	glm::vec3 velocity;
	//Car should be harder to turn at high speeds
	turningSpeed = 5.0f - speed / maxSpeed * 3.0f;

	speed -= drag * dt;
	if (speed < 0) speed = 0;
	else if (speed > maxSpeed) speed = maxSpeed;
	velocity = forward * speed;
	transform.position += velocity * dt;
}

void Car::updateLights() {
	//apply car model matrix to its lights, so that they're not left behind
	updateModelMatrix();
	for (int i = 0; i < lights.size(); i++) {
		*lights[i]->pos = glm::vec3(modelMatrix * glm::vec4(lightCarPos[i], 1));
		*lights[i]->direction = glm::vec3(modelMatrix * glm::vec4(lightCarDir, 0));
	}
}

void Car::toggleLights() {
	for (int i = 0; i < lights.size(); i++) {
		lights[i]->active = !lights[i]->active;
	}
}