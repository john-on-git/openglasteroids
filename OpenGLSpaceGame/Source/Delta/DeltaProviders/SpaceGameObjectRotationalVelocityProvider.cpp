#include "SpaceGameObjectRotationalVelocityProvider.hpp"

SpaceGameObjectRotationalVelocityProvider::SpaceGameObjectRotationalVelocityProvider(SpaceGameObject* spaceGameObject)
{
	this->spaceGameObject = spaceGameObject;
}

glm::vec3 SpaceGameObjectRotationalVelocityProvider::Tick()
{
	return spaceGameObject->getRotationalVelocity();
}