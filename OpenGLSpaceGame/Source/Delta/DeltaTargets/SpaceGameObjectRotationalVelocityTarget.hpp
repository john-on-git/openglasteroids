#pragma once
#include "../../WorldObject/SpaceGameObject.hpp"
#include "../DeltaTarget.hpp"

class SpaceGameObjectRotationalVelocityTarget : public DeltaTarget<glm::vec3>
{
	public:
		SpaceGameObjectRotationalVelocityTarget(SpaceGameObject* worldObject);
		void Tick(glm::vec3 change, float maximumMagnitude);
	protected:
		SpaceGameObject* spaceGameObject;
};