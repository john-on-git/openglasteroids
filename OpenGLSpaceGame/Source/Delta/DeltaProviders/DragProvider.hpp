#pragma once
#include "../DeltaProvider.hpp"
#include "../../WorldObject/SpaceGameObject.hpp"

constexpr auto SHIP_DRAG_COEFFICIENT = 0.02f;

class DragProvider : public DeltaProvider<glm::vec3>
{
	public:
		DragProvider(SpaceGameObject* target);
		DragProvider(SpaceGameObject* target, float mult);
		glm::vec3 Tick();
	protected:
		SpaceGameObject* target;
		float mult;
};