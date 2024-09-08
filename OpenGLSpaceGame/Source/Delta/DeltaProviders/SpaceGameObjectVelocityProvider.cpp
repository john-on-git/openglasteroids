#include "SpaceGameObjectVelocityProvider.hpp"

SpaceGameObjectVelocityProvider::SpaceGameObjectVelocityProvider(SpaceGameObject* spaceGameObject)
{
	this->spaceGameObject = spaceGameObject;
}

glm::vec3 SpaceGameObjectVelocityProvider::Tick()
{
	return spaceGameObject->getVelocity();
}