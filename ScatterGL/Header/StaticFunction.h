#pragma once
#include "pch.h"

namespace ScatterGL
{
	template<typename t>
	static std::vector<t> makeVectorFromArray(const t values[])
	{
		std::vector<t> vectorFromArray;
		int arraySize = sizeof(values) / sizeof(values[0]);
		for (unsigned int i = 0; i < arraySize; i++)
		{
			vectorFromArray.push_back(values[i]);
		}
		return vectorFromArray;
	}

	void MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
		if (severity != GL_DEBUG_SEVERITY_NOTIFICATION) {
			fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
				(type == GL_DEBUG_TYPE_ERROR ? " GL ERROR " : ""),
				type, severity, message);
			assert(false);
		}
	}
}
