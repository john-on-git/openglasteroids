#include "SpaceGameObjectVelocityTarget.hpp"

SpaceGameObjectVelocityTarget::SpaceGameObjectVelocityTarget(SpaceGameObject* spaceGameObject)
{
	this->spaceGameObject = spaceGameObject;
}

void SpaceGameObjectVelocityTarget::Tick(glm::vec3 change, float maximumMagnitude)
{
	auto next = this->spaceGameObject->getVelocity() + change;
	auto nextMag = glm::length(next);
	if (!(maximumMagnitude < 0 || nextMag <= maximumMagnitude)) //resize the vector to limit it within range
	{
		next *= maximumMagnitude / nextMag;
	}
	this->spaceGameObject->setVelocity(next);
}