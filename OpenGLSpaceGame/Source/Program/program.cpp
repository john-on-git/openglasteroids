#pragma once

#include "program.hpp"

Program::Program(std::string fragShaderPath, std::string vertShaderPath)
{
	GLint fragShaderHandle, vertShaderHandle, success;
	GLchar const* fragShaderSource, * vertShaderSource;
	handle = -1, fragShaderHandle = -1, vertShaderHandle = -1;

	//read from files
	std::ifstream fragSourceFile(fragShaderPath, std::fstream::in);
	if (fragSourceFile.is_open())
	{
		std::string fragSourceString = std::string(std::istreambuf_iterator<char>(fragSourceFile), std::istreambuf_iterator<char>());
		fragShaderSource = fragSourceString.c_str();

		std::ifstream vertSourceFile(vertShaderPath, std::fstream::in);
		if (vertSourceFile.is_open())
		{
			std::string vertSourceString = std::string(std::istreambuf_iterator<char>(vertSourceFile), std::istreambuf_iterator<char>());
			vertShaderSource = vertSourceString.c_str();

			//compile frag
			success = GL_FALSE;
			fragShaderHandle = glad_glCreateShader(GL_FRAGMENT_SHADER);
			glad_glShaderSource(fragShaderHandle, 1, &fragShaderSource, NULL);
			glad_glCompileShader(fragShaderHandle);
			glad_glGetShaderiv(fragShaderHandle, GL_COMPILE_STATUS, &success);
			if (success == GL_FALSE)
				OutputDebugStringW(L"WARN: failed to compile shader\n");

			//compile vert
			success = GL_FALSE;
			vertShaderHandle = glad_glCreateShader(GL_VERTEX_SHADER);
			glad_glShaderSource(vertShaderHandle, 1, &vertShaderSource, NULL);
			glad_glCompileShader(vertShaderHandle);
			glad_glGetShaderiv(vertShaderHandle, GL_COMPILE_STATUS, &success);
			if (success == GL_FALSE)
				OutputDebugStringW(L"WARN: failed to compile shader\n");

			//merge into program
			handle = glad_glCreateProgram();
			glad_glAttachShader(handle, fragShaderHandle);
			glad_glAttachShader(handle, vertShaderHandle);
			glad_glLinkProgram(handle);
			glad_glValidateProgram(handle);

			//delete temp shaders
			glad_glDeleteShader(fragShaderHandle);
			glad_glDeleteShader(vertShaderHandle);
		}
	}
}

void Program::Use()
{
	glad_glUseProgram(handle);
}