#include "MirrorHandler.h"


Mirror* MirrorHandler::createMirror(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation,
	std::string texture) {
	mirrors.push_back(new Mirror(MeshHandler::getInstance().getMesh("plane"), position, scale, rotation, texture));
	return mirrors[getSize() - 1];
}