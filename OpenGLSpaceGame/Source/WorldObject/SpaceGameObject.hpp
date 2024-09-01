#pragma once

#include "../Delta/Delta.hpp"
#include "WorldObject.hpp"
#include "../Model/Model.hpp"

class SpaceGameObject : public WorldObject
{
	public:
		SpaceGameObject(Model* model, glm::vec3 position, glm::vec3 velocity, glm::vec3 rotVelocity, glm::vec3 angle, glm::vec3 scale, GLuint projectionLocation, GLuint viewLocation, GLuint modelLocation, unordered_set<tag> tags);
		void addDelta(Delta<glm::vec3>* delta);
		void Tick() override;
		glm::vec3 velocity;
		glm::vec3 rotationalVelocity;
		Delta<glm::vec3>* velocityDelta;
		Delta<glm::vec3>* rotationalVelocityDelta;
	protected:
		vector<Delta<glm::vec3>*> deltas;
};

