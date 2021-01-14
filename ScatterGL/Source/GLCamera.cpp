#include "GLCamera.h"

namespace ScatterGL
{
	glm::mat4 GLCamera::getViewMatrix()
	{
		return glm::lookAtRH(position, position + front, up);
	}

	void GLCamera::processKeyboard(Camera_Direction direction, float deltaTime)
	{
		float velocity = movementSpeed * deltaTime *2;
		if (direction == FORWARD) position += front * velocity;
		if (direction == BACKWARD) position -= front * velocity;
		if (direction == LEFT) position -= right * velocity;
		if (direction == RIGHT) position += right * velocity;
		if (direction == UP) position += up * velocity;
		if (direction == DOWN) position -= up * velocity;

	}

	void GLCamera::processMouseMovement(float xOffset, float yOffset)
	{
		xOffset *= mouseSensitivity;
		yOffset *= mouseSensitivity;

		yaw += xOffset;
		pitch += yOffset;
		GLboolean constrainPitch = true;
		if (constrainPitch)
		{
			if (pitch > 89.0f) pitch = 89.0f;
			if (pitch < -89.0f) pitch = -89.0f;
			
		}
		if (!mouseForMenu)
		{
			GLCamera::updateCameraVectors();
		}
	}

	void GLCamera::processMouseScroll(float yOffset)
	{
		zoom -= (float)yOffset;
		if (zoom < 1.0f) zoom = 1.0f;
		if (zoom > 45.0f) zoom = 45.0f;
	}

	void GLCamera::updateCameraVectors()
	{
		//calculating new front vector
		glm::vec3 Front;
		Front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		Front.y = sin(glm::radians(pitch));
		Front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(Front);
		//update the right and up vector
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}
}