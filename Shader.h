#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
class Shader
{
public:
	//program ID
	unsigned int iD;
	//Constructs, reads, and builds the shader
	Shader(const GLchar * vertexPath, const GLchar * fragmentPath);
	//activates the shader
	void use();
	//utility uniform functions
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setVec3(const char * name, float one, float two, float three);
	void setMatrix4(const char * name, glm::mat4);
	void Shader::setVec3(const char * name, glm::vec3 & vector);
};

Shader::Shader(const GLchar * vertexPath, const GLchar * fragmentPath)
{
	//1. Read in Code from File
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		//open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		//read files contents
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		//close file handlers
		vShaderFile.close();
		fShaderFile.close();
		//convert stream to string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char * vShaderCode = vertexCode.c_str();
	const char * fShaderCode = fragmentCode.c_str();
	//2. Compile Shaders
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];
	//vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	//print compile errors
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	//print compile errors
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//shader program
	iD = glCreateProgram();
	glAttachShader(iD, vertex);
	glAttachShader(iD, fragment);
	glLinkProgram(iD);
	glGetProgramiv(iD, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(iD, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}
void Shader::use()
{
	glUseProgram(iD);
}
void Shader::setBool(const std::string & name, bool value) const
{
	glUniform1i(glGetUniformLocation(iD, name.c_str()), (int)value);
}
void Shader::setInt(const std::string & name, int value) const
{
	glUniform1i(glGetUniformLocation(iD, name.c_str()), value);
}
void Shader::setFloat(const std::string & name, float value) const
{
	glUniform1f(glGetUniformLocation(iD, name.c_str()), value);
}
void Shader::setVec3(const char * name, float one, float two, float three)
{
	glUniform3fv(glGetUniformLocation(iD, name), 1, glm::value_ptr(glm::vec3(one, two, three)));
}
void Shader::setVec3(const char * name, glm::vec3 & vector)
{
	glUniform3fv(glGetUniformLocation(iD, name), 1, glm::value_ptr(vector));
}
void Shader::setMatrix4(const char * name, glm::mat4 matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(iD, name), 1, GL_FALSE, glm::value_ptr(matrix));
}