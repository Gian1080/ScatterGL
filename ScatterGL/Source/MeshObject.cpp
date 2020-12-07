#include "MeshObject.h"

//std::vector<float> vertices;
//std::vector<float> indices;
//int index;
//bool normalized;
//uint8_t stride;
//const void* vertDataSize;

namespace ScatterGL
{
	MeshObject::MeshObject(std::vector<float>& verticesTMP, uint8_t strideTMP) :
		vertices(verticesTMP),
		stride(strideTMP)
	{
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		//indices object
		//vertex buffer object
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
		//vertex descriptor
		glVertexAttribPointer(0, stride, GL_FLOAT, GL_FALSE, (stride * 2) * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, stride, GL_FLOAT, GL_FALSE, (stride * 2) * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	void MeshObject::drawObject()
	{
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	void MeshObject::destroyObject()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}
}