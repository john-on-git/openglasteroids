#include "SpaceGameObjectRotationalVelocityTarget.hpp"

SpaceGameObjectRotationalVelocityTarget::SpaceGameObjectRotationalVelocityTarget(SpaceGameObject* spaceGameObject)
{
	this->spaceGameObject = spaceGameObject;
}

void SpaceGameObjectRotationalVelocityTarget::Tick(glm::vec3 change, float maximumMagnitude)
{
	auto next = this->spaceGameObject->getRotationalVelocity() + change;
	auto nextMag = glm::length(next);
	if (!(maximumMagnitude < 0 || nextMag <= maximumMagnitude)) //resize the vector to limit it within range
	{
		next *= maximumMagnitude / nextMag;
	}
	this->spaceGameObject->setRotationalVelocity(next);
}