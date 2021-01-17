#include "pch.h"
#include "ScatterGLui.h"
#include "Camera.h"
#include "Shader.h"
#include "GLTexture.h"
#include "TestVariables.h"
#include "MeshObject.h"
#include "StaticFunction.h"
#include "Mesh.h"
#include "Model.h"
#include "Framebuffer.h"
#include "Scatter.h"

ScatterGL::GenericInfo info{};
ScatterGL::Framebuffer framebuffer;
ScatterGL::Camera camera(glm::vec3(0.0f, 1.0f, 5.0f));
float nearPlane = 0.1f;
float farPlane = 10000.0f;

glm::vec3 RaekorLight = { 0.0f, -0.97f, -0.26f };

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

ScatterGL::MaterialLight materialLight
{
	glm::vec3(1.0f, 1.0f, 1.0f),   //ambient
	glm::vec3(1.0f, 1.0f, 1.0f),   //diffuse
	glm::vec3(0.5f, 0.5f, 0.5f),    //specular
	glm::vec3(0.5f, 0.5f, 0.5f),    

	2.0f							//shine
};

ScatterGL::DirectionalLight sunLight
{
	glm::vec3(0.0f, -1.0f, -0.3f), //direction
	1.0f
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
	glfwMaximizeWindow(window);
	glViewport(0, 0, info.SCREEN_WIDTH, info.SCREEN_HEIGHT);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(ScatterGL::MessageCallback, nullptr);
	glDepthFunc(GL_LEQUAL);
	glFrontFace(GL_CCW);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	
	return window;
}

