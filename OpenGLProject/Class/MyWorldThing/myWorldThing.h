#pragma once

#include <glm/ext/vector_float2.hpp>
#include <string>
#include <glad/glad.h>

#include "../MyModel/myModel.h"
#include "myWorldThing.h"

/// <summary>
/// something with a model, position and direction
/// </summary>
class MyWorldThing {
	public:
		MyModel model;
		glm::vec3 position;
		glm::vec3 angle; //mag, x, y, z

		MyWorldThing();
		MyWorldThing(float verts[], unsigned int length, glm::vec3 position, glm::vec3 angle, GLint VAO);
		MyWorldThing(std::string modelPath, glm::vec3 position, glm::vec3 angle, GLint VAO);

		void Draw(GLint projectionLocation, GLint viewLocation, GLint modelLocation);
	private:	
};