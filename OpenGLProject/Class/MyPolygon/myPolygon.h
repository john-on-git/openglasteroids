#pragma once

#include <glm/ext/vector_float2.hpp>
#include <string>
#include <glad/glad.h>

#include "../MyModel/myModel.h"
#include "myPolygon.h"

class MyPolygon {
	public:
		MyModel model;
		glm::vec2 position;
		float angle;

		MyPolygon();
		MyPolygon(float verts[], unsigned int length, glm::vec2 position, float angle);
		MyPolygon(std::string modelPath, glm::vec2 position, float angle);

		void Draw(GLint projectionLocation, GLint viewLocation, GLint modelLocation);
	private:	
};