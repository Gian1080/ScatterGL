#pragma once

#include "pch.h"
#include "GeneralStructs.h"
#include "Framebuffer.h"

namespace ScatterGL
{
class ScatterGLui
{
public:
	void init(GLFWwindow* window);
	void destroy();
	void drawGui();
	void drawDirectionalLight(DirectionalLight& light);
	void drawScene(ScatterGL::Framebuffer& framebuffer);
	void beginFrameGui();
	void endFrameGui();
private:
	GLFWwindow* window;
	ImVec4 clear_color;
	bool checkBox;
};
}