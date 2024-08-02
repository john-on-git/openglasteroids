#pragma once
#include "../DeltaTarget.hpp"
#include <glm/vec3.hpp>

class Vec3Target : public DeltaTarget<glm::vec3>
{
	public:
		Vec3Target(glm::vec3* target);
		void Tick(glm::vec3, float maximumMagnitude);
	protected:
		glm::vec3* target;
};