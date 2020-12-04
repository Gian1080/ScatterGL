#include "pch.h"
#include "GLCamera.h"
#include "Shader.h"
#include "GLTexture.h"

unsigned int indicesSquare[] = { 0, 1, 3, 1, 2, 3 };

float cubeVertices[] = {
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
};

float verticesBox[] = {
-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
glm::vec3(0.0f,  0.0f,  0.0f),
glm::vec3(2.0f,  5.0f, -15.0f),
glm::vec3(-1.5f, -2.2f, -2.5f),
glm::vec3(-3.8f, -2.0f, -12.3f),
glm::vec3(2.4f, -0.4f, -3.5f),
glm::vec3(-1.7f,  3.0f, -7.5f),
glm::vec3(1.3f, -2.0f, -2.5f),
glm::vec3(1.5f,  2.0f, -2.5f),
glm::vec3(1.5f,  0.2f, -1.5f),
glm::vec3(-1.3f,  1.0f, -1.5f)
};

struct GenericInfo
{
	const unsigned int SCREEN_WIDTH = 1575;
	const unsigned int SCREEN_HEIGHT = 675;
	float lastX = SCREEN_WIDTH / 2.0f;
	float lastY = SCREEN_HEIGHT / 2.0f;
	bool firstMouse = true;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
};

GenericInfo info{};
ScatterGL::GLCamera camera(glm::vec3(0.0f, 0.0f, 3.0f));
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
	glEnable(GL_DEPTH_TEST);
	return window;
}

int main()
{
	GLFWwindow* window = initWindow(info);
	ScatterGL::GLTexture woodTexture("Textures\\container.jpg");
	ScatterGL::GLTexture faceTexture("Textures\\awesomeface.png");

	ScatterGL::Shader cubeShader;
	cubeShader.initialize("Shaders\\CubeShader.vert",
							"Shaders\\CubeShader.frag");


	ScatterGL::Shader naturalLightShader;
	naturalLightShader.initialize("Shaders\\NaturalLight.vert",
							"Shaders\\NaturalLight.frag");

	ScatterGL::Shader absorbCubeShader;
	absorbCubeShader.initialize("Shaders\\AbsorbCube.vert",
							"Shaders\\AbsorbCube.frag");

	unsigned VAO; //vertex array object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int EBO; //element buffer object
	glGenBuffers(1, &EBO); 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesSquare), indicesSquare, GL_STATIC_DRAW);

	unsigned int VBO; //vertex buffer object
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBox), verticesBox, GL_STATIC_DRAW);

	//attribute descriptor for vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	//configue cubes Vertex Array Object and Vertex Buffer Object
	unsigned int VBOTWO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBOTWO);
	glBindBuffer(GL_ARRAY_BUFFER, VBOTWO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glBindVertexArray(cubeVAO);

	//position attribute of cube
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBOTWO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
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
		absorbCubeShader.use();
		absorbCubeShader.setVec3("objectColor", 1.0f, 0.5f, 0.30f);
		absorbCubeShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

		//render square
		//glBindTextureUnit(0, woodTexture.texture);
		//glBindTextureUnit(1, faceTexture.texture);

		glm::mat4 projection = glm::perspective(glm::radians(camera.zoom),
			(float)info.SCREEN_WIDTH / (float)info.SCREEN_HEIGHT, 0.1f, 512.0f);
		glm::mat4 view = camera.getViewMatrix();

		absorbCubeShader.setMat4("projection", projection);
		absorbCubeShader.setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		absorbCubeShader.setMat4("model", model);

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//draw lamp object
		naturalLightShader.use();
		naturalLightShader.setMat4("projection", projection);
		naturalLightShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPosition);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		naturalLightShader.setMat4("model", model);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//int viewLoc = glGetUniformLocation(cubeShader.ID, "view");
		//int projectionLoc = glGetUniformLocation(cubeShader.ID, "projection");
		//cubeShader.setMat4("projection", projection);

		//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		//cubeShader.setMat4("view", view);
		//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		//for (unsigned int i = 0; i < 10; i++)
		//{
		//	glm::mat4 model = glm::mat4(1.0f);
		//	model = glm::translate(model, cubePositions[i]);
		//	float angle = 20.0f * i;
		//	if (i % 3 == 0) angle = glfwGetTime() * i * 20.0f;
		//	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		//	cubeShader.setMat4("model", model);
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
}




