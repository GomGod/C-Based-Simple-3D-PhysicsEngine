#ifndef SHADER_H
#define SHADER_H

#include<glad/glad.h>

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	unsigned int ID;//shader program�� id
	
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath); //shader�� �а� ����
	
	void use();// shader�� Ȱ��ȭ�ϰ� ���

	//utils
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, glm::mat4& value) const;
	void setVec3(const std::string& name, float v1, float v2, float v3) const;
};

#endif