int main()
{
	srand(static_cast <unsigned> (time(0)));
	stbi_set_flip_vertically_on_load(false);
	GLFWwindow* window = initWindow(info);
	ScatterGL::ScatterGLui myGui;
	myGui.init(window);

	ScatterGL::MeshObject block = ScatterGL::MeshObject(cube, cubeIndices, cubeMaterial);

	ScatterGL::MeshObject postWork = ScatterGL::MeshObject(screenQuad, screenQuadIndices, cubeMaterial);

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
	std::filesystem::path pathName("Models\\sponzaTwo\\sponza.gltf");
	ScatterGL::Model sponza(pathName.string());

	ScatterGL::Shader depthShader;
	depthShader.initialize("Shaders\\depthShader.vert",
		"Shaders\\depthShader.frag");

	ScatterGL::Shader shadowShader;
	shadowShader.initialize("Shaders\\postProcess.vert",
		"Shaders\\postProcess.frag");

	//starting Shadow API Scatter
	scatter::Scatter myScatter;
	myScatter.init();
	//Setting Vertex information in API
	myScatter.setVertexFormat(scatter::VertexFormat::R32G32B32_SFLOAT);
	myScatter.setVertexOffset(0);
	myScatter.setVertexStride(sizeof(ScatterGL::Vertex));
	myScatter.setIndexFormat(scatter::IndexFormat::UINT32);
	myScatter.createTextures(info.SCREEN_WIDTH, info.SCREEN_HEIGHT);
	//creating memory handles for Scatter
	void* shadowtxtMemHandle = myScatter.getShadowTextureMemoryHandle();
	void* depthTxtMemHandle = myScatter.getDepthTextureMemoryhandle();
	//memory size allocation
	size_t shadowTexMemSize = myScatter.getShadowTextureMemorySize();
	size_t depthTexMemSize = myScatter.getDepthTextureMemorySize();
	//semaphores creation
	void* getReadySemaphore = myScatter.getReadySemaphoreHandle();
	void* getDoneSemaphore = myScatter.getDoneSemaphoreHandle();
	unsigned int readySemaphore;
	unsigned int doneSemaphore;
	glGenSemaphoresEXT(1, &readySemaphore);
	glGenSemaphoresEXT(1, &doneSemaphore);
	glImportSemaphoreWin32HandleEXT(readySemaphore, GL_HANDLE_TYPE_OPAQUE_WIN32_EXT, getReadySemaphore);
	glImportSemaphoreWin32HandleEXT(doneSemaphore, GL_HANDLE_TYPE_OPAQUE_WIN32_EXT, getDoneSemaphore);

	unsigned int shadowTextureMemory;
	glCreateMemoryObjectsEXT(1, &shadowTextureMemory);
	glImportMemoryWin32HandleEXT(shadowTextureMemory, shadowTexMemSize, GL_HANDLE_TYPE_OPAQUE_WIN32_EXT, shadowtxtMemHandle);
	
	glCreateTextures(GL_TEXTURE_2D, 1, &framebuffer.shadowTexture);
	glTextureStorageMem2DEXT(framebuffer.shadowTexture, 1, GL_RGBA8, info.SCREEN_WIDTH, info.SCREEN_HEIGHT, shadowTextureMemory, 0);
	glTextureParameteri(framebuffer.shadowTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(framebuffer.shadowTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	unsigned int depthTextureMemory;
	glCreateMemoryObjectsEXT(1, &depthTextureMemory);
	glImportMemoryWin32HandleEXT(depthTextureMemory, depthTexMemSize, GL_HANDLE_TYPE_OPAQUE_WIN32_EXT, depthTxtMemHandle);

	glCreateTextures(GL_TEXTURE_2D, 1, &framebuffer.depthTexture);
	glTextureStorageMem2DEXT(framebuffer.depthTexture, 1, GL_DEPTH_COMPONENT32F, info.SCREEN_WIDTH, info.SCREEN_HEIGHT, depthTextureMemory, 0);
	glTextureParameteri(framebuffer.depthTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(framebuffer.depthTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glm::mat4 identityMatrix = glm::mat4(1.0f);
	identityMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
	glm::mat4 matrixTwee = glm::mat4(1.0);
	matrixTwee = glm::scale(matrixTwee, glm::vec3(2.0f, 2.0f, 2.0f));
	matrixTwee = glm::translate(matrixTwee, glm::vec3(0.0f, 2.0f, 0.0f));
	for (unsigned int i = 0; i < sponza.getMeshes().size(); i++)
	{
		ScatterGL::Mesh& tempMesh = sponza.getMeshes()[i];
		uint64_t tempMeshHandle = myScatter.addMesh(tempMesh.vertices.data(), tempMesh.indices.data(), tempMesh.vertices.size(), tempMesh.indices.size());
		myScatter.addInstance(tempMeshHandle, glm::value_ptr(identityMatrix));
	}
	ScatterGL::MeshObject& tempMesh = block;
	uint64_t tempMeshHandle = myScatter.addMesh(tempMesh.vertices.data(), tempMesh.indices.data(), tempMesh.vertices.size(), tempMesh.indices.size());
	myScatter.addInstance(tempMeshHandle, glm::value_ptr(matrixTwee));
	myScatter.build();
	framebuffer.attachTexture(framebuffer.depthTexture);

	ScatterGL::Framebuffer postProcess;
	postProcess.initialize(info.SCREEN_WIDTH, info.SCREEN_HEIGHT);


	glm::mat4 projection = glm::perspectiveRH(glm::radians(camera.zoom),
		(float)info.SCREEN_WIDTH / (float)info.SCREEN_HEIGHT, nearPlane, farPlane);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	while(!glfwWindowShouldClose(window))
	{
		//calculating time passed since last frame
		float currentFrame = glfwGetTime();
		info.deltaTime = currentFrame - info.lastFrame;
		info.lastFrame = currentFrame;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, info.SCREEN_WIDTH, info.SCREEN_HEIGHT);
		// user input
		processInput(window, info);
		// setting view matrix
		glm::mat4 view = camera.getViewMatrix();
		// cube renderpass
		framebuffer.bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		materialShader.use();
		materialShader.setMat4("projection", projection);
		materialShader.setMat4("view", view);
		materialShader.setMat4("model", matrixTwee);
		materialShader.setVec3("viewPosition", camera.position);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, boxTexture.texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularBoxTexture.texture);

		materialShader.setInt("material.diffuse", 0);
		materialShader.setInt("material.specular", 1);
		materialShader.setFloat("material.shine", 64.0f);

		materialShader.setVec3("light.position", lightPosition);
		materialShader.setVec3("light.diffuse", 0.2f, 0.2f, 0.2f);
		materialShader.setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
		materialShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		block.drawObject();

		//model rederpass
		modelShader.use();
		modelShader.setInt("texture_diffuse1", 0);
		modelShader.setMat4("projection", projection);
		modelShader.setMat4("view", view);
		modelShader.setMat4("model", identityMatrix);
		sponza.draw(modelShader);
		
		framebuffer.unbind();

		//Scatter API render calls & semaphore calls
		unsigned int texturesThings[] = { framebuffer.shadowTexture, framebuffer.depthTexture };
		GLenum textureThingsHelper[] = { GL_LAYOUT_GENERAL_EXT, GL_LAYOUT_GENERAL_EXT };
		glSignalSemaphoreEXT(readySemaphore, 0, nullptr, 2, texturesThings, textureThingsHelper);

		myScatter.setLightDirection(sunLight.direction.r, sunLight.direction.g, sunLight.direction.b);
		glm::mat4 invertedMatrix = glm::inverse(projection * view);
		myScatter.setInverseViewProjectionMatrix(glm::value_ptr(invertedMatrix));
		myScatter.submit(info.SCREEN_WIDTH, info.SCREEN_HEIGHT);

		GLenum layoutThingsHelper[] = { GL_LAYOUT_SHADER_READ_ONLY_EXT, GL_LAYOUT_DEPTH_STENCIL_ATTACHMENT_EXT };
		glWaitSemaphoreEXT(doneSemaphore, 0, nullptr, 2, texturesThings, layoutThingsHelper);

		postProcess.bind();
		shadowShader.use();
		shadowShader.setFloat("intensity", sunLight.intensity);
		shadowShader.setInt("shadowTexture", 0);
		shadowShader.setInt("colorTexture", 1);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, framebuffer.shadowTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, framebuffer.colorTexture);
		postWork.drawObject();
		postProcess.unbind();

		myGui.beginFrameGui();
		myGui.drawGui();
		myGui.drawDirectionalLight(sunLight);
		myGui.drawTexture("scene", postProcess.colorTexture);
		myGui.drawTexture("normals", framebuffer.normalTexture);
		myGui.drawTexture("positions", framebuffer.positionTexture);
		myGui.drawTexture("shadow", framebuffer.shadowTexture);
		myGui.endFrameGui();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	myGui.destroy();
	cubeObject.destroyObject();
	glfwTerminate();
}

void say()
{
	std::cout << " IETS \n";
}



//float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
//cubeShader.setFloat("r", r);
//float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
//cubeShader.setFloat("g", g);
//float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
//cubeShader.setFloat("b", b);