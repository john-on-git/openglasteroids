#include "DragProvider.hpp"

DragProvider::DragProvider(glm::vec3* shipVelocity)
{
	this->shipVelocity = shipVelocity;
}

glm::vec3 DragProvider::Tick()
{
	return glm::vec3(
		-SHIP_DRAG_COEFFICIENT * shipVelocity->x,
		-SHIP_DRAG_COEFFICIENT * shipVelocity->y,
		-SHIP_DRAG_COEFFICIENT * shipVelocity->z
	);
}