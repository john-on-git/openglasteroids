#pragma once

#include <glm/ext/vector_float3.hpp>

class Delta
{
	public:
		glm::vec3* magnitude;
		long long duration;
		Delta();
		Delta(glm::vec3* now, glm::vec3* magnitude);
		Delta(glm::vec3* now, glm::vec3* magnitude, long long duration);
		Delta(Delta* now, glm::vec3* magnitude);
		Delta(Delta* now, glm::vec3* magnitude, long long duration);
		Delta(Delta* now, glm::vec3* magnitude, float maximumMagnitude, long long duration);
		~Delta();
		bool Tick();
	protected:
		glm::vec3* now;
		float maximum;
};

