#pragma once
#include "pch.h"

namespace ScatterGL
{

struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shine; //prefered numbers --> 1, 2, 4, 8 etc..
};

struct Light
{
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

}

