#pragma once

#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glad/glad.h>

#include "myModel.hpp"

class MyModel
{
	public:
		MyModel();
		MyModel(size_t nPolygons, float *verts, unsigned char *polygonBoundaries, GLuint VAO);
		MyModel(std::string, GLint VAO);
		
		virtual void Draw();
	protected:
		GLuint *VBOs;
		size_t nPolygons; //number of my polygons
		unsigned char* polygonBoundaries; //number of verts in each polygon
};