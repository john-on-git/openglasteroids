#pragma once

#include <glm/ext/vector_float2.hpp>
#include <string>
#include <glad/glad.h>

#include "../MyModel/myModel.hpp"
#include "myWorldThing.hpp"

/// <summary>
/// something with a model, position and direction
/// </summary>
class MyWorldThing {
	public:
		MyModel *model;
		glm::vec3 position;
		glm::vec3 angle; //mag, x, y, z

		MyWorldThing();
		MyWorldThing(MyModel *model, glm::vec3 position, glm::vec3 angle, GLuint projectionLocation, GLuint viewLocation, GLuint modelLocation);

		void Draw();
	protected:
		GLuint projectionLocation;
		GLuint viewLocation;
		GLuint modelLocation;
};