#include "pch.h"
#include "ScatterGLui.h"
#include "GLCamera.h"
#include "Shader.h"
#include "GLTexture.h"
#include "TestVariables.h"
#include "MeshObject.h"
#include "StaticFunction.h"
#include "Mesh.h"

GenericInfo info{};
ScatterGL::GLCamera camera(glm::vec3(0.0f, 1.0f, 5.0f));


ScatterGL::Material cubeMaterial
{
	glm::vec3(1.0f, 0.5f, 0.31f),   //ambient
	glm::vec3(0.2f, 0.7f, 0.12f),   //diffuse
	glm::vec3(0.5f, 0.5f, 0.5f),    //specular
	8.0f							//shine
};

ScatterGL::Material surfaceMaterial
{
	glm::vec3(1.0f, 1.0f, 1.0f),   //ambient
	glm::vec3(1.0f, 1.0f, 1.0f),   //diffuse
	glm::vec3(0.0f, 0.0f, 0.0f),    //specular
	2.0f							//shine
};

ScatterGL::DirectionalLight sunLight
{
	glm::vec3(-0.2f, -1.0f, -0.3f), //direction
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
		camera.mouseForMenu = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
	{
		camera.mouseForMenu = false;
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
	srand(static_cast <unsigned> (time(0)));
	GLFWwindow* window = initWindow(info);
	ScatterGL::ScatterGLui myGui;
	myGui.init(window);
	
	ScatterGL::MeshObject cubeObject(cube, cubeIndices, cubeMaterial);
	ScatterGL::GLTexture woodTexture("Textures\\container.jpg");
	
	ScatterGL::MeshObject surface(flatSurface, flatIndices, surfaceMaterial);
	ScatterGL::GLTexture faceTexture("Textures\\awesomeface.png");

	ScatterGL::GLTexture boxTexture("Textures\\container2.png");
	ScatterGL::GLTexture specularBoxTexture("Textures\\container2_specular.png");

	ScatterGL::Shader cubeShader;
	cubeShader.initialize("Shaders\\CubeShader.vert",
							"Shaders\\CubeShader.frag");

	ScatterGL::Shader naturalLightShader;
	naturalLightShader.initialize("Shaders\\NaturalLight.vert",
							"Shaders\\NaturalLight.frag");

	ScatterGL::Shader materialShader;
	materialShader.initialize("Shaders\\Material.vert",
							"Shaders\\Material.frag");

	ScatterGL::Shader testShader;
	testShader.initialize("Shaders\\TestShader.vert",
		"Shaders\\TestShader.frag");
	
	cubeShader.use();
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	cubeShader.setFloat("r", r);
	float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	cubeShader.setFloat("g", g);
	float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	cubeShader.setFloat("b", b);

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

		glm::mat4 projection = glm::perspective(glm::radians(camera.zoom),
			(float)info.SCREEN_WIDTH / (float)info.SCREEN_HEIGHT, 0.1f, 512.0f);
		glm::mat4 view = camera.getViewMatrix();
		materialShader.setMat4("projection", projection);
		materialShader.setMat4("view", view);

		//setting surface test things
		materialShader.setInt("material.diffuse", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, boxTexture.texture);
		materialShader.setInt("material.specular", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularBoxTexture.texture);
		materialShader.setVec3("material.ambient", surface.material.ambient);
		materialShader.setFloat("material.shine", surface.material.shine);
		
		materialShader.setVec3("light.direction", sunLight.direction);
		materialShader.setVec3("light.ambient", sunLight.ambient);
		materialShader.setVec3("light.diffuse", sunLight.diffuse);
		materialShader.setVec3("light.specular", sunLight.specular);
		materialShader.setVec3("viewPosition", camera.position);
		glm::mat4 testModel = glm::mat4(1.0f);
		testModel = glm::translate(testModel, glm::vec3(0.0f, 0.0f, 0.0f));
		testModel = glm::scale(testModel, glm::vec3(10.0f, 1.0f, 10.0f));
		materialShader.setMat4("model", testModel);
		surface.drawObject();
		glBindTexture(GL_TEXTURE_2D, 0);

		//setting material properties
		materialShader.setInt("material.diffuse", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, woodTexture.texture);
		materialShader.setVec3("material.ambient", cubeObject.material.ambient);
		materialShader.setFloat("material.shine", cubeObject.material.shine);

		materialShader.setVec3("light.position", sunLight.direction);
		materialShader.setVec3("light.ambient", sunLight.ambient);
		materialShader.setVec3("light.diffuse", sunLight.diffuse);
		materialShader.setVec3("light.specular", sunLight.specular);

		materialShader.setVec3("viewPosition", camera.position);

		//view & projection matrix calculations
		glm::mat4 model = glm::mat4(1.0f);
		materialShader.setMat4("model", model);
		cubeObject.drawObject();



		//draw lamp object
		naturalLightShader.use();
		naturalLightShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
		naturalLightShader.setMat4("projection", projection);
		naturalLightShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, sunLight.direction);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		naturalLightShader.setMat4("model", model);
		cubeObject.drawObject();

		testShader.use();
		testShader.setInt("material.diffuse", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, boxTexture.texture);
		testShader.setInt("material.specular", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularBoxTexture.texture);
		testShader.setVec3("material.ambient", surface.material.ambient);
		testShader.setFloat("material.shine", surface.material.shine);

		testShader.setVec3("light.direction", sunLight.direction);
		testShader.setVec3("light.ambient", sunLight.ambient);
		testShader.setVec3("light.diffuse", sunLight.diffuse);
		testShader.setVec3("light.specular", sunLight.specular);
		testShader.setVec3("viewPosition", camera.position);
		int viewLoc = glGetUniformLocation(testShader.ID, "view");
		int projectionLoc = glGetUniformLocation(testShader.ID, "projection");
		testShader.setMat4("projection", projection);
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		testShader.setMat4("view", view);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));


		for (unsigned int i = 0; i < 5; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			angle = glfwGetTime() * i * 20.0f;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			testShader.setMat4("model", model);
			cubeObject.drawObject();
		}
		myGui.beginFrameGui();
		myGui.drawGui();
		myGui.drawDirectionalLight(sunLight);
		myGui.endFrameGui();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	myGui.destroy();
	cubeObject.destroyObject();
	glfwTerminate();
}




