#pragma once

#include "pch.h"
#include "GeneralStructs.h"
#include "Shader.h"
#include "Mesh.h"

namespace ScatterGL
{
class Model
{
public:
	Model(std::string pathName)
	{
		loadModel(pathName);
		std::cout << pathName << " in constructor \n";
	}

	void draw(Shader& shader);
	std::vector<Mesh>& getMeshes();
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;

private:
	std::string directory;
	
	void loadModel(std::string pathName);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTexture(aiMaterial* material, aiTextureType type, std::string typeName);
	unsigned int textureFromFile(const char* pathName, const std::string& directory, bool gamma);
};
}