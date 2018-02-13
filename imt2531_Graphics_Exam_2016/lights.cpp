#include "lights.h"
#include "globals.h"

glm::mat4 Light::getView() {
	return glm::lookAt(*pos , *pos + *direction, { 0, 1, 0 });
}

glm::mat4 Light::getProj() { //Directional lights use ortographic projection
	if (type == 1)
		return glm::perspective(glm::half_pi<GLfloat>(), 1.0f, 0.1f, 200.0f);
	if (type == 2)
		return glm::ortho(100.0f, -100.0f, 50.0f, -50.0f, 0.1f, 200.0f);
}

void Light::initShadow(int param) { //Makes a framebuffer and a texture for the shadows that the light casts
	//generate framebuffer and texture for shadow depth map
	glGenFramebuffers(1, &depthFB);
	glBindFramebuffer(GL_FRAMEBUFFER, depthFB);

	glActiveTexture(GL_TEXTURE0 + textureCount);
	glGenTextures(1, &depthFBTex);	
	glBindTexture(GL_TEXTURE_2D, depthFBTex);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_DEPTH_COMPONENT,
		shadowRes,
		shadowRes,
		0,
		GL_DEPTH_COMPONENT,
		GL_UNSIGNED_BYTE,
		NULL
	);

	if (param == 1) { //Tried to make shadows from the sun look nicer, didn't help
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//make framebuffer draw to texture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthFBTex, 0);

	GLenum draw_bufs[] = { GL_NONE };
	glDrawBuffers(1, draw_bufs);

	glReadBuffer(GL_NONE);
	//rebind default buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	depthFBTex = textureCount++;
}