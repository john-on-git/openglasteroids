#pragma once

#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glad/glad.h>

#include "myModel.h"

class MyModel
{
	public:
		unsigned long int nVerts; //number of my verts in the vbo

		MyModel();
		MyModel(float vertsIn[], unsigned int lenVertsIn, GLint VAO);
		MyModel(std::string, GLint VAO);
		
		void Draw();
	private:
		GLuint *VBOs;
};