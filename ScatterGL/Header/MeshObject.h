#pragma once

#include "pch.h"

namespace ScatterGL
{
class MeshObject
{
public:
	MeshObject(std::vector<float>& verticesTMP, uint8_t strideTMP);

	void drawObject();
	void destroyObject();
	std::vector<float> makeVector(float array[]);

private:
	std::vector<float> vertices;
	unsigned int VAO, VBO, EBO;
	uint8_t stride;
	const void* vertDataSize;
};
}