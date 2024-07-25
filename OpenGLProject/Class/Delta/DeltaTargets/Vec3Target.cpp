#include "Vec3Target.hpp"
#include <glm/glm.hpp>

Vec3Target::Vec3Target(glm::vec3* target)
{
	this->target = target;
}

void Vec3Target::Tick(glm::vec3 change, float maximumMagnitude)
{
	auto next = *this->target + change;
	auto nextMag = glm::length(next);
	if (!(maximumMagnitude < 0 || nextMag <= maximumMagnitude)) //resize the vector to limit it within range
	{
		next *= maximumMagnitude / nextMag;
	}
	*this->target = next;
}