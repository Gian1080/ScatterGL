#pragma once

#include "pch.h"

namespace ScatterGL
{
class ScatterGLui
{
public:
	void innit(GLFWwindow* window);
	void destroy();
	void drawGui();

private:
	GLFWwindow* window;
	ImVec4 clear_color;
	bool checkBox;
};
}