#pragma once

#include "../Model/Model.hpp"
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <vector>
#include "../tag.hpp"

using namespace std;

/// <summary>
/// something with a model, position and direction
/// </summary>
class WorldObject {
	public:
		Model *model;
		bool markedForDelete;
		unordered_set<tag> tags;
		glm::mat4 modelMatrix;
		glm::mat4 rotationMatrix;

		WorldObject(Model *model, glm::vec3 position, glm::vec3 angle, glm::vec3 scale, GLuint projectionLocation, GLuint viewLocation, GLuint modelLocation, unordered_set<tag> tags);
		virtual void Tick();
		void Draw();
		glm::vec3 getPosition();
		void setPosition(glm::vec3 position);
		glm::vec3 getAngle();
		void setAngle(glm::vec3 angle);
		glm::vec3 getScale();
		void setScale(glm::vec3 scale);
		glm::vec3* getOrientedBoundingBox();
		std::vector<glm::vec4>* calcFaces();
		std::vector<glm::mat2x4>* calcEdges();
	protected:
		glm::vec3 position;
		glm::vec3 angle;
		glm::vec3 scale;
		GLuint projectionLocation;
		GLuint viewLocation;
		GLuint modelLocation;
		glm::vec3* boundingBox;
		glm::vec3 boundingBoxAngle; //angle at which the current value of boundingBox^ was calculated
		void UpdateModelMatrix();
};