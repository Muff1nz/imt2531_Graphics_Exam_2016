#include "ShaderHandler.h"

#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>


#include <stdlib.h>
#include <string.h>

const std::string shaderPath = "./resources/shaders/";

//Loads all of my shaders
ShaderHandler::ShaderProgram* ShaderHandler::initializeShaders() { 
	this->shaders["particle"] = std::unique_ptr<ShaderProgram>(new ShaderProgram("particle"));
	this->shaders["skyboxShader"] = std::unique_ptr<ShaderProgram>(new ShaderProgram("skyBoxShader"));
	this->shaders["shadowPhongTexture"] = std::unique_ptr<ShaderProgram>(new ShaderProgram("shadowPhongTexture"));
	this->shaders["shadowPhong"] = std::unique_ptr<ShaderProgram>(new ShaderProgram("shadowPhong"));
	this->shaders["shadow"] = std::unique_ptr<ShaderProgram>(new ShaderProgram("shadow"));

	return this->shaders["shadowPhong"].get(); 
}

ShaderHandler::ShaderProgram* ShaderHandler::getShader(const std::string& shader)
{
	auto it = this->shaders.find(shader);
	if (it != this->shaders.end())
	{
		return this->shaders[shader].get();
	}

	return nullptr;
}

ShaderHandler::ShaderProgram::ShaderProgram(const std::string& shader)
{
	const std::string vertexSuffix = ".vs";
	const std::string fragmentSuffix = ".fs";

	this->programId = LoadShaders((shaderPath + shader + vertexSuffix).c_str(), (shaderPath + shader + fragmentSuffix).c_str());
	findUniforms((shaderPath + shader + vertexSuffix).c_str());
	findUniforms((shaderPath + shader + fragmentSuffix).c_str());

	typedef std::map<std::string, GLuint>::iterator it;

	printf("\n%d uniforms found! listing...\n", uniforms.size());
	for (it iterator = uniforms.begin(); iterator != uniforms.end(); iterator++){
		printf("%s\n", iterator->first.c_str());
	}
	printf("\n");

	glUseProgram(this->programId);
}

ShaderHandler::ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(this->programId);
}

//Parses shader files to locate and get uniform locations automatically.
void ShaderHandler::ShaderProgram::findUniforms(const char * shaderFile) {
	std::ifstream VertexShaderStream(shaderFile, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string line = "", name;
		while (getline(VertexShaderStream, line)) {
			int start, end = 0;
			if (line.find("uniform") != std::string::npos) {
				while (line[end] != ';') {
					//User can define multiple uniforms in one line
					end = (line.find(',', end+1) != std::string::npos) ? line.find(',', end+1) : line.find(';', end+1);
					start = end; //Find semicolon, then backtrack until first space
					while (line[--start] != ' ')
						if (line[start] == '[') end = start; //Dont want array brackets in name
					start++;
					name = line.substr(start, end - start).c_str();
					uniforms[name] = glGetUniformLocation(programId, name.c_str());

					if (line[end] == '[')
						end = (line.find(',', end + 1) != std::string::npos) ? line.find(',', end + 1) : line.find(';', end + 1);
				}
			}			
		}
	}
}

GLuint ShaderHandler::ShaderProgram::LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	} else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}



	GLint Result = GL_FALSE;
	int InfoLogLength;



	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}


	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}