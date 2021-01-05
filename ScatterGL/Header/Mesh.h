#pragma once

#include "GeneralStructs.h"
#include "pch.h"
#include "Shader.h"

namespace ScatterGL
{
class Mesh
{
public:
	//mesh data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex>& verticesREF, std::vector<unsigned int>& indicesREF, std::vector<Texture>& texturesREF);
	void draw(ScatterGL::Shader& shader);

private:
	//render data;
	unsigned int VAO, VBO, EBO;

	void setupMesh();
};
}