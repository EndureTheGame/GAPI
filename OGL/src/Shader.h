#pragma once

#include <iostream>
#include <String>
#include <fstream>
#include <sstream>

#include "Renderer.h"

#include "Shader.h"


struct ShaderProgramSource
{
	std::string VertexShader;
	std::string FragmentShader;
};

class Shader
{

private:
	std::string m_FilePath;
	unsigned int m_RendererID;

	public:
		Shader(const std::string& filepath);
		~Shader();

		void Bind() const;
		void Unbind() const;

		ShaderProgramSource ParseShader(const std::string& filePath);

		void Setuniform4f(const std::string& name, float v0, float v1, float v2, float v3);

	private:
		unsigned int CompileShader(unsigned int type, const std::string& source);
		unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
		int GetUniformLocation(const std::string& name);
};