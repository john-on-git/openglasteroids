#pragma once
#include "../../WorldObject/SpaceGameObject.hpp"
#include "../DeltaTarget.hpp"

class SpaceGameObjectVelocityProvider : public DeltaProvider<glm::vec3>
{
	public:
		SpaceGameObjectVelocityProvider(SpaceGameObject* worldObject);
		glm::vec3 Tick();
	protected:
		SpaceGameObject* spaceGameObject;
};