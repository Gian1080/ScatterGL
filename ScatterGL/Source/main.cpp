#include "pch.h"
#include "Shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

GLFWwindow* initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "ScatterGL", NULL, NULL);
	if (window == NULL)
	{
		throw std::runtime_error("failed to create window \n");
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::runtime_error("failed to initialize GLAD \n");
	}
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	return window;
}

int main()
{
	GLFWwindow* window = initWindow();

	unsigned int textureWood;
	{
		int width, height, nrChannels;
		glGenTextures(1, &textureWood);
		glBindTexture(GL_TEXTURE_2D, textureWood);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		unsigned char* data = stbi_load("Textures\\container.jpg", &width, &height, &nrChannels, 0);
		std::cout << nrChannels << " one "  << std::endl;
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			std::cout << "WOOD \n";
		}
		else
		{
			std::cout << "Failed to load texture 1 \n";
		}
		stbi_image_free(data);
	}

	unsigned int textureFace;
	{
		int width, height, nrChannels;
		glGenTextures(1, &textureFace);
		glBindTexture(GL_TEXTURE_2D, textureFace);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		unsigned char* dataTwo = stbi_load("Textures\\awesomeface.png", &width, &height, &nrChannels, 0);
		std::cout << nrChannels << " two " <<std::endl;
		if (dataTwo)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataTwo);
			glGenerateMipmap(GL_TEXTURE_2D);
			std::cout << "FACE \n";
		}
		else
		{
			std::cout << "Failed to load texture 2 \n";
		}
		stbi_image_free(dataTwo);
	}
	ScatterGL::Shader someShader;
	someShader.initialize("Shaders\\VertexShader.vert",
							"Shaders\\FragmentShader.frag");

	float verticesNew[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	float verticesSquare[] = {
	 0.5f,  0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f };   

	unsigned int indicesSquare[] = { 0, 1, 3, 1, 2, 3 };

	float texCoords[] =
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.5f, 1.0f
	};

	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	float verticesTriangle[] = { 
				//positions			//colors
				-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
				0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f };

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesNew), verticesNew, GL_STATIC_DRAW);

	//attribute descriptor for vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//attribute descriptor for colors
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);
	//attribute descriptor for texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	someShader.use();

	while(!glfwWindowShouldClose(window))
	{
		// user input
		processInput(window);
		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//render square
		glBindTextureUnit(0, textureWood);
		glBindTextureUnit(1, textureFace);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
}