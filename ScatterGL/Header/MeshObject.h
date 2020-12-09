#pragma once

#include "pch.h"
#include "GeneralStructs.h"

namespace ScatterGL
{

class MeshObject
{
public:
	MeshObject(std::vector<float>& verticesTMP, uint8_t strideTMP, Material& materialREF);

	void drawObject();
	void destroyObject();
	std::vector<float> makeVector(float array[]);
	Material& material;

private:
	std::vector<float> vertices;
	unsigned int VAO, VBO, EBO;
	uint8_t stride;
	const void* vertDataSize;
};
}