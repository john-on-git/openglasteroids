#pragma once
#include "../WorldObject/SpaceGameObject.hpp"

 class TemporarySpaceGameObject : public SpaceGameObject
{
	public:
		void Tick() override;
		TemporarySpaceGameObject(Model* model, glm::vec3 position, glm::vec3 velocity, glm::vec3 rotVelocity, glm::vec3 angle, glm::vec3 scale, GLuint projectionLocation, GLuint viewLocation, GLuint modelLocation, unordered_set<tag> tags, long long int duration);
	private:
		long long int ticksRemaining;
};