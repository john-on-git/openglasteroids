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
		static unsigned long int totalVertices; //counter for total required for buffer

		unsigned long int startVertices; //start of my verts in the VBO
		unsigned long int nVertices; //number of my verts in the vbo

		MyModel();
		MyModel(float verts[], unsigned int nVerts);
		MyModel(std::string);
		
		void Buffer();
	private:
		float(*verts)[4];
};