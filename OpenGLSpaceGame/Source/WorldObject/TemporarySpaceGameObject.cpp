#include "TemporarySpaceGameObject.hpp"

void TemporarySpaceGameObject::Tick()
{
	if (ticksRemaining > 0)
	{
		ticksRemaining--;
	}
	else
	{
		markedForDelete = true;
	}
	this->SpaceGameObject::Tick();
}

TemporarySpaceGameObject::TemporarySpaceGameObject(Model* model, glm::vec3 position, glm::vec3 velocity, glm::vec3 rotVelocity, glm::vec3 angle, glm::vec3 scale, GLuint projectionLocation, GLuint viewLocation, GLuint modelLocation, unordered_set<tag> tags, long long int duration) : SpaceGameObject(model, position, velocity, rotVelocity, angle, scale, projectionLocation, viewLocation, modelLocation, tags)
{
	ticksRemaining = duration;
}