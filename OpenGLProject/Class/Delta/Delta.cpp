#include "Delta.hpp"

Delta::Delta()
{
	this->magnitude = glm::vec3(0.0f, 0.0f, 0.0f);
	this->now = NULL;
	this->duration  = 0;
}

Delta::Delta(glm::vec3* now, glm::vec3 magnitude, long long duration)
{
	this->magnitude = magnitude;
	this->now = now;
	this->duration = duration;
}

Delta::Delta(glm::vec3* now, glm::vec3 magnitude)
{
	this->magnitude = magnitude;
	this->now = now;
	this->duration = -1;
}

Delta::Delta(Delta* now, glm::vec3 magnitude, long long duration)
{
	this->magnitude = magnitude;
	this->now = now->now;
	this->duration = duration;
}

Delta::Delta(Delta* now, glm::vec3 magnitude)
{
	this->magnitude = magnitude;
	this->now = now->now;
	this->duration = -1;
}

/// <summary>
/// do the thing
/// </summary>
/// <returns>true if this is finished</returns>
bool Delta::Tick()
{
	if (duration != 0)
		*now += magnitude;
	if (duration > 0)
		duration--;
	return duration == 0;
}