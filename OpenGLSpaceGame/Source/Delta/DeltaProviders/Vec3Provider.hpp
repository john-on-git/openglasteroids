#pragma once

#include "../DeltaProvider.hpp"
#include <glm/vec3.hpp>

class Vec3Provider : public DeltaProvider<glm::vec3>
{
	public:
		Vec3Provider(glm::vec3* source);
		glm::vec3 Tick();
	protected:
		glm::vec3* source;
};