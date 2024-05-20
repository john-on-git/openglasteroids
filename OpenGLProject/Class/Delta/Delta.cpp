
#include <glm/glm.hpp>
#include "Delta.hpp"

Delta::Delta()
{
	this->magnitude = new glm::vec3(0.0f, 0.0f, 0.0f);
	this->now = nullptr;
	this->duration  = 0;
	this->maximum = -1;
}

Delta::Delta(glm::vec3* now, glm::vec3* magnitude, long long duration)
{
	this->magnitude = magnitude;
	this->now = now;
	this->duration = duration;
	this->maximum = -1;
}

Delta::Delta(glm::vec3* now, glm::vec3* magnitude)
{
	this->magnitude = magnitude;
	this->now = now;
	this->duration = -1;
	this->maximum = -1;
}

Delta::Delta(Delta* now, glm::vec3* magnitude)
{
	this->magnitude = magnitude;
	this->now = now->magnitude;
	this->duration = -1;
	this->maximum = -1;
}

Delta::Delta(Delta* now, glm::vec3* magnitude, long long duration)
{
	this->magnitude = magnitude;
	this->now = now->magnitude;
	this->duration = duration;
	this->maximum = -1;
}

Delta::Delta(Delta* now, glm::vec3* magnitude, float maximumMagnitude, long long duration)
{
	this->magnitude = magnitude;
	this->now = now->magnitude;
	this->duration = duration;
	this->maximum = maximumMagnitude;
}

Delta::~Delta()
{
	delete this->magnitude;
}

/// <summary>
/// do the thing
/// </summary>
/// <returns>true if this is finished</returns>
bool Delta::Tick()
{
	if (duration != 0) { 
		//update the value, limiting its magnitude between
		*now += *magnitude;
		float nowMag = glm::length(*now);
		if (!(maximum < 0 || nowMag <= maximum))
		{
			*now *= maximum / nowMag;
		}
	}
	if (duration > 0)
		duration--;
	return duration == 0;
}