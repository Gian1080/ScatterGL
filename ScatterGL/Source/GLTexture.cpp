#include "GLTexture.h"

namespace ScatterGL
{
GLTexture::GLTexture() :
	pathName("No path was given \n"),
	texture(0),
	width(0),
	height(0),
	nrChannels(0)
{};
GLTexture::GLTexture(const char* pathnameTMP) :
	pathName(pathnameTMP)
{
		
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char* data = stbi_load(pathName, &width, &height, &nrChannels, 0);
	if (nrChannels == 3)
	{
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			std::cout << pathName << "\n";
		}
	}
	else if (nrChannels == 4)
	{
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			std::cout << pathName << "\n";
		}
	}

	else
	{
		std::cout << "Failed to load texture\n";
	}
	stbi_image_free(data);
}
}