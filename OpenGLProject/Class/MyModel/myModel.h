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
		unsigned int nVertices;

		MyModel();
		MyModel(float verts[], unsigned int nVerts);
		MyModel(std::string);

		void BindBuffer(); //bind this model's buffer
	private:
		GLuint VBO;

		void Buffer(); //set up this model's buffer
};