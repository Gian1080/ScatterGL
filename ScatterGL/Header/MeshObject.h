#pragma once

#include "pch.h"
#include "GeneralStructs.h"

namespace ScatterGL
{

class MeshObject
{
public:
	MeshObject(std::vector<float>& verticesTMP, Material& materialREF);

	void drawObject();
	void destroyObject();
	std::vector<float> makeVector(float array[]);
	Material& material;

private:
	std::vector<float> vertices;
	unsigned int VAO, VBO, EBO;
	const void* vertDataSize;
};
}