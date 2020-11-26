#pragma once
#include "pch.h"

namespace ScatterGL
{

class Shader
{
private:


public:
	//program id
	unsigned int ID;
	//constructor that reads and builds the shader
	void initialize(const char* vertexPath, const char* fragmentPath);
	// use/activate the shader
	void use();
	//uniform functionality
	void setBool(const std::string& name, bool value)const;
	void setInt(const std::string& name, int value)const;
	void setFloat(const std::string& name, float value)const;
	void setMat4(const std::string& name, glm::mat4& value)const;

};

}
