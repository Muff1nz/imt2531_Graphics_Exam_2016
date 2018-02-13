#include "ParticleSystem.h"
#include <random>

//Sets up a particle system to cover the gameLevel in snow
ParticleSystem::ParticleSystem() { 
	std::default_random_engine rng(78760);
	std::uniform_real_distribution<float> unif(-100.0f, 100.0f); //My map dimensions
	std::uniform_real_distribution<float> unifstart(0.0f, 10.0f); //particle start delay
	GLuint VBO;

	glm::vec3 startPos[particleCount];
	float startTime[particleCount];

	for (int i = 0; i < particleCount; i++) {
		startPos[i] = { unif(rng), 100, unif(rng) };
		startTime[i] = unifstart(rng);
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(startPos), &startPos[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(startTime), &startTime[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);

	texture = TextureHandler::getInstance().getTexture("snow");
	time = 0;
}


ParticleSystem::~ParticleSystem() {
}

//The shader needs the time to work out particle positions
void ParticleSystem::update(float dt) {
	time += dt;
}

void ParticleSystem::draw(ShaderHandler::ShaderProgram* shaderProgram) {	
	glUniform1i((*shaderProgram)["textureBuffer"], texture);
	glUniform1f((*shaderProgram)["time"], time);

	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, particleCount);
}