#pragma once
#include "../DeltaProvider.hpp"
#include <glm/vec3.hpp>

constexpr auto SHIP_DRAG_COEFFICIENT = 0.02f;

class DragProvider : public DeltaProvider<glm::vec3>
{
	public:
		DragProvider(glm::vec3* shipVelocity);
		glm::vec3 Tick();
	protected:
		glm::vec3* shipVelocity;
};