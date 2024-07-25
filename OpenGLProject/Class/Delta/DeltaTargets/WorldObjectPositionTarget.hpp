#pragma once
#include "../../WorldObject/WorldObject.hpp"
#include "../DeltaTarget.hpp"

class WorldObjectPositionTarget : public DeltaTarget<glm::vec3>
{
	public:
		WorldObjectPositionTarget(WorldObject* worldObject);
		void Tick(glm::vec3 change, float maximumMagnitude);
	protected:
		WorldObject* worldObject;
};