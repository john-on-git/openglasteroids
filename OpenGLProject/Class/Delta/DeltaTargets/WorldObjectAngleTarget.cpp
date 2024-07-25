#include "WorldObjectAngleTarget.hpp"

WorldObjectAngleTarget::WorldObjectAngleTarget(WorldObject* worldObject)
{
	this->worldObject = worldObject;
}

void WorldObjectAngleTarget::Tick(glm::vec3 change, float maximumMagnitude)
{
	auto next = this->worldObject->getAngle() + change;
	auto nextMag = glm::length(next);
	if (!(maximumMagnitude < 0 || nextMag <= maximumMagnitude)) //resize the vector to limit it within range
	{
		next *= maximumMagnitude / nextMag;
	}
	this->worldObject->setAngle(next);
}