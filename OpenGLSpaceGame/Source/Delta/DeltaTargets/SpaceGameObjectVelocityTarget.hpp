#pragma once
#include "../../WorldObject/SpaceGameObject.hpp"
#include "../DeltaTarget.hpp"

class SpaceGameObjectVelocityTarget : public DeltaTarget<glm::vec3>
{
	public:
		SpaceGameObjectVelocityTarget(SpaceGameObject* worldObject);
		void Tick(glm::vec3 change, float maximumMagnitude);
	protected:
		SpaceGameObject* spaceGameObject;
};