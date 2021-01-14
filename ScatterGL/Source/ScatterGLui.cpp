#include "ScatterGLui.h"

namespace ScatterGL
{
	void ScatterGLui::init(GLFWwindow* window)
	{
		const char* glsl_version = "#version 150";
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplOpenGL3_Init(glsl_version);
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		checkBox = true;
	}

	void ScatterGLui::destroy()
	{
		//shutting down ImGui
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ScatterGLui::drawGui()
	{

		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("ScatterGL");                          // Create a window called "ScatterGL" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &checkBox);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &checkBox);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

	}

	void ScatterGLui::drawDirectionalLight(DirectionalLight& light)
	{
		ImGui::Begin("DirectionalLightValue");
		ImGui::Text("Slider to adjust light");
		ImGui::DragFloat3("Direction", glm::value_ptr(light.direction), 0.01f, -3.14f, 3.14f, "%.2f");
		ImGui::End();
	}

	//ScatterGL::Framebuffer& framebuffer
	void ScatterGLui::drawScene(unsigned int framebuffer)
	{
		ImGui::Begin("Scene");
		ImVec2 screen = ImGui::GetContentRegionAvail();
		//.texture
		ImGui::Image((void*)(intptr_t)framebuffer, screen, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
	}

	void ScatterGLui::drawShadowTexture(unsigned int shadowTexture)
	{
		ImGui::Begin("Shadow");
		ImVec2 screen = ImGui::GetContentRegionAvail();
		ImGui::Image((void*)(intptr_t)shadowTexture, screen, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
	}

	void ScatterGLui::beginFrameGui()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ScatterGLui::endFrameGui()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

}