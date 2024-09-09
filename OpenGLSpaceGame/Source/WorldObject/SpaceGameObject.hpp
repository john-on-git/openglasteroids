#pragma once

#include "../Delta/Delta.hpp"
#include "WorldObject.hpp"
#include "../Model/Model.hpp"

class SpaceGameObject : public WorldObject
{
	public:
		SpaceGameObject(Model* model, glm::vec3 position, glm::vec3 velocity, glm::vec3 rotVelocity, glm::vec3 angle, glm::vec3 scale, GLuint modelViewLocation, unordered_set<tag> tags);
		glm::vec3 getVelocity();
		void setVelocity(glm::vec3 velocity);
		glm::vec3 getRotationalVelocity();
		void setRotationalVelocity(glm::vec3 rotationalVelocity);
		void addDelta(Delta<glm::vec3>* delta);
		void Tick() override;
		Delta<glm::vec3>* velocityDelta;
		Delta<glm::vec3>* rotationalVelocityDelta;
	protected:
		glm::vec3 velocity;
		glm::vec3 rotationalVelocity;
		vector<Delta<glm::vec3>*> deltas;
};

