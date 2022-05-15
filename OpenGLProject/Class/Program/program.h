#pragma once

#include <iostream>
#include <fstream>
#include <windows.h>
#include <glad/glad.h>
#include <string>

class Program
{
	public:
		GLint handle;

		Program(std::string fragShaderPath, std::string vertShaderPath);

		void Use();
};