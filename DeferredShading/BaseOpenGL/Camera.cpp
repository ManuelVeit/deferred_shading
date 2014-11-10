#include "StdAfx.h"
#include "Camera.h"


Camera::Camera(float width, float height)
{
	m_ProjectionMatrix = (glm::mat4)glm::perspective(45.0f, (float)width/(float)height, 0.001f, 1000.0f);

	glm::vec3 eyePosition(0.0f, 0.0f, 0.0f);
	glm::vec3 lookinAtPosition(0.0f, 0.0f, -1.0f);

	glm::vec3 right(1.0f, 0.0f, 0.0f);
	glm::vec3 forward = lookinAtPosition - eyePosition;
	forward = glm::normalize(forward);

	glm::vec3 upDirection = glm::cross(right, forward);

	m_ViewMatrix = (glm::mat4)glm::lookAt(eyePosition, lookinAtPosition, upDirection);
}


Camera::~Camera(void)
{

}

void Camera::UpdateProjectionMatrix(float width, float height)
{
	m_ProjectionMatrix = (glm::mat4)glm::perspective(75.0f, (float)width/(float)height, 0.001f, 1000.0f);
}