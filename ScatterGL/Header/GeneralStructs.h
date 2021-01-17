#pragma once
#include "pch.h"

namespace ScatterGL
{
struct GenericInfo
{
	const unsigned int SCREEN_WIDTH = 2500;
	const unsigned int SCREEN_HEIGHT = 1080;
	float lastX = SCREEN_WIDTH / 2.0f;
	float lastY = SCREEN_HEIGHT / 2.0f;
	bool firstMouse = true;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
};

struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shine; //prefered numbers --> 1, 2, 4, 8 etc..
};

struct DirectionalLight
{
	glm::vec3 direction;
	float intensity;
};

struct MaterialLight
{
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shine;
};

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;
};

}

