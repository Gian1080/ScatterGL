#pragma once

#include "pch.h"

namespace ScatterGL
{
class GLTexture
{
public:
	GLTexture();
	GLTexture(const char* pathnameTMP);

	const char* pathName;
	unsigned int texture;
	int width, height, nrChannels;
};
}
