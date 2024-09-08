#pragma once
#include "../../WorldObject/SpaceGameObject.hpp"
#include "../DeltaTarget.hpp"

class SpaceGameObjectRotationalVelocityProvider : public DeltaProvider<glm::vec3>
{
	public:
		SpaceGameObjectRotationalVelocityProvider(SpaceGameObject* worldObject);
		glm::vec3 Tick();
	protected:
		SpaceGameObject* spaceGameObject;
};