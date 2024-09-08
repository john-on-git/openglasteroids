#include "RotDragProvider.hpp"



RotDragProvider::RotDragProvider(SpaceGameObject* target)
{
	this->target = target;
	mult = 1.0f;
}
RotDragProvider::RotDragProvider(SpaceGameObject* target, float mult)
{
	this->target = target;
	this->mult = mult;
}

glm::vec3 RotDragProvider::Tick()
{
	glm::vec3 targetRotVel = target->getRotationalVelocity();
	return glm::vec3(
		-SHIP_ROT_DRAG_COEFFICIENT * mult * targetRotVel.x,
		-SHIP_ROT_DRAG_COEFFICIENT * mult * targetRotVel.y,
		-SHIP_ROT_DRAG_COEFFICIENT * mult * targetRotVel.z
	);
}