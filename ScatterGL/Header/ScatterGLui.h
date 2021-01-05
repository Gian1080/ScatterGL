#pragma once

#include "pch.h"
#include "GeneralStructs.h"

namespace ScatterGL
{
class ScatterGLui
{
public:
	void init(GLFWwindow* window);
	void destroy();
	void drawGui();
	void drawDirectionalLight(DirectionalLight& light);
	void beginFrameGui();
	void endFrameGui();
private:
	GLFWwindow* window;
	ImVec4 clear_color;
	bool checkBox;
};
}