#pragma once

#include "../Model/Model.hpp"
#include <glm/vec3.hpp>
#include <vector>
#include "../../tag.hpp"

using namespace std;

/// <summary>
/// something with a model, position and direction
/// </summary>
class WorldObject {
	public:
		Model *model;
		glm::vec3 position;
		glm::vec3 angle;
		glm::vec3 scale;
		bool markedForDelete;
		vector<tag>* tags;

		WorldObject();
		WorldObject(Model *model, glm::vec3 position, glm::vec3 angle, glm::vec3 scale, GLuint projectionLocation, GLuint viewLocation, GLuint modelLocation, vector<tag>* tags);
		~WorldObject();
		void Draw();
	protected:
		GLuint projectionLocation;
		GLuint viewLocation;
		GLuint modelLocation;
};