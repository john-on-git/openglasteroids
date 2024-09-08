#include "DragProvider.hpp"



DragProvider::DragProvider(SpaceGameObject* target)
{
	this->target = target;
	mult = 1.0f;
}
DragProvider::DragProvider(SpaceGameObject* target, float mult)
{
	this->target = target;
	this->mult = mult;
}

glm::vec3 DragProvider::Tick()
{
	glm::vec3 targetVel = target->getVelocity();
	return glm::vec3(
		-SHIP_DRAG_COEFFICIENT * mult * targetVel.x,
		-SHIP_DRAG_COEFFICIENT * mult * targetVel.y,
		-SHIP_DRAG_COEFFICIENT * mult * targetVel.z
	);
}