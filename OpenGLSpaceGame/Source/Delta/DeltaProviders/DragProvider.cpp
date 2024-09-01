#include "DragProvider.hpp"



DragProvider::DragProvider(glm::vec3* target)
{
	this->target = target;
	mult = 1.0f;
}
DragProvider::DragProvider(glm::vec3* target, float mult)
{
	this->target = target;
	this->mult = mult;
}

glm::vec3 DragProvider::Tick()
{
	return glm::vec3(
		-SHIP_DRAG_COEFFICIENT * mult * target->x,
		-SHIP_DRAG_COEFFICIENT * mult * target->y,
		-SHIP_DRAG_COEFFICIENT * mult * target->z
	);
}