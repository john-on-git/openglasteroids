#pragma once
#include "../DeltaProvider.hpp"
#include "../../WorldObject/SpaceGameObject.hpp"

constexpr auto SHIP_ROT_DRAG_COEFFICIENT = 0.02f;

class RotDragProvider : public DeltaProvider<glm::vec3>
{
	public:
		RotDragProvider(SpaceGameObject* target);
		RotDragProvider(SpaceGameObject* target, float mult);
		glm::vec3 Tick();
	protected:
		SpaceGameObject* target;
		float mult;
};