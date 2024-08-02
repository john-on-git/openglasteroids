#pragma once
#include "../../WorldObject/WorldObject.hpp"
#include "../DeltaTarget.hpp"

class WorldObjectAngleTarget : public DeltaTarget<glm::vec3>
{
	public:
		WorldObjectAngleTarget(WorldObject* worldObject);
		void Tick(glm::vec3 change, float maximumMagnitude);
	protected:
		WorldObject* worldObject;
};