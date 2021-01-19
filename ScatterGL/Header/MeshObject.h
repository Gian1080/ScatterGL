#pragma once

#include "pch.h"

namespace ScatterGL
{

class MeshObject
{
public:
	MeshObject(std::vector<float>& verticesTMP, std::vector<GLuint>& indicesREF);
	void drawObject();
	void destroyObject();
	std::vector<float> vertices;
	std::vector<GLuint> indices;

private:
	unsigned int VAO, VBO, EBO;
};
}