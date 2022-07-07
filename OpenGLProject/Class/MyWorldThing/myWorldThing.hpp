#pragma once

#include "../Model/Model.hpp"
#include <glm/vec3.hpp>

/// <summary>
/// something with a model, position and direction
/// </summary>
class MyWorldThing {
	public:
		Model *model;
		glm::vec3 position;
		glm::vec3 angle; //mag, x, y, z
		glm::vec3 scale;

		MyWorldThing();
		MyWorldThing(Model *model, glm::vec3 position, glm::vec3 angle, glm::vec3 scale, GLuint projectionLocation, GLuint viewLocation, GLuint modelLocation);

		void Draw();
	protected:
		GLuint projectionLocation;
		GLuint viewLocation;
		GLuint modelLocation;
};