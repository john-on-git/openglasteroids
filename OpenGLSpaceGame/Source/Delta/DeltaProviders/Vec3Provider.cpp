#include "Vec3Provider.hpp"

Vec3Provider::Vec3Provider(glm::vec3* source)
{
	this->source = source;
}

glm::vec3 Vec3Provider::Tick()
{
	return *this->source;
}

bool Vec3Provider::References(void* reference)
{
	return source == reference;
}