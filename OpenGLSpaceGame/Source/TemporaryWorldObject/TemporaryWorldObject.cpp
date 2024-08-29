#include "TemporaryWorldObject.hpp"

void TemporaryWorldObject::Tick()
{
	if (ticksRemaining > 0)
	{
		ticksRemaining--;
	}
	else
	{
		markedForDelete = true;
	}
}

TemporaryWorldObject::TemporaryWorldObject(Model* model, glm::vec3 position, glm::vec3 angle, glm::vec3 scale, GLuint projectionLocation, GLuint viewLocation, GLuint modelLocation, vector<tag> tags, long long int duration) : WorldObject(model, position, angle, scale, projectionLocation, viewLocation, modelLocation, tags)
{
	ticksRemaining = duration;
}