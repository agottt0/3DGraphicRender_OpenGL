#pragma once

#ifndef SHADER_H
#define SHADER_H

#include "iostream"
#include "string"
#include "fstream"
#include "sstream"

#include "glad/glad.h"
#include "glm/glm.hpp"

class Shader
{
public:
	unsigned int ID;

	Shader(const char* VertexPath, const char* FragmentPath)
	{
		// retrieve the vertex/fragment source code from filePath
		std::string VertexCode;
		std::string FragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			// open files
			vShaderFile.open(VertexPath);
			fShaderFile.open(FragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			//close
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			VertexCode = vShaderStream.str();
			FragmentCode = fShaderStream.str();
		}
		catch(std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
		}
		const char* vShaderCode = VertexCode.c_str();
		const char* fShaderCode = FragmentCode.c_str();

		// complie shader
		unsigned int vertexshader;
		vertexshader = glCreateShader(GL_VERTEX_SHADER);
		// create shader object, GL_VERTEX_SHADER is parameter to pass
		glShaderSource(vertexshader, 1, &vShaderCode, NULL);
		glCompileShader(vertexshader);
		// attach source to shader object and compile it

		int success;
		char InfoLog[512];
		glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexshader, 512, NULL, InfoLog);
			std::cout << "Error::Shader::Vertex::CompilationFailed\n" << InfoLog << std::endl;
		}

		unsigned int fragmentshader;
		fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentshader, 1, &fShaderCode, NULL);
		glCompileShader(fragmentshader);
		//compile fragmentshader
		glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentshader, 512, NULL, InfoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << InfoLog << std::endl;
		}

		// ID is programShader
		ID = glCreateProgram();
		glAttachShader(ID, vertexshader);
		glAttachShader(ID, fragmentshader);
		glLinkProgram(ID);
		// use programshader link multiple shaders
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, InfoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << InfoLog << std::endl;
		}

		glDeleteShader(vertexshader);
		glDeleteShader(fragmentshader);
		// delete two shader after link them

	}
	void use()
	{
		glUseProgram(ID);	
	}
	void SetBool(const std::string& name, bool value)const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void SetInt(const std::string& name, int value)const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void SetFloat(const std::string& name, float value)const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void SetVec2(const std::string& name, const glm::vec2 &value)const
	{
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void SetVec2(const std::string& name, float x, float y)const
	{
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}
	// ------------------------------------------------------------------------
	void setVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec4(const std::string& name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	void setMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

private:
	void checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}

};

#endif // !SHADER_H