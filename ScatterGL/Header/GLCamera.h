#pragma once

#include "pch.h"

enum Camera_Direction
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.22f;
const float SENSITIVITY = 0.05f;
const float ZOOM = 45.0f;

namespace ScatterGL
{

class GLCamera
{
public:
	float yaw;
	float pitch;
	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	//default constructor
	GLCamera() :
		front(glm::vec3(0.0f, 0.0f, -1.0f)),
		movementSpeed(SPEED),
		mouseSensitivity(SENSITIVITY),
		zoom(ZOOM),
		position(glm::vec3(0.0f, 0.0f, 0.0f)),
		worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
		yaw(YAW),
		pitch(PITCH)
	{}
	//constructor with vectors
	GLCamera(glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f),
		float Yaw = YAW, float Pitch = PITCH) :

		front(glm::vec3(0.0f, 0.0f, -1.0f)),
		movementSpeed(SPEED),
		mouseSensitivity(SENSITIVITY),
		zoom(ZOOM)
	{
		position = Position;
		worldUp = Up;
		yaw = Yaw;
		pitch = Pitch;
		updateCameraVectors();
	}
	//constructor with scalar (single) values
	GLCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float Yaw, float Pitch) :
		front(glm::vec3(0.0f, 0.0f, -1.0f)),
		movementSpeed(SPEED),
		mouseSensitivity(SENSITIVITY),
		zoom(ZOOM)
	{
		position = glm::vec3(posX, posY, posZ);
		worldUp = glm::vec3(upX, upY, upZ);
		yaw = Yaw;
		pitch = Pitch;
		updateCameraVectors();
	}

	glm::mat4 getViewMatrix();

	void processKeyboard(Camera_Direction direction, float deltaTime);

	void processMouseMovement(float xOffset, float yOffset);

	void processMouseScroll(float yOffset);


private:
	void updateCameraVectors();
};

}