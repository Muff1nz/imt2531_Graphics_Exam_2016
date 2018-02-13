#include "Camera.h"


Camera::Camera(glm::vec3 position) {
	m_position 			= position;

	lookAt(position);
	setProjection(0.1f, 600.0f);

	angle1 = 3.14f/2; angle2 = 3.14f;
	updateAngle({ 0, 0 });

	carSeat = glm::vec3(0.973298, 2, -2.393688);
}

Camera::~Camera(){

}

/**
 * Sets the viewMatrix for the camera
 * @param position 	The position the camera should look at
 * @param up  		The up direction
 */
void Camera::lookAt(glm::vec3 position, glm::vec3 up) {
	this->up = up;
	m_viewMatrix = glm::lookAt(position, glm::vec3(0, 0, 0), up);
}

//angle1 = yaw, angle2 = pitch
void Camera::updateAngle(glm::vec2 mouseDelta) {
	angle1 += mouseDelta.x * 0.01f;
	angle2 += mouseDelta.y * 0.01f;

	if (angle2 > 3.14f - 0.5f) angle2 = 3.14f - 0.5f; //bounds, so that forward doesn't become parallell with up
	if (angle2 < 0.1f) angle2 = 0.1f;
	//point on unitsphere
	forward.x = cosf(angle1) * sinf(angle2);
	forward.z = sinf(angle1) * sinf(angle2);
	forward.y = cosf(angle2);

	right = glm::cross(forward, up);
	glm::normalize(right);
}

void Camera::updatePos(ActionEnum action, float dt) {
	if (action == ActionEnum::FORWARD) m_position += forward * dt * speed;
	if (action == ActionEnum::BACK) m_position -= forward * dt * speed;
	if (action == ActionEnum::RIGHT) m_position += right * dt * speed;
	if (action == ActionEnum::LEFT) m_position -= right * dt * speed;
	if (action == ActionEnum::UP) m_position += up * dt * speed;
	if (action == ActionEnum::DOWN) m_position -= up * dt * speed;
}

/**
 * Sets the position of the camera
 * @param position 
 */
void Camera::setPosition(glm::vec3 position) {
	m_position = position;
	lookAt(position, glm::vec3(0, 0, 0));	//Placeholder
}

/**
 * Sets the perspective matrix for the camera
 * @param near Near clip plane
 * @param far  Far clip plane
 * @param fov  Field of view angle in radians
 */
void Camera::setProjection(float near, float far, float fov) {
	glm::vec2 screen(1920, 1080);
	m_projectionMatrix = glm::perspective(fov, screen.x/screen.y, near, far);
}

glm::mat4 Camera::getViewMatrix() {
	if (freeCam)
		m_viewMatrix = glm::lookAt(m_position, forward + m_position, up);
	else { //Makes the camera look from the car seat
		glm::vec3 pos = glm::vec3(*carModel * glm::vec4(carSeat, 1));
		m_viewMatrix = glm::lookAt(pos, forward + pos, up);
	}

	return m_viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix() {
	return m_projectionMatrix;
}