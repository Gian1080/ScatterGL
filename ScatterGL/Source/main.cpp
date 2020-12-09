#include "pch.h"
#include "ScatterGLui.h"
#include "GLCamera.h"
#include "Shader.h"
#include "GLTexture.h"
#include "TestVariables.h"
#include "MeshObject.h"
#include "StaticFunction.h"

GenericInfo info{};
ScatterGL::GLCamera camera(glm::vec3(0.0f, 0.0f, 3.0f));
ScatterGL::Material cubeMaterial
{
	glm::vec3(1.0f, 0.5f, 0.31f),   //ambient
	glm::vec3(0.2f, 0.7f, 0.12f),   //diffuse
	glm::vec3(0.5f, 0.5f, 0.5f), //specular
	8.0f							//shine
};

ScatterGL::Light sunLight
{
	glm::vec3(2.0f, 2.0f, 2.2f), //position
	glm::vec3(0.1f, 0.1f, 0.1f), //ambient
	glm::vec3(0.5f, 0.5f, 0.5f), //diffuse
	glm::vec3(1.0f, 1.0f, 1.0f) //specular
};

glm::vec3 lightPosition(1.2f, 1.0f, 2.0f);

void framebuffer_resize_callback(GLFWwindow* windowPTR, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* windowPTR, double xpos, double ypos)
{
	if (info.firstMouse)
	{
		info.lastX = xpos;
		info.lastY = ypos;
		info.firstMouse = false;
	}
	float xoffset = xpos - info.lastX;
	float yoffset = info.lastY - ypos;

	info.lastX = xpos;
	info.lastY = ypos;

	camera.processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.processMouseScroll(yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, GenericInfo& info)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	float cameraSpeed = 2.5f * info.deltaTime;
	//std::cout << info.deltaTime << '\n';
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.processKeyboard(FORWARD, info.deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.processKeyboard(BACKWARD, info.deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.processKeyboard(LEFT, info.deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.processKeyboard(RIGHT, info.deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camera.processKeyboard(UP, info.deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		camera.processKeyboard(DOWN, info.deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

GLFWwindow* initWindow(GenericInfo& info)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(info.SCREEN_WIDTH, info.SCREEN_HEIGHT, "ScatterGL", NULL, NULL);
	if (window == NULL)
	{
		throw std::runtime_error("failed to create window \n");
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	//glfwSwapInterval(0); If you dont want vsync
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::runtime_error("failed to initialize GLAD \n");
	}
	glViewport(0, 0, info.SCREEN_WIDTH, info.SCREEN_HEIGHT);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(ScatterGL::MessageCallback, nullptr);
	glEnable(GL_DEPTH_TEST);
	return window;
}

int main()
{
	GLFWwindow* window = initWindow(info);
	ScatterGL::ScatterGLui myGui;
	myGui.innit(window);
	

	ScatterGL::MeshObject object(verticesWithNormals, 3, cubeMaterial);
	ScatterGL::GLTexture woodTexture("Textures\\container.jpg");
	ScatterGL::GLTexture faceTexture("Textures\\awesomeface.png");

	ScatterGL::Shader cubeShader;
	cubeShader.initialize("Shaders\\CubeShader.vert",
							"Shaders\\CubeShader.frag");

	ScatterGL::Shader naturalLightShader;
	naturalLightShader.initialize("Shaders\\NaturalLight.vert",
							"Shaders\\NaturalLight.frag");

	ScatterGL::Shader materialShader;
	materialShader.initialize("Shaders\\Material.vert",
							"Shaders\\Material.frag");
	bool waar = true;
	
	while(!glfwWindowShouldClose(window))
	{
		//calculating time passed since last frame
		float currentFrame = glfwGetTime();
		info.deltaTime = currentFrame - info.lastFrame;
		info.lastFrame = currentFrame;
		// user input
		processInput(window, info);
		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//activating shader
		materialShader.use();
		//setting material properties
		materialShader.setVec3("material.ambient", object.material.ambient);
		materialShader.setVec3("material.diffuse", object.material.diffuse);
		materialShader.setVec3("material.specular", object.material.specular);
		materialShader.setFloat("material.shine", object.material.shine);

		materialShader.setVec3("light.position", sunLight.position);
		materialShader.setVec3("light.ambient", sunLight.ambient);
		materialShader.setVec3("light.diffuse", sunLight.diffuse);
		materialShader.setVec3("light.specular", sunLight.specular);

		materialShader.setVec3("viewPosition", camera.position);
		//view & projection matrix calculations
		glm::mat4 projection = glm::perspective(glm::radians(camera.zoom),
			(float)info.SCREEN_WIDTH / (float)info.SCREEN_HEIGHT, 0.1f, 512.0f);
		glm::mat4 view = camera.getViewMatrix();
		materialShader.setMat4("projection", projection);
		materialShader.setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		materialShader.setMat4("model", model);

		object.drawObject();

		//draw lamp object
		naturalLightShader.use();
		naturalLightShader.setMat4("projection", projection);
		naturalLightShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, sunLight.position);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		naturalLightShader.setMat4("model", model);
		object.drawObject();

		cubeShader.use();
		int viewLoc = glGetUniformLocation(cubeShader.ID, "view");
		int projectionLoc = glGetUniformLocation(cubeShader.ID, "projection");
		cubeShader.setMat4("projection", projection);
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		cubeShader.setMat4("view", view);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		for (unsigned int i = 0; i < 2; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			angle = glfwGetTime() * i * 20.0f;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			cubeShader.setMat4("model", model);
		}
		object.drawObject();

		myGui.drawGui();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	myGui.destroy();
	//Destroying objects
	object.destroyObject();
	glfwTerminate();
}




