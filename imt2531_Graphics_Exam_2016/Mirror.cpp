#include "Mirror.h"

void Mirror::init() {
	mirrorRes = 1024;

	//generate framebuffer and texture for shadow depth map
	glGenFramebuffers(1, &mirrorFB);
	glBindFramebuffer(GL_FRAMEBUFFER, mirrorFB);

	glActiveTexture(GL_TEXTURE0 + textureCount);
	glGenTextures(1, &mirrorFBTex);
	glBindTexture(GL_TEXTURE_2D, mirrorFBTex);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		mirrorRes,
		mirrorRes,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		NULL
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// The depth buffer
	GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mirrorRes, mirrorRes);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	//make framebuffer draw to texture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mirrorFBTex, 0);

	GLenum draw_bufs[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, draw_bufs);

	//glReadBuffer(GL_NONE);
	//rebind default buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	mirrorFBTex = textureCount++;
}

void Mirror::draw(ShaderHandler::ShaderProgram* shaderProgram) {
	updateModelMatrix();

	//Bind data spesific to this object (modelmatrix and texture)
	if (!parent)
		glUniformMatrix4fv((*shaderProgram)["ModelMatrix"], 1, GL_FALSE, glm::value_ptr(modelMatrix));
	else
		glUniformMatrix4fv((*shaderProgram)["ModelMatrix"], 1, GL_FALSE, glm::value_ptr(*parent->getModelMatrix() * modelMatrix));

	glUniform1i((*shaderProgram)["textureBuffer"], mirrorFBTex);

	//draw actual mesh
	mesh->draw();
}

glm::mat4 Mirror::getView() {
	if (!parent)
		direction = glm::vec3(modelMatrix * glm::vec4(0, 1, 0, 0));
	else
		direction = glm::vec3(*parent->getModelMatrix() * modelMatrix * glm::vec4(0, 1, 0, 0));

	if (!parent)
		return glm::lookAt(transform.position, transform.position + direction, { 0, 1, 0 });
	else {
		glm::vec3 pos = glm::vec3(*parent->getModelMatrix() * glm::vec4(transform.position, 1));
		return glm::lookAt(pos,	pos + direction, { 0, 1, 0 });
	}
}

glm::mat4 Mirror::getProj() {
	return glm::perspective(glm::half_pi<float>(), 1.0f, 0.1f, 200.0f);
}