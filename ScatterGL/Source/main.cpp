#include "pch.h"
#include "GeneralStructs.h"
#include "ScatterGLui.h"
#include "Camera.h"
#include "Shader.h"
#include "GLTexture.h"
#include "MeshObject.h"
#include "StaticFunction.h"
#include "Mesh.h"
#include "Model.h"
#include "Framebuffer.h"
#include "Scatter.h"



ScatterGL::GenericInfo info{};
ScatterGL::Framebuffer framebuffer;
ScatterGL::Camera camera(glm::vec3(0.0f, 1.0f, 5.0f));
std::vector<uint64_t> meshHandlesBlocks;
std::vector<uint64_t> meshHandlesSponza;
std::vector<ScatterGL::Mesh> lightOrbs;


ScatterGL::Material cubeMaterial
{
	glm::vec3(1.0f, 0.5f, 0.31f),   //ambient
	glm::vec3(0.2f, 0.7f, 0.12f),   //diffuse
	glm::vec3(0.5f, 0.5f, 0.5f),    //specular
	8.0f							//shine
};

ScatterGL::DirectionalLight sunLight
{
	glm::vec3(0.0f, -1.0f, 0.0f), //direction
	1.0f						//intensity
};

ScatterGL::PointLight lightOrb
{
	glm::vec3(0.0f, 0.0f, 0.0f), //position
	0.05f,	//constant
	0.05f,  //linear
	0.01f  //quadratic
};

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
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
	{
		//camera.position = 
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
	glfwSwapInterval(0); //If you dont want vsync
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
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(ScatterGL::MessageCallback, nullptr);
	glDepthFunc(GL_LEQUAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	//glFrontFace(GL_CCW);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	
	return window;
}

struct PointLightCollection
{
	std::vector<ScatterGL::Model*> pointLights;
	std::vector<ScatterGL::GLTexture> pointLightTextures;
	std::vector<glm::mat4*> pointLightMatrices;
};

int main()
{
	srand(static_cast <unsigned> (time(0)));
	stbi_set_flip_vertically_on_load(false);
	GLFWwindow* window = initWindow(info);
	ScatterGL::ScatterGLui myGui;
	myGui.init(window);

	ScatterGL::MeshObject fullScreenQuad = ScatterGL::MeshObject(ScatterGL::screenQuad, ScatterGL::screenQuadIndices);
	ScatterGL::BlockCollection blockCollection;
	PointLightCollection pointLightCollection;

	ScatterGL::MeshObject blueBlock = ScatterGL::MeshObject(ScatterGL::cube, ScatterGL::cubeIndices);
	ScatterGL::GLTexture blueTexture("Textures\\Nico.png");
	ScatterGL::GLTexture blueSpecTexture("Textures\\Nico.png");
	blockCollection.blocks.push_back(blueBlock);
	blockCollection.blockTextures.push_back(blueTexture);
	blockCollection.blockSpecTextures.push_back(blueSpecTexture);

	ScatterGL::MeshObject stijnBlock = ScatterGL::MeshObject(ScatterGL::cube, ScatterGL::cubeIndices);
	ScatterGL::GLTexture stijnTexture("Textures\\Stijn.png");
	ScatterGL::GLTexture specStijnTexture("Textures\\Stijn.png");
	blockCollection.blocks.push_back(stijnBlock);
	blockCollection.blockTextures.push_back(stijnTexture);
	blockCollection.blockSpecTextures.push_back(specStijnTexture);

	ScatterGL::MeshObject jeroenBlock = ScatterGL::MeshObject(ScatterGL::cube, ScatterGL::cubeIndices);
	ScatterGL::GLTexture jeroenTexture("Textures\\Jeroen.png");
	ScatterGL::GLTexture specJeroenTexture("Textures\\Jeroen.png");
	blockCollection.blocks.push_back(jeroenBlock);
	blockCollection.blockTextures.push_back(jeroenTexture);
	blockCollection.blockSpecTextures.push_back(specJeroenTexture);

	ScatterGL::MeshObject lightBlock = ScatterGL::MeshObject(ScatterGL::cube, ScatterGL::cubeIndices);
	ScatterGL::GLTexture lightTexture("Textures\\gianTwee.png");
	ScatterGL::GLTexture specLightTexture("Textures\\gianTwee.png");
	blockCollection.blocks.push_back(lightBlock);
	blockCollection.blockTextures.push_back(lightTexture);
	blockCollection.blockSpecTextures.push_back(specLightTexture);

	std::filesystem::path pathName("Models\\sponzaTwo\\sponza.gltf");
	ScatterGL::Model sponza(pathName.string());
	glm::mat4 sponzaMatrix = glm::mat4(1.0f);
	sponzaMatrix = glm::scale(sponzaMatrix, glm::vec3(0.05f, 0.05f, 0.05f));

	//Sphere making
	std::filesystem::path pathNameSphere("Models\\sphere\\sphere.obj");
	ScatterGL::Model sphereOne(pathNameSphere.string());
	glm::mat4 sphereMatrixOne = glm::mat4(1.0f);
	sphereMatrixOne = glm::scale(sphereMatrixOne, glm::vec3(0.01f, 0.01f, 0.01f));
	sphereMatrixOne = glm::translate(sphereMatrixOne, glm::vec3(-6000.0f, 777.7f, -2250.0f));
	pointLightCollection.pointLights.push_back(&sphereOne);
	pointLightCollection.pointLightMatrices.push_back(&sphereMatrixOne);

	ScatterGL::Model sphereTwo(pathNameSphere.string());
	glm::mat4 sphereMatrixTwo = glm::mat4(1.0f);
	sphereMatrixTwo = glm::scale(sphereMatrixTwo, glm::vec3(0.01f, 0.01f, 0.01f));
	sphereMatrixTwo = glm::translate(sphereMatrixTwo, glm::vec3(5800.0f, 777.7f, 2100.0f));
	pointLightCollection.pointLights.push_back(&sphereTwo);
	pointLightCollection.pointLightMatrices.push_back(&sphereMatrixTwo);

	ScatterGL::Shader depthShader;
	depthShader.initialize("Shaders\\depthShader.vert",
		"Shaders\\depthShader.frag");

	ScatterGL::Shader uberShader;
	uberShader.initialize("Shaders\\UberShader.vert",
		"Shaders\\UberShader.frag");

	ScatterGL::Shader shadowShader;
	shadowShader.initialize("Shaders\\postProcess.vert",
		"Shaders\\postProcess.frag");

	framebuffer.initialize(info.SCREEN_WIDTH, info.SCREEN_HEIGHT);
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

	std::vector<ScatterGL::Mesh> tempMeshCollection;
	for (unsigned int i = 0; i < sponza.getMeshes().size(); i++)
	{
		ScatterGL::Mesh& tempMesh = sponza.getMeshes()[i];
		tempMeshCollection.push_back(tempMesh);
		uint64_t tempMeshHandle = myScatter.addMesh(tempMesh.vertices.data(), tempMesh.indices.data(), tempMesh.vertices.size(), tempMesh.indices.size());
		meshHandlesSponza.push_back(tempMeshHandle);
		myScatter.addInstance(tempMeshHandle, glm::value_ptr(sponzaMatrix));
	}

	glm::mat4 cubeMatrix = glm::mat4(1.0);
	cubeMatrix = glm::scale(cubeMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
	for (unsigned int i = 0; i < blockCollection.blocks.size(); i++)
	{
		cubeMatrix = glm::translate(cubeMatrix, glm::vec3((float)i * 1.25f, (float)i + 0.5f, (float)i * 1.25f));
		ScatterGL::MeshObject& tempMesh = blockCollection.blocks[i];
		blockCollection.blockMatrices.push_back(cubeMatrix);
		glm::mat4 matrixNew = blockCollection.blockMatrices[i];
		matrixNew = glm::transpose(matrixNew);
		uint64_t tempMeshHandle = myScatter.addMesh(tempMesh.vertices.data(), tempMesh.indices.data(), tempMesh.vertices.size(), tempMesh.indices.size());
		meshHandlesBlocks.push_back(tempMeshHandle);
		myScatter.addInstance(tempMeshHandle, glm::value_ptr(matrixNew));
	}
	
	myScatter.build();
	framebuffer.attachTexture(framebuffer.depthTexture);
	ScatterGL::Framebuffer postProcess;
	postProcess.initialize(info.SCREEN_WIDTH, info.SCREEN_HEIGHT);

	glm::mat4 projection = glm::perspectiveRH(glm::radians(camera.zoom),
		(float)info.SCREEN_WIDTH / (float)info.SCREEN_HEIGHT,  info.nearPlane, info.farPlane);
	float xPositive = 0.05;
	float xNegative = -0.05;
	int frameCount = 0;
	float timeTraveled = 0.0;

	glm::vec3 lightPosSphere;
	lightPosSphere.x = sphereMatrixOne[3][0];
	lightPosSphere.y = sphereMatrixOne[3][1];
	lightPosSphere.z = sphereMatrixOne[3][2];
	glm::vec3 lightPosSphereTwo;
	lightPosSphereTwo.x = sphereMatrixTwo[3][0];
	lightPosSphereTwo.y = sphereMatrixTwo[3][1];
	lightPosSphereTwo.z = sphereMatrixTwo[3][2];

	while(!glfwWindowShouldClose(window))
	{
		//calculating time passed since last frame
		float currentFrame = glfwGetTime();
		info.deltaTime = currentFrame - info.lastFrame;
		info.lastFrame = currentFrame;
		float travelSpeed = 1200 * info.deltaTime;

		lightPosSphere.x = sphereMatrixOne[3][0];
		lightPosSphere.y = sphereMatrixOne[3][1];
		lightPosSphere.z = sphereMatrixOne[3][2];

		lightPosSphereTwo.x = sphereMatrixTwo[3][0];
		lightPosSphereTwo.y = sphereMatrixTwo[3][1];
		lightPosSphereTwo.z = sphereMatrixTwo[3][2];

		myScatter.clearInstances();
		for (unsigned int i = 0; i < sponza.getMeshes().size(); i++)
		{
			myScatter.addInstance(meshHandlesSponza[i], glm::value_ptr(sponzaMatrix));
		}
		for (unsigned int i = 0; i < blockCollection.blocks.size(); i++)
		{
			cubeMatrix = glm::translate(cubeMatrix, glm::vec3((float)i * 1.25f, (float)i + 0.5f, (float)i * 1.25f));
			glm::mat4 matrixNew = blockCollection.blockMatrices[i];
			matrixNew = glm::transpose(matrixNew);
			myScatter.addInstance(meshHandlesBlocks[i], glm::value_ptr(matrixNew));
		}
		myScatter.build();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, info.SCREEN_WIDTH, info.SCREEN_HEIGHT);

		// user input
		processInput(window, info);
		// setting view matrix
		glm::mat4 view = camera.getViewMatrix();
		// cube renderpass
		framebuffer.bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// setting shader
		uberShader.use();
		uberShader.setMat4("projection", projection);
		uberShader.setMat4("view", view);
		uberShader.setVec3("viewPosition", camera.position);
		if (timeTraveled < 10.0f)
		{
			sphereMatrixOne = glm::translate(sphereMatrixOne, glm::vec3(travelSpeed, 0.0, 0.0));
			sphereMatrixTwo = glm::translate(sphereMatrixTwo, glm::vec3(-travelSpeed, 0.0, 0.0));
			timeTraveled += info.deltaTime;
		}
		else
		{
			sphereMatrixOne = glm::translate(sphereMatrixOne, glm::vec3(-travelSpeed, 0.0, 0.0));
			sphereMatrixTwo = glm::translate(sphereMatrixTwo, glm::vec3(travelSpeed, 0.0, 0.0));
			timeTraveled += info.deltaTime;
			if (timeTraveled > 20.0f)
			{
				timeTraveled = 0.0;
			}
		}
		for (unsigned int i = 0; i < blockCollection.blocks.size(); i++)
		{
			if (frameCount > 1250)
			{
				xPositive *= -1;
				frameCount = 0;
			}
			if (i % 2)
			{
				blockCollection.blockMatrices[i] = glm::translate(blockCollection.blockMatrices[i], glm::vec3(xPositive, 0.0, 0.0));
			}
			else if (i % 3)
			{
				blockCollection.blockMatrices[i] = glm::rotate(blockCollection.blockMatrices[i], info.deltaTime, glm::vec3(0.05f, 0.0f, 0.05f));
			}
			uberShader.setMat4("model", blockCollection.blockMatrices[i]);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, blockCollection.blockTextures[i].texture);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, blockCollection.blockSpecTextures[i].texture);
			uberShader.setInt("material.diffuse", 0);
			uberShader.setInt("material.specular", 1);
			uberShader.setFloat("material.shine", 32.0f);
			uberShader.setVec3("light.direction", sunLight.direction);
			uberShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
			uberShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
			uberShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
			uberShader.setVec3("pointLight.position", lightPosSphere);
			blockCollection.blocks[i].drawObject();
		}
		//Starting PointLights!
		uberShader.setBool("isModel", true);
		uberShader.setInt("numberOfPointLights", pointLightCollection.pointLights.size());
		for (unsigned int i = 0; i < pointLightCollection.pointLights.size(); i++)
		{
			uberShader.setMat4("model", *pointLightCollection.pointLightMatrices[i]);
			uberShader.setFloat("pointLight.constant", lightOrb.constant);
			uberShader.setFloat("pointLight.linear", lightOrb.linear);
			uberShader.setFloat("pointLight.quadratic", lightOrb.quadratic);
			pointLightCollection.pointLights[i]->draw(uberShader);
		}
		// Starting Sponza!
		uberShader.setInt("texture_diffuse1", 0);
		uberShader.setMat4("model", sponzaMatrix);
		sponza.draw(uberShader);
		uberShader.setBool("isModel", false);
		frameCount++;
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
		fullScreenQuad.drawObject();
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
	glfwTerminate();
}

//float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
//cubeShader.setFloat("r", r);
//float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
//cubeShader.setFloat("g", g);
//float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
//cubeShader.setFloat("b", b);
		//std::cout << pointLightCollection.pointLights.size() << std::endl;
