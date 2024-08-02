#include "WorldObjectPositionTarget.hpp"

WorldObjectPositionTarget::WorldObjectPositionTarget(WorldObject* worldObject)
{
	this->worldObject = worldObject;
}

void WorldObjectPositionTarget::Tick(glm::vec3 change, float maximumMagnitude)
{
	auto next = this->worldObject->getPosition() + change;
	auto nextMag = glm::length(next);
	if (!(maximumMagnitude < 0 || nextMag <= maximumMagnitude)) //resize the vector to limit it within range
	{
		next *= maximumMagnitude / nextMag;
	}
	this->worldObject->setPosition(next);
}