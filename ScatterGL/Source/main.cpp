#include "pch.h"
#include "ScatterGLui.h"
#include "GLCamera.h"
#include "Shader.h"
#include "GLTexture.h"
#include "TestVariables.h"
#include "MeshObject.h"
#include "StaticFunction.h"
#include "Mesh.h"
#include "Model.h"
#include "Framebuffer.h"

ScatterGL::GenericInfo info{};
ScatterGL::Framebuffer framebuffer;
ScatterGL::GLCamera camera(glm::vec3(0.0f, 1.0f, 5.0f));
float nearPlane = 0.1f;
float farPlane = 10000.0f;

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

void processInput(GLFWwindow* window, ScatterGL::GenericInfo& info)
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

GLFWwindow* initWindow(ScatterGL::GenericInfo& info)
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
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glfwMaximizeWindow(window);
	return window;
}

int main()
{
	
	srand(static_cast <unsigned> (time(0)));
	stbi_set_flip_vertically_on_load(false);
	GLFWwindow* window = initWindow(info);
	ScatterGL::ScatterGLui myGui;
	myGui.init(window);
	framebuffer.initialize(info.SCREEN_WIDTH, info.SCREEN_HEIGHT);
	ScatterGL::MeshObject cubeObject(cube, cubeIndices, cubeMaterial);
	ScatterGL::GLTexture woodTexture("Textures\\container.jpg");
	
	ScatterGL::GLTexture boxTexture("Textures\\container2.png");
	ScatterGL::GLTexture specularBoxTexture("Textures\\container2_specular.png");

	ScatterGL::MeshObject surface(flatSurface, flatIndices, surfaceMaterial);
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

	ScatterGL::Shader testShader;
	testShader.initialize("Shaders\\TestShader.vert",
		"Shaders\\TestShader.frag");
	
	ScatterGL::Shader modelShader;
	modelShader.initialize("Shaders\\modelShader.vert",
		"Shaders\\modelShader.frag");
	std::filesystem::path pathName("Z:\\ScatterGL\\ScatterGL\\Models\\sponzaTwo\\sponza.gltf");
	ScatterGL::Model sponza(pathName.string());

	ScatterGL::Shader depthShader;
	depthShader.initialize("Shaders\\depthShader.vert",
		"Shaders\\depthShader.frag");
	//depthShader.setFloat("nearPlane", 0.1f);
	//depthShader.setFloat("farPlane", 10000.0f);

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
		int w, h;
		glfwGetFramebufferSize(window, &w, &h);
		framebuffer.resizeFramebuffer(w, h);
		framebuffer.bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// setting projection and view matrix
		glm::mat4 projection = glm::perspective(glm::radians(camera.zoom),
			(float)info.SCREEN_WIDTH / (float)info.SCREEN_HEIGHT, nearPlane, farPlane);
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 backpackModel = glm::mat4(1.0f);
		backpackModel = glm::translate(backpackModel, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		backpackModel = glm::scale(backpackModel, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		//model render part
		modelShader.use();
		modelShader.setMat4("projection", projection);
		modelShader.setMat4("view", view);
		backpackModel = glm::translate(backpackModel, glm::vec3(1.0f, 1.0f, 1.0f));
		backpackModel = glm::scale(backpackModel, glm::vec3(0.05f, 0.05f, 0.05f));
		modelShader.setMat4("model", backpackModel);
		
		sponza.draw(modelShader);
		framebuffer.unbind();
		
		myGui.beginFrameGui();
		myGui.drawGui();
		myGui.drawDirectionalLight(sunLight);
		myGui.drawScene(framebuffer);
		myGui.endFrameGui();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	myGui.destroy();
	cubeObject.destroyObject();
	glfwTerminate();
}